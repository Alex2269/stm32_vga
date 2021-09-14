//--------------------------------------------------------------
// File     : stm32_ub_lcd_vga.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_LCD_VGA_H
#define __STM32F4_UB_LCD_VGA_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_ltdc.h"
#include "stm32_ub_sdram.h"





//--------------------------------------------------------------
// Standard Farbwerte
// 16bit Farbwert (R5G6B5)
// Red   (5bit) -> Bit15-Bit11
// Green (6bit) -> Bit10-Bit5
// Blue  (5bit) -> Bit4-Bit0
//--------------------------------------------------------------
#define  RGB_COL_BLACK          0x0000
#define  RGB_COL_BLUE           0x001F
#define  RGB_COL_GREEN          0x07E0
#define  RGB_COL_RED            0xF800
#define  RGB_COL_WHITE          0xFFFF

#define  RGB_COL_CYAN           0x07FF
#define  RGB_COL_MAGENTA        0xF81F
#define  RGB_COL_YELLOW         0xFFE0

#define  RGB_COL_GREY           0xF7DE


//--------------------------------------------------------------
// Display Mode
//--------------------------------------------------------------
typedef enum {
  PORTRAIT =0,
  LANDSCAPE
} LCD_MODE_t;

extern LCD_MODE_t  LCD_DISPLAY_MODE;

//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
extern  uint32_t LCD_CurrentFrameBuffer;   // aktuelle Adresse zum zeichnen
extern  uint32_t LCD_CurrentLayer;         // 0=Hintergrund, 1=Vodergrund
extern  uint32_t LCD_CurrentOrientation;   // 0=0Grad, 1=180Grad


//--------------------------------------------------------------
// Einstellung der Aufloesung und der Ausrichtung
// auf dem VGA-Screen
//--------------------------------------------------------------
//#define  USE_VGA_MODE    0  // 0 = 640x480 Pixel
//#define  USE_VGA_MODE    1  // 1 = 320x240 Pixel
#define  USE_VGA_MODE    2  // 2 = 240x320 Pixel


#if USE_VGA_MODE==0
  #define  LCD_MAXX           ((uint16_t)480)      // Pixel in X-Richtung
  #define  LCD_MAXY           ((uint16_t)640)      // Pixel in Y-Richtung
  #define  VGA_DIRECTION      0
  #define  VGA_OFFSET_H       0
  #define  VGA_OFFSET_V       0
#endif
#if USE_VGA_MODE==1
  #define  LCD_MAXX           ((uint16_t)240)      // Pixel in X-Richtung
  #define  LCD_MAXY           ((uint16_t)320)      // Pixel in Y-Richtung
  #define  VGA_DIRECTION      0
  #define  VGA_OFFSET_H        (640-LCD_MAXY)/2
  #define  VGA_OFFSET_V        (480-LCD_MAXX)/2
#endif
#if USE_VGA_MODE==2
  #define  LCD_MAXX           ((uint16_t)240)      // Pixel in X-Richtung
  #define  LCD_MAXY           ((uint16_t)320)      // Pixel in Y-Richtung
  #define  VGA_DIRECTION      1
  #define  VGA_OFFSET_H        (640-LCD_MAXX)/2
  #define  VGA_OFFSET_V        (480-LCD_MAXY)/2
#endif

#define  LCD_PIXEL  LCD_MAXX*LCD_MAXY



//--------------------------------------------------------------
// Defines für das Display-RAM
//--------------------------------------------------------------
#define  LCD_FRAME_BUFFER     SDRAM_START_ADR         // Start vom SDRAM
#define  LCD_FRAME_OFFSET   ((uint32_t)(LCD_PIXEL*2)) // größe von einem Puffer (in Byte)







//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_LCD_InitVGA(void);
void UB_LCD_LayerInit_Fullscreen(void);
void UB_LCD_SetLayer_1(void);
void UB_LCD_SetLayer_2(void);
void UB_LCD_FillLayer(uint16_t color);
void UB_LCD_SetTransparency(uint8_t wert);
void UB_LCD_SetCursor2Draw(uint16_t xpos, uint16_t ypos);
void UB_LCD_DrawPixel(uint16_t color);
void UB_LCD_SetMode(LCD_MODE_t mode);
void UB_LCD_Copy_Layer1_to_Layer2(void);
void UB_LCD_Copy_Layer2_to_Layer1(void);
void UB_LCD_Refresh(void);


//--------------------------------------------------------------
#endif // __STM32F4_UB_LCD_VGA_H
