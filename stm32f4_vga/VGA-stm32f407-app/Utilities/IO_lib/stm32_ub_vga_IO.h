//--------------------------------------------------------------
// File     : stm32_ub_vga_IO.h
//--------------------------------------------------------------


#ifndef __STM32F4_UB_VGA_IO_H
#define __STM32F4_UB_VGA_IO_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_dma.h"


#include "stm32_ub_vga_shapes.h"

//--------------------------------------------------------------



//--------------------------------------------------------------
// define the VGA_display
//--------------------------------------------------------------
#define UB_VGA_Get_X()   320
#define UB_VGA_Get_Y()   240



//--------------------------------------------------------------
// VGA Structure
//--------------------------------------------------------------
typedef struct {
  uint16_t hsync_cnt;   // counter
  uint32_t start_adr;   // start_adres
  uint32_t dma2_cr_reg; // Register constant CR-Register
}VGA_t;

extern VGA_t VGA;

//--------------------------------------------------------------
// Display RAM
//--------------------------------------------------------------
extern uint8_t VGA_RAM1[(UB_VGA_Get_X()+1)*UB_VGA_Get_Y()];


//--------------------------------------------------------------
// Timer-1
// Function  = Pixelclock (Speed for  DMA Transfer)
//
// basefreq = 2*APB2 (APB2=84MHz) => TIM_CLK=168MHz
// Frq       = 168MHz/1/12 = 14MHz
//
//--------------------------------------------------------------
#define VGA_TIM1_PERIODE      12
#define VGA_TIM1_PRESCALE      0



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
#define  VGA_TIM2_HSYNC_PERIODE   2667
#define  VGA_TIM2_HSYNC_PRESCALE     0

#define  VGA_TIM2_HSYNC_IMP       320  // HSync-length (3,81us)
#define  VGA_TIM2_HTRIGGER_START  480  // HSync+BackPorch (5,71us)
#define  VGA_TIM2_DMA_DELAY        60  // ease the delay when DMA START (Optimization = none)
//#define  VGA_TIM2_DMA_DELAY        30  // ease the delay when DMA START (Optimization = -O1)


//--------------------------------------------------------------
// VSync-Signal
// Trigger   = Timer2 Update (f=31,48kHz => T = 31,76us)
// 1TIC      = 31,76us
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE        525
#define  VGA_VSYNC_IMP              2
#define  VGA_VSYNC_BILD_START      36
#define  VGA_VSYNC_BILD_STOP      514   // (16,38ms)


//--------------------------------------------------------------
// Adress from PORTE (Reg ODR) callback DMA
// (see Page 53+204 of the Manual)
//
// Data-Bit0 => PE8
// Data-Bit7 => PE15
//--------------------------------------------------------------
#define VGA_GPIOE_BASE_ADR     ((uint32_t)0x40021000) // ADR from Port-E
#define VGA_GPIO_ODR_OFFSET    ((uint32_t)0x00000014) // ADR from Register ODR
#define VGA_GPIO_BYTE_OFFSET   ((uint32_t)0x00000001) // Data for 8bit
#define VGA_GPIOE_ODR_ADDRESS   (VGA_GPIOE_BASE_ADR | VGA_GPIO_ODR_OFFSET | VGA_GPIO_BYTE_OFFSET)

//--------------------------------------------------------------
// Define for black on PE8 - PE15
//--------------------------------------------------------------
#define VGA_GPIO_HINIBBLE  ((uint16_t)0xFF00) // GPIO_Pin_8 to GPIO_Pin_15

//--------------------------------------------------------------
// Global Function call
//--------------------------------------------------------------
void UB_VGA_Screen_Init(void);


void UB_VGA_delay(uint16_t milliseconds);


#endif //__STM32F4_UB_VGA_IO_H


