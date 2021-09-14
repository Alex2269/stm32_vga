//--------------------------------------------------------------
// File     : stm32_ub_vga_shapes.h
//--------------------------------------------------------------


//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_SHAPES_H
#define __STM32F4_UB_VGA_SHAPES_H
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "main.h"
#include "stm32f4xx.h"
#include "stm32_ub_vga_IO.h"

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




//--------------------------------------------------------------
// Global Function call
//--------------------------------------------------------------
uint8_t UB_VGA_drawLine(uint16_t x_start,uint16_t y_start,uint16_t x_stop, uint16_t y_stop, uint8_t width, uint8_t color);
uint8_t UB_VGA_drawEllipse(long xmp,long ymp,long x_radius, long y_radius, uint8_t color);
uint8_t UB_VGA_drawRectangle(uint16_t x_lo,uint16_t y_lo,uint16_t x_rb, uint16_t y_rb, uint8_t color);
uint8_t UB_VGA_drawTriangle(uint16_t x_one,uint16_t y_one,uint16_t x_two, uint16_t y_two ,uint16_t x_tree, uint16_t y_tree, uint8_t color);
//@TODO datatypes for text are not right
uint8_t UB_VGA_drawText(uint16_t x_start,uint16_t y_start, uint16_t x_fontname, uint16_t style , uint8_t color);
uint8_t UB_VGA_setBitmap(uint8_t img_number,uint16_t x_start, uint16_t y_start);
uint8_t UB_VGA_clearscreen(uint8_t color);
uint8_t UB_VGA_FillScreen(uint8_t color);
uint8_t UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color);
uint8_t Draw_Bitmap(uint8_t nr,uint16_t xp, uint16_t yp);
uint8_t Draw_Text(uint16_t x0, uint16_t y0, uint8_t *text, uint8_t color);



//--------------------------------------------------------------
#endif// __STM32F4_UB_VGA_SHAPES_H


