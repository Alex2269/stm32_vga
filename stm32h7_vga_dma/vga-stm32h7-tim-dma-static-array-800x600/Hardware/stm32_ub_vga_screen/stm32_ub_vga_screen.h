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
#define VGA_DISPLAY_X   800
#define VGA_DISPLAY_Y   600
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
//--------------------------------------------------------------

#define VGA_TIM1_PERIODE        7 // 9
#define VGA_TIM1_PRESCALE       0

//--------------------------------------------------------------
// Timer-2
// Function  = CH4 : HSync-Signal on PB11
//             CH3 : Trigger point for DMA start
//--------------------------------------------------------------
/******** debug *****************
address:
TIM1->ARR     0x4001002C
TIM2->ARR     0x4000002C
TIM2->CCR3    0x4000003C
TIM2->CCR4    0x40000040

print (gdb):
x/1w 0x4001002C
x/1w 0x4000002C
x/1w 0x4000003C
x/1w 0x40000040

set (gdb):
set {int}0x4001002C=7
set {int}0x4000002C=6819
set {int}0x4000003C=1100
set {int}0x40000040=850

********************************/
#define  VGA_TIM2_HSYNC_PERIODE    6819
#define  VGA_TIM2_HSYNC_PRESCALE      0
// WRITE_REG(TIM2->CCR4, 800);
// WRITE_REG(TIM2->CCR3, 1400);
#define  VGA_TIM2_HSYNC_IMP         850  // HSync-length
#define  VGA_TIM2_HTRIGGER_START   1100  // HSync+BackPorch
//--------------------------------------------------------------

/***********************************
SVGA Signal 800 x 600 @ 56 Hz timing

Interested in easy to use VGA solution for embedded applications? Click here!

General timing
Screen refresh rate	56 Hz
Vertical refresh	35.15625 kHz
Pixel freq.	36.0 MHz
Horizontal timing (line)
Polarity of horizontal sync pulse is positive.

Scanline part	Pixels	Time [µs]
Visible area	800	22.222222222222
Front porch	24	0.66666666666667
Sync pulse	72	2
Back porch	128	3.5555555555556
Whole line	1024	28.444444444444
Vertical timing (frame)
Polarity of vertical sync pulse is positive.

Frame part	Lines	Time [ms]
Visible area	600	17.066666666667
Front porch	1	0.028444444444444
Sync pulse	2	0.056888888888889
Back porch	22	0.62577777777778
Whole frame	625	17.777777777778
***********************************/
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE         625   // Whole frame
#define  VGA_VSYNC_IMP               2   // Sync pulse
#define  V_VISIBLE_AREA            600
#define  V_BACK_PORCH               22
#define  V_FRONT_PORCH               1

#define  VGA_VSYNC_BILD_START      V_BACK_PORCH
#define  VGA_VSYNC_BILD_STOP       V_VISIBLE_AREA + V_FRONT_PORCH + VGA_VSYNC_IMP
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
