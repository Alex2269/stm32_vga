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
#include "stm32f4xx_ll_dma_mix.h"

//--------------------------------------------------------------
// color designation
// 8bit  color (R3G3B2)
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

#define VGA_DISPLAY_X   290
#define VGA_DISPLAY_Y   200

extern uint16_t UB_VGA_size_X;
extern uint16_t UB_VGA_size_Y;

//--------------------------------------------------------------
/******** debug *****************
address:
TIM1->CNT     0x40010024
TIM2->CNT     0x40000024
TIM3->CNT     0x40000424
TIM4->CNT     0x40000824

TIM1->PSC     0x40010028
TIM2->PSC     0x40000028
TIM3->PSC     0x40000428
TIM4->PSC     0x40000828

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
x/1d 0x40000428
x/1d 0x40000434
x/1d 0x4000043c

: 72mgz
set {int}0x40000428=0
set {int}0x4001002c=7
set {int}0x4000042c=2235
set {int}0x40000434=440
set {int}0x4000043c=300

: 84mgz
set {int}0x40000428=0
set {int}0x4001002c=6
set {int}0x4000042c=2660
set {int}0x40000434=360
set {int}0x4000043c=200

: 84mgz
set {int}0x40000428=2
set {int}0x4001002c=7
set {int}0x4000042c=880
set {int}0x40000434=200
set {int}0x4000043c=140

: 100mhz
set {int}0x40000428=0
set {int}0x4001002c=10
set {int}0x4000042c=3180
set {int}0x40000434=500
set {int}0x4000043c=300

: 120mhz
set {int}0x40000428=0
set {int}0x4001002c=7
set {int}0x4000042c=3660
set {int}0x40000434=440
set {int}0x4000043c=300

: 125mhz
set {int}0x40000428=0
set {int}0x4001002c=12
set {int}0x4000042c=3900
set {int}0x40000434=380
set {int}0x4000043c=120

: 128mhz
set {int}0x40000428=0
set {int}0x4001002c=12
set {int}0x4000042c=4010
set {int}0x40000434=400
set {int}0x4000043c=120

-----------
: 120mhz v2
set {int}0x40000428=2
set {int}0x4000042c=1280
set {int}0x4000043c=140
set {int}0x40000434=180
-----------

set {int}0x4001002c       VGA_TIM1_PERIODE // TIM1->ARR
set {int}0x40000428       VGA_TIM3_HSYNC_PRESCALE
set {int}0x4000042c       VGA_TIM3_HSYNC_PERIODE
set {int}0x4000043c       VGA_TIM3_HSYNC_IMP
set {int}0x40000434       VGA_TIM3_HTRIGGER_START

********************************/

//--------------------------------------------------------------
// Timer-1
// Function  = Pixelclock (Speed for  DMA Transfer)
//
// basefreq = 2*APB2 (APB2=84MHz) => TIM_CLK=168MHz
// Frq       = 168MHz/1/12 = 14MHz
//
//--------------------------------------------------------------
#define VGA_TIM1_PERIODE       7 // 8 // TIM1->ARR
#define VGA_TIM1_PRESCALE      0

//--------------------------------------------------------------
// Timer-2
// Function  = CH4 : HSync-Signal on PB11
//             CH3 : Trigger point for DMA start
//--------------------------------------------------------------
#define  VGA_TIM3_HSYNC_PERIODE     880 // 3660 // TIM3->ARR
#define  VGA_TIM3_HSYNC_PRESCALE      2

#define  VGA_TIM3_HSYNC_IMP         140 // HSync-length (3,81us) TIMx->CCR3 CompareValue
#define  VGA_TIM3_HTRIGGER_START    200 // HSync+BackPorch (5,71us) TIMx->CCR1 CompareValue

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
#define  VERTICAL_VISIBLE_AREA     480
#define  VERTICAL_FRONT_PORCH       10 // not use
#define  VERTICAL_SYNC_PULSE         2
#define  VERTICAL_BACK_PORCH        33
#define  VERTICAL_WHOLE_FRAME      525

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
void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Enable capture/compare TIMx interrupt.
{
  TIMx->DIER |= (uint32_t)0x01 << Interrupt;
}

static inline
void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Disable capture/compare TIMx interrupt.
{
  TIMx->DIER &= ~(uint32_t)0x01 << Interrupt;
}

static inline
void TIM_ClearFlag_CC(TIM_TypeDef *TIMx, uint32_t CCxFlag) // Clear the capture/compare TIMx interrupt flag
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

void VGA_horizontal_sync(DMA_TypeDef *DMAx, uint32_t Stream, uint16_t v_line_beg, uint16_t v_line_end);
void DMA_Transfer_Complete(DMA_TypeDef *DMAx, uint32_t Stream);


#endif // __STM32F4_UB_VGA_SCREEN_H
