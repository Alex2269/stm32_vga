//--------------------------------------------------------------
// File     : stm32_ub_vga_screen.c
// CPU      : STM32F4
// IDE      : Makefile
// Module   : GPIO, TIM, DMA
// Function : VGA out by GPIO (240x200 Pixel, 8bit color)
//
// signals  : PB0       = HSync-Signal  13
//            PC13      = VSync-Signal  14
//            PB8+PB9   = color Blue    3
//            PB10-PB12 = color Green   2
//            PB13-PB15 = color red     1
//
// uses     : TIM1, TIM3
// LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_5, LL_DMA_CHANNEL_6);
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_screen.h"
#include "stdint.h"
#include "stdlib.h"

static uint32_t dma2_cr_reg; // Register constant CR-Register
uint16_t UB_VGA_size_X;
uint16_t UB_VGA_size_Y;

/**
  * @brief  Locks the FLASH control register access
  * Защита от записи автоматически устанавливается при системном сбросе.
  * Тем не менее, настоятельно рекомендуется явно заблокировать память
  * после завершения всех операций записи.
  * Это предотвращает любую случайную запись, вызванную сбоем микропро-
  * граммы или нестабильностью питания.
  */
void FLASH_Lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Bank1 Control Register access */
  FLASH->CR1 |= FLASH_CR_LOCK;
  /* Set the LOCK Bit to lock the FLASH Bank2 Control Register access */
  FLASH->CR2 |= FLASH_CR_LOCK;
}

uint16_t UB_VGA_Get_X(void)
{
  return UB_VGA_size_X;
}

uint16_t UB_VGA_Get_Y(void)
{
  return UB_VGA_size_Y;
}

void UB_VGA_Set_X(uint16_t x)
{
  UB_VGA_size_X = x;
}

void UB_VGA_Set_Y(uint16_t y)
{
  UB_VGA_size_Y = y;
}

//--------------------------------------------------------------
// Display RAM
//--------------------------------------------------------------
// uint8_t *VGA_RAM1;
uint8_t VGA_RAM1[(VGA_DISPLAY_X*VGA_DISPLAY_Y)]={NULL};
uint8_t VGA_RAM2[1]={NULL};

void UB_VGA_ll_Init(void)
{
  FLASH_Lock();
  SCB_EnableICache();
  SCB_DisableDCache();

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  /* Init the low level hardware */

  /** @brief  Enables the Compensation cell Power Down
   *  @note   The I/O compensation cell can be used only when the device supply
   *          voltage ranges from 2.4 to 3.6 V */
  LL_SYSCFG_EnableCompensationCell();

  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
  // LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);
  // LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_ADC3);
}

static void
DMA_SetConf(DMA_TypeDef *DMAx, uint32_t Stream, uint32_t SrcAddress, uint32_t SrcAddress2, uint32_t DstAddress, uint32_t DataLength)
{
  // LL_DMA_EnableDoubleBufferMode(DMAx, Stream);
  LL_DMA_DisableDoubleBufferMode(DMAx, Stream);
  LL_DMA_SetDataLength(DMAx, Stream, DataLength);
  LL_DMA_SetMemoryAddress(DMAx, Stream, SrcAddress);
  // LL_DMA_SetMemory1Address(DMAx, Stream, SrcAddress2);
  LL_DMA_SetPeriphAddress(DMAx, Stream, DstAddress);

  // LL_DMA_EnableIT_HT(DMAx, Stream); // Enable Half transfer interrupt.
  LL_DMA_EnableIT_TC(DMAx, Stream); // Enable Transfer complete interrupt.
  LL_DMA_EnableIT_TE(DMAx, Stream); // Enable Transfer error interrupt.
  // LL_DMA_EnableIT_DME(DMAx, Stream); // Enable Direct mode error interrupt.
}

//--------------------------------------------------------------
// Init VGA-Module
//--------------------------------------------------------------
void UB_VGA_Screen_Init(uint16_t vga_x, uint16_t vga_y)
{
  #define TIMx TIM3
  uint32_t TIM_Ch1 = LL_TIM_CHANNEL_CH1;
  DMA_Stream_TypeDef *DMAx = DMA2;
  uint8_t Stream = 5;
  uint32_t TIM_Ch3 = LL_TIM_CHANNEL_CH3;

  UB_VGA_Set_X(vga_x);
  UB_VGA_Set_Y(vga_y);

  // VGA_RAM1 = (uint8_t *)malloc(((vga_x) * vga_y) * sizeof(uint8_t));

  LL_TIM_EnableCounter(TIMx);
  LL_TIM_CC_EnableChannel(TIMx, TIM_Ch3);
  TIM_EnableIT(TIMx, TIM_Ch1); // Enable capture/compare interrupt.
  LL_TIM_EnableCounter(TIM1);

  DMA_SetConf(DMAx, Stream, VGA_RAM1, VGA_RAM2, VGA_ADDRESS, UB_VGA_Get_X());

  LL_DMA_DisableDoubleBufferMode(DMAx, Stream);
  LL_TIM_EnableDMAReq_UPDATE(TIM1); // Enable update DMA request (UDE).

  // content of CR-Register read and save
  dma2_cr_reg = LL_DMA_GetRegiter_CR(DMAx, Stream); // dma2_cr_reg = DMA2_Stream5->CR;
}

