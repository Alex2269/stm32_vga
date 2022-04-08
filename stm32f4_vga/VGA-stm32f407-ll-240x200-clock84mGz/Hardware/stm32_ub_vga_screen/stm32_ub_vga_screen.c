//--------------------------------------------------------------
// File     : stm32_ub_vga_320x240.c
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, TIM, MISC, DMA
// Function : VGA out by GPIO (320x240 Pixel, 8bit color)
//
// signals  : PB11      = HSync-Signal  13
//            PB12      = VSync-Signal  14
//            PE8+PE9   = color Blue    3
//            PE10-PE12 = color Green   2
//            PE13-PE15 = color red     1
//
// uses     : TIM1, TIM3
//            DMA2, Channel6, Stream5
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_screen.h"
#include "stdint.h"
#include "stdlib.h"


VGA_t VGA;

uint16_t UB_VGA_size_X;
uint16_t UB_VGA_size_Y;

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
uint8_t VGA_RAM1[VGA_DISPLAY_X*VGA_DISPLAY_Y]={NULL};

void UB_VGA_ll_Init(void)
{
  LL_FLASH_EnableInstCache(); // FLASH->ACR |= FLASH_ACR_ICEN;
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  /* Init the low level hardware */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG); // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR); // RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}

static void DMA_SetConf(DMA_TypeDef *DMAx, uint32_t Stream, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  LL_DMA_DisableDoubleBufferMode(DMAx, Stream);
  LL_DMA_SetDataLength(DMAx, Stream, DataLength);
  LL_DMA_SetMemoryAddress(DMAx, Stream, SrcAddress);
  LL_DMA_SetPeriphAddress(DMAx, Stream, DstAddress);
  LL_DMA_EnableIT_TC(DMAx, Stream); // Enable Transfer complete interrupt.
  LL_DMA_EnableIT_TE(DMAx, Stream); // Enable Transfer error interrupt.
  LL_DMA_EnableIT_DME(DMAx, Stream); // Enable Direct mode error interrupt.
}

//--------------------------------------------------------------
// Init VGA-Module
//--------------------------------------------------------------
void UB_VGA_Screen_Init(uint16_t vga_x, uint16_t vga_y)
{
  #define TIMx TIM3
  #define CC_Channel 1
  #define DMAx DMA2
  #define Stream 5
  #define TIM_Channel LL_TIM_CHANNEL_CH3
  UB_VGA_Set_X(vga_x);
  UB_VGA_Set_Y(vga_y);

  // VGA_RAM1 = (uint8_t *)malloc(((vga_x) * vga_y) * sizeof(uint8_t));

  VGA.hsync_cnt = 0;
  VGA.start_adr = 0;
  VGA.dma2_cr_reg = 0;

  LL_TIM_EnableCounter(TIMx);
  LL_TIM_CC_EnableChannel(TIMx, TIM_Channel);
  // LL_TIM_EnableIT_CC3(TIMx); // Enable capture/compare 3 interrupt (CC3IE).
  TIM_EnableIT(TIMx, CC_Channel); // Enable capture/compare interrupt.
  LL_TIM_EnableCounter(TIM1);

  DMA_SetConf(DMAx, Stream, VGA_RAM1, VGA_ADDRESS, UB_VGA_Get_X());
  // DMA_EnableStream(DMAx, Stream);  // __HAL_DMA_ENABLE(hdma);
  LL_DMA_DisableDoubleBufferMode(DMAx, Stream);
  LL_TIM_EnableDMAReq_UPDATE(TIM1); // Enable update DMA request (UDE).

  /* Check if the DMA Stream is effectively disabled */
  LL_DMA_DisableStream(DMAx, Stream);
  while(LL_DMA_IsEnabledStream(DMAx, Stream)) __asm volatile ("nop");

  // LL_DMA_EnableStream(DMAx, Stream);

  //-----------------------
  // Register swap and safe
  //-----------------------
  // content of CR-Register read and save
  VGA.dma2_cr_reg =((DMA_Stream_TypeDef *)((uint32_t)((uint32_t)DMAx + STREAM_OFFSET_TAB[Stream])))->CR;
  // VGA.dma2_cr_reg = DMA2_Stream5->CR;
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
  VGA_RAM1[UB_VGA_Get_X() * yp + xp] = color;
}

uint32_t UB_VGA_GetPixel(uint16_t xp, uint16_t yp)
{
  if(xp >= UB_VGA_Get_X()) return;
  if(yp >= UB_VGA_Get_Y()) return;

  // Read pixel to ram
  return VGA_RAM1[UB_VGA_Get_X() * yp + xp];
}
