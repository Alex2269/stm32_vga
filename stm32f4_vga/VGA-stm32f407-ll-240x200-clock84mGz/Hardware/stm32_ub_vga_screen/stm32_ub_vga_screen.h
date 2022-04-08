//--------------------------------------------------------------
// File     : stm32_ub_vga_screen.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_SCREEN_H
#define __STM32F4_UB_VGA_SCREEN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "main.h"
#include "stm32f4xx_ll_dma_flags.h"

#define VGA_BLUE0_Pin LL_GPIO_PIN_8
#define VGA_BLUE0_GPIO_Port GPIOB

//--------------------------------------------------------------
// color designation
// 8bit color (R3G3B2)
// Red   (3bit) -> Bit7-Bit5
// Green (3bit) -> Bit4-Bit2
// Blue  (2bit) -> Bit1-Bit0
//--------------------------------------------------------------
#define  VGA_COL_BLACK          0x00
#define  VGA_COL_BLUE           0x03
#define  VGA_COL_GREEN          0x1C
#define  VGA_COL_RED            0xE0
#define  VGA_COL_WHITE          0xFF

#define  VGA_COL_CYAN           0x1F
#define  VGA_COL_MAGENTA        0xE3
#define  VGA_COL_YELLOW         0xFC

//--------------------------------------------------------------
// define the VGA_display
//--------------------------------------------------------------

#define VGA_DISPLAY_X   240
#define VGA_DISPLAY_Y   200

//--------------------------------------------------------------
// VGA Structure
//--------------------------------------------------------------
typedef struct {
  uint16_t hsync_cnt;   // counter
  uint32_t start_adr;   // start_adres
  uint32_t dma2_cr_reg; // Register constant CR-Register
} VGA_t;

extern VGA_t VGA;
extern uint16_t UB_VGA_size_X;
extern uint16_t UB_VGA_size_Y;

//--------------------------------------------------------------
// Display RAM
//--------------------------------------------------------------
// extern uint8_t *VGA_RAM1;
extern uint8_t VGA_RAM1[];

//--------------------------------------------------------------
// Timer-1
// Function  = Pixelclock (Speed for  DMA Transfer)
//
// basefreq = 2*APB2 (APB2=84MHz) => TIM_CLK=168MHz
// Frq       = 168MHz/1/12 = 14MHz
//
//--------------------------------------------------------------
#define VGA_TIM1_PERIODE       8 // TIM1->ARR
#define VGA_TIM1_PRESCALE      0

//--------------------------------------------------------------
/******** debug *****************
address:
TIM1->ARR     0x4001002c
TIM2->ARR     0x4000002c
TIM3->ARR     0x4000042c
TIM4->ARR     0x4000082c

TIM2->CCR1    0x40000034
TIM2->CCR2    0x40000038
TIM2->CCR3    0x4000003c
TIM2->CCR4    0x40000040

TIM3->CCR1    0x40000434
TIM3->CCR2    0x40000438
TIM3->CCR3    0x4000043c
TIM3->CCR4    0x40000440

TIM4->CCR1    0x40000834
TIM4->CCR2    0x40000838
TIM4->CCR3    0x4000083c
TIM4->CCR4    0x40000840

print (gdb):
x/1d 0x4001002c
x/1d 0x4000042c
x/1d 0x40000434
x/1d 0x4000043c

set (gdb):
TIM1:
set {int}0x4001002c=7

: 72mgz
set {int}0x4001002c=7
set {int}0x4000042c=2235
set {int}0x40000434=440
set {int}0x4000043c=300

: 84mgz
set {int}0x4001002c=8
set {int}0x4000042c=2660
set {int}0x40000434=360
set {int}0x4000043c=200

: 100mhz
set {int}0x4001002c=10
set {int}0x4000042c=3180
set {int}0x40000434=500
set {int}0x4000043c=300

: 120mhz
set {int}0x4001002c=7
set {int}0x4000042c=3660
set {int}0x40000434=440
set {int}0x4000043c=300

: 125mhz
set {int}0x4001002c=12
set {int}0x4000042c=3900
set {int}0x40000434=380
set {int}0x4000043c=120

: 128mhz
set {int}0x4001002c=12
set {int}0x4000042c=4010
set {int}0x40000434=400
set {int}0x4000043c=120
********************************/

//--------------------------------------------------------------
// Timer-2
// Function  = CH4 : HSync-Signal on PB11
//             CH3 : Trigger point for DMA start
//--------------------------------------------------------------
#define  VGA_TIM3_HSYNC_PERIODE   2660  // TIM3->ARR
#define  VGA_TIM3_HSYNC_PRESCALE     0

#define  VGA_TIM3_HSYNC_IMP        200  // HSync-length (3,81us) TIMx->CCR3 CompareValue
#define  VGA_TIM3_HTRIGGER_START   360  // HSync+BackPorch (5,71us) TIMx->CCR1 CompareValue

// // Visible area	640	25.42
// // Front porch	16	0.63
// // Sync pulse	96	3.81
// // Back porch	48	1.90
// // Whole line	800	31.77

//--------------------------------------------------------------
// VSync-Signal
// Trigger   = TIMx Update (f=31,48kHz => T = 31,76us)
// 1TIC      = 31,76us
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE         480 // Vertical Visible area
#define  VERTICAL_FRONT_PORCH       10 // not use
#define  VGA_VSYNC_IMP               2 // Vertical Sync pulse
#define  VGA_VSYNC_BILD_START       33 // Vertical Back porch
#define  VGA_VSYNC_BILD_STOP       525 // Vertical Whole frame

