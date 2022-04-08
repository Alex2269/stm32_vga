//--------------------------------------------------------------
// File     : stm32_ub_vga_text.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_TEXT_H
#define __STM32F4_UB_VGA_TEXT_H
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "main.h"
// #include "stm32f4xx.h"

//--------------------------------------------------------------

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
#define	 VGA_COL_GRAY			0x92
#define  VGA_COL_BROWN			0xB0


#define  VGA_COL_LBLUE          0x97
#define  VGA_COL_LGREEN         0x9E
#define  VGA_COL_LRED           0xF2
#define  VGA_COL_LCYAN          0x9B
#define  VGA_COL_LMAGENTA       0xD3
#define  VGA_COL_BROWN          0x80
#define  VGA_COL_GREY	        0x92
#define  VGA_COL_PINK	        0xF6
#define  VGA_COL_PURPLE	        0x82

uint8_t Draw_Bitmap(uint8_t nr,uint16_t xp, uint16_t yp);
void tft_putchar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t f_color, uint8_t b_color);
void tft_putchar14x24(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color);
void tft_putchar18x32(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color);
void tft_putchar26x48(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color);
void tft_puts26x48(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color);
void tft_puts18x32(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color);
void tft_puts14x24(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color);
void tft_puts8x16(uint16_t row, uint16_t column, int8_t *string,  uint8_t size, uint8_t f_color, uint8_t b_color);

#include "fonts.h"

#endif// __STM32F4_UB_VGA_TEXT_H
