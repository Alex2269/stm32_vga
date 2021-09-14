//--------------------------------------------------------------
// File     : stm32_ub_vga_graphic.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_GRAPHIC_H
#define __STM32F4_UB_VGA_GRAPHIC_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32_ub_vga_screen.h"


//--------------------------------------------------------------
// Aktivieren, Deaktivieren von einer Funktion
// 1=aktiv, 0=inaktiv
//--------------------------------------------------------------
#define   VGA_GRAPHIC_USE_IMG     1   // Image-Funktionen


//--------------------------------------------------------------
// Struktur von einem Image
//--------------------------------------------------------------
typedef struct UB_VGA_Image_t
{
  const uint8_t *table;  // Tabelle mit den Daten
  uint16_t width;        // Breite des Bildes (in Pixel)
  uint16_t height;       // Hoehe des Bildes  (in Pixel)
}UB_VGA_Image;



//--------------------------------------------------------------
// Einbinden der benutzen Bilder
// fuer alle Bilder muss das entsprechende File
// auch in der CooCox-IDE hinzugefügt werden
//--------------------------------------------------------------
extern UB_VGA_Image Emo1_VGA_Image;
extern UB_VGA_Image Emo2_VGA_Image;



//--------------------------------------------------------------
// Return Werte beim zeichnen
//--------------------------------------------------------------
typedef enum {
  VGA_GRAPHIC_OK =0,
  VGA_GRAPHIC_WIDTH_ERR,
  VGA_GRAPHIC_HEIGHT_ERR
}VGA_GRAPHIC_ERR_t;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_VGA_Graphic_DrawPixel(int16_t xpos, int16_t ypos, uint8_t color); 
void UB_VGA_Graphic_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
void UB_VGA_Graphic_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint8_t color);
#if VGA_GRAPHIC_USE_IMG==1
VGA_GRAPHIC_ERR_t UB_VGA_Graphic_DrawImage(UB_VGA_Image *img, int16_t xpos, int16_t ypos);
#endif




//--------------------------------------------------------------
#endif // __STM32F4_UB_VGA_GRAPHIC_H
