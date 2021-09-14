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
#define VGA_DISPLAY_X   640
#define VGA_DISPLAY_Y   480

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

#define VGA_TIM1_PERIODE       16
#define VGA_TIM1_PRESCALE       0

//--------------------------------------------------------------
// Timer-2
// Function  = CH4 : HSync-Signal on PB11
//             CH3 : Trigger point for DMA start
//
// basefreq = 2*APB1 (APB1=48MHz) => TIM_CLK=84MHz
// Frq       = 84MHz/1/2668 = 31,48kHz => T = 31,76us
// 1TIC      = 11,90ns
//

//--------------------------------------------------------------
/******** debug *****************
address:
TIM1->ARR     0x4001002C
TIM2->ARR     0x4000002C

TIM2->CCR3    0x4000003C
TIM2->CCR4    0x40000040

set (gdb):
set {int}0x4001002C=16
set {int}0x4000002C=6640

print (gdb):
x/1w 0x4001002C
x/1w 0x4000002C

print (gdb):
x/1w 0x4000003C
x/1w 0x40000040

set (gdb):
set {int}0x4000003C=1000
set {int}0x40000040=840

********************************/

#define  VGA_TIM2_HSYNC_PERIODE    6640
#define  VGA_TIM2_HSYNC_PRESCALE      0

#define  VGA_TIM2_HSYNC_IMP         840  // HSync-length (3,81us)
// WRITE_REG(TIM2->CCR4, 1100);

// WRITE_REG(TIM2->CCR3, 1400);
#define  VGA_TIM2_HTRIGGER_START   1000  // HSync+BackPorch (5,71us)
#define  VGA_TIM2_DMA_DELAY           0  // ease the delay when DMA START (Optimization = none)
//--------------------------------------------------------------
// VSync-Signal
// Trigger   = Timer2 Update (f=31,48kHz => T = 31,76us)
// 1TIC      = 31,76us
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE         525   // Whole frame	525	16.683217477656
#define  VGA_VSYNC_IMP               2   // Sync pulse
#define  VGA_VSYNC_BILD_STOP       540   // (16,38ms)
#define  VGA_VSYNC_BILD_START       60

//--------------------------------------------------------------
// Adress from PORTE (Reg ODR) callback DMA
// (see Page 53+204 of the Manual)
//
// Data-Bit0 => PE8
// Data-Bit7 => PE15
//--------------------------------------------------------------
#define VGA_GPIO_ODR_OFFSET    ((uint32_t)0x00000014) // ADR from Register ODR
#define VGA_GPIO_BYTE_OFFSET   ((uint32_t)0x00000001) // Data for 8bit
#define VGA_GPIOE_ODR_ADDRESS   (GPIOE_BASE | VGA_GPIO_ODR_OFFSET | VGA_GPIO_BYTE_OFFSET)

//--------------------------------------------------------------
// Define for black on PE8 - PE15
//--------------------------------------------------------------
#define VGA_GPIO_HINIBBLE  ((uint16_t)0xFF00) // GPIO_Pin_8 to GPIO_Pin_15


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

/* include for memory alloc/free */
#include "multi_heap_4.h"
#define  LCD_MALLOC       d1Malloc
#define  LCD_FREE         d1Free

#endif // __STM32F4_UB_VGA_SCREEN_H