//--------------------------------------------------------------
// fill the DMA RAM buffer with one color
//--------------------------------------------------------------
void UB_VGA_FillScreen(uint8_t color)
{
  uint16_t xp,yp;

  for(yp = 0; yp < UB_VGA_Get_Y(); yp++) {
    for(xp = 0; xp < UB_VGA_Get_X(); xp++) {
      UB_VGA_SetPixel(xp, yp, color);
    }
  }
}

//--------------------------------------------------------------
// put one Pixel on the screen with one color
//--------------------------------------------------------------
void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp >= UB_VGA_Get_X()) return;
  if(yp >= UB_VGA_Get_Y()) return;

  // Write pixel to ram
  // if(yp <= (UB_VGA_Get_Y()/2))
  VGA_RAM1[UB_VGA_Get_X() * yp + xp] = color;
  // else
  //   VGA_RAM2[UB_VGA_Get_X() * yp + xp] = color;
}

uint32_t UB_VGA_GetPixel(uint16_t xp, uint16_t yp)
{
  if(xp >= UB_VGA_Get_X()) return;
  if(yp >= UB_VGA_Get_Y()) return;

  // Read pixel from ram
  // if(yp <= (UB_VGA_Get_Y()/2))
   return VGA_RAM1[UB_VGA_Get_X() * yp + xp];
  // else
  //  return VGA_RAM2[UB_VGA_Get_X() * yp + xp];
}

void VGA_horizontal_sync(DMA_TypeDef *DMAx, uint32_t Stream, uint16_t h_line_beg, uint16_t h_line_end)
{
  #define TIMx TIM1

  static uint16_t h_line_count;
  static uint32_t dma_address;

  TIM_ClearFlag_CC(TIM3, 1);

  h_line_count++;
  if (h_line_count > VERTICAL_VISIBLE_AREA)
  {
    h_line_count = 0;
    dma_address = (uint32_t)VGA_RAM1; // Adresspointer first dot
  }

  if(h_line_count < VERTICAL_SYNC_PULSE) LL_GPIO_ResetOutputPin(VGA_VSYNC_GPIO_Port, VGA_VSYNC_Pin);
  else LL_GPIO_SetOutputPin(VGA_VSYNC_GPIO_Port, VGA_VSYNC_Pin);

  // Test for DMA start
  if((h_line_count > h_line_beg) && (h_line_count < h_line_end))
  {
    // after FP start => DMA Transfer
    LL_DMA_SetRegiter_CR(DMAx, Stream, dma2_cr_reg);
    LL_DMA_SetMemoryAddress(DMAx, Stream, dma_address);
    LL_TIM_EnableCounter(TIMx);
    LL_DMA_EnableStream(DMAx, Stream);

    // if(h_line_count == (h_line_end - h_line_beg)/2)
    //   dma_address = (uint32_t)VGA_RAM1;

    // if(!(h_line_count % 2)) // Test Adrespointer for high
      dma_address += UB_VGA_Get_X(); // inc after Hsync
  }
}

void DMA_Transfer_Complete(DMA_TypeDef *DMAx, uint32_t Stream)
{
  #define TIMx TIM1

  /* Check if the DMA Stream is effectively disabled */
  LL_DMA_DisableStream(DMAx, Stream);
  while(LL_DMA_IsEnabledStream(DMAx, Stream)) delay_us(1);

  /* Disable timer counter. */
  LL_TIM_DisableCounter(TIMx);

  // switch on black
  VGA_BLUE0_GPIO_Port->BSRR = VGA_HINIBBLE << 16u;

  // LL_DMA_ClearFlag_HT(DMAx, Stream); // Clear Stream x half transfer flag.
  // LL_DMA_EnableIT_HT(DMAx, Stream); // Enable Half transfer interrupt.
  LL_DMA_ClearFlag_TC(DMAx, Stream); // Clear Stream x transfer complete flag.
  LL_DMA_EnableIT_TC(DMAx, Stream); // Enable Transfer complete interrupt.
  LL_DMA_ClearFlag_TE(DMAx, Stream); // Clear Stream x transfer error flag.
  LL_DMA_EnableIT_TE(DMAx, Stream); // Enable Transfer error interrupt.
  // LL_DMA_DisableIT_DME(DMAx, Stream); // Disable Direct mode error interrupt.
  // LL_DMA_EnableIT_DME(DMAx, Stream); // Enable Direct mode error interrupt.
}

//! @brief This function handles TIM3 global interrupt.
// void TIM3_IRQHandler(void) { VGA_horizontal_sync(DMA2, 5, VERTICAL_BACK_PORCH, VERTICAL_WHOLE_FRAME); }

//! @brief This function handles DMA2 stream5 global interrupt.
// void DMA2_Stream5_IRQHandler(void) { DMA_Transfer_Complete(DMA2, 5); }