//--------------------------------------------------------------
// Adress from PORTE (Reg ODR) callback DMA
// (see Page 53+204 of the Manual)
//
// Data-Bit0 => PE8
// Data-Bit7 => PE15
//--------------------------------------------------------------
// #define GPIOx_BASE // ADR from Port-x
#define VGA_ODR_OFFSET    ((uint32_t)0x00000014) // ADR from Register ODR
#define VGA_BYTE_OFFSET   ((uint32_t)0x00000001) // Data for 8bit
// #define VGA_ADDRESS    (GPIOE_BASE | VGA_ODR_OFFSET | VGA_BYTE_OFFSET)
#define VGA_ADDRESS       (GPIOB_BASE | VGA_ODR_OFFSET | VGA_BYTE_OFFSET)

//--------------------------------------------------------------
// Define for black on PE8 - PE15
//--------------------------------------------------------------
#define VGA_HINIBBLE  ((uint16_t)0xFF00) // GPIO_Pin_8 to GPIO_Pin_15

/*
void hw_tim_start(TIM_TypeDef *TIMx)
{
  TIMx->CNT=0;
  TIMx->BDTR |= TIM_BDTR_MOE;
  TIMx->CR1  |= TIM_CR1_CEN;
}

void hw_pwm_chan_start(TIM_TypeDef *TIMx, uint32_t Channel)
{
  TIMx->CCER |= (uint32_t)0x01 << ( (Channel - 1) * 4 );
}

void hw_pwm_chan_stop(TIM_TypeDef *TIMx, uint32_t Channel)
{
  TIMx->CCER &= ~((uint32_t)0x01 << ( (Channel - 1) * 4 ));
}
*/

static inline
void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Enable capture/compare interrupt.
{
  TIMx->DIER |= (uint32_t)0x01 << Interrupt;
}

static inline
void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Disable capture/compare x interrupt.
{
  TIMx->DIER &= ~(uint32_t)0x01 << Interrupt;
}

static inline
void TIM_ClearFlag_CC(TIM_TypeDef *TIMx, uint32_t CCxFlag) // Clear the Capture/Compare x interrupt flag
{
  TIMx->SR &= ~(uint32_t)0x01 << CCxFlag;
}

/*
 * Indicate whether Capture/Compare interrupt flag
 * is set (Capture/Compare flag interrupt is pending).
 */
static inline
uint32_t TIM_IsActiveFlag_CC(TIM_TypeDef *TIMx, uint32_t flag)
{
  return ((READ_BIT(TIMx->SR, flag) == (flag)) ? 1UL : 0UL);
}

/* Exported functions ------------------------------------------------------- */
/* IRQ Handler treatment.*/
static inline
void DMA_Transfer_Complete(DMA_TypeDef *DMAx, uint32_t Stream)
{
  #define TIMx TIM1

  /* Enable DMA stream. */
  // LL_DMA_EnableStream(DMAx, Stream); // DMA2_Stream5->CR |= DMA_SxCR_EN;

  /* Disable DMA stream. */
  // LL_DMA_DisableStream(DMAx, Stream); // DMA2_Stream5->CR &= ~DMA_SxCR_EN;

  /* Disable timer counter. */
  LL_TIM_DisableCounter(TIMx);

  // switch on black
  VGA_BLUE0_GPIO_Port->BSRR = VGA_HINIBBLE << 16u;

  LL_DMA_ClearFlag_TEx(DMAx, Stream); // LL_DMA_ClearFlag_TE5(DMAx);
  LL_DMA_ClearFlag_TCx(DMAx, Stream); // LL_DMA_ClearFlag_TC5(DMAx);
  LL_DMA_DisableIT_DME(DMAx, Stream); // Disable Direct mode error interrupt.

  /* Enable interrupts:
   * Transfer error interrupt.     DMA_SxCR_TEIE
   * Transfer complete interrupt.  DMA_SxCR_TCIE
   * Direct mode error interrupt.  DMA_SxCR_DMEIE */
  // DMA2_Stream5->CR    |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;
  LL_DMA_EnableIT_TC(DMAx, Stream); // Enable Transfer complete interrupt.
  LL_DMA_EnableIT_TE(DMAx, Stream); // Enable Transfer error interrupt.
  LL_DMA_EnableIT_DME(DMAx, Stream); // Enable Direct mode error interrupt.
}

static inline
void DMA_Transfer_Error(DMA_TypeDef *DMAx, uint32_t Stream)
{
}

uint16_t UB_VGA_Get_X(void);
uint16_t UB_VGA_Get_Y(void);
void UB_VGA_Set_X(uint16_t x);
void UB_VGA_Set_Y(uint16_t y);
//--------------------------------------------------------------
// Global Function call
//--------------------------------------------------------------
void UB_VGA_ll_Init(void);
void UB_VGA_Screen_Init(uint16_t vga_x, uint16_t vga_y);
void UB_VGA_FillScreen(uint8_t color);
void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color);
uint32_t UB_VGA_GetPixel(uint16_t xp, uint16_t yp);

//--------------------------------------------------------------
#endif // __STM32F4_UB_VGA_SCREEN_H
