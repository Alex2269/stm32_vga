//--------------------------------------------------------------
// File     : stm32_ub_vga_font.c
// Datum    : 06.10.2013
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : STM32_UB_VGA_SCREEN
// Funktion : Text-VGA Funktionen
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_font.h"



//--------------------------------------------------------------
// Zeichnet ein Ascii-Zeichen eines Fonts an x,y Position
// mit Vorder- und Hintergrundfarbe (Font = max 16 Pixel breite)
// -> Font muss mit &-Operator uebergeben werden
//--------------------------------------------------------------
void UB_VGA_Font_DrawChar(uint16_t x, uint16_t y, uint8_t ascii, UB_VGA_Font *font, uint8_t vg, uint8_t bg)
{
  uint16_t xn,yn,start_maske,maske;
  const uint16_t *wert;
  
  ascii -= 32;
  wert=&font->table[ascii * font->height];

  start_maske=0x80;
  if(font->width>8) start_maske=0x8000;

  for(yn = 0; yn < font->height; yn++) {
    maske=start_maske;
    for(xn = 0; xn < font->width; xn++) {
      if((wert[yn] & maske) == 0x00) {
        // Pixel in Hintergrundfarbe zeichnen
        UB_VGA_SetPixel(xn+x,yn+y,bg);
      }
      else {
        // Pixel in Vordergrundfarbe zeichnen
        UB_VGA_SetPixel(xn+x,yn+y,vg);
      }
      maske=(maske>>1);
    }
  }
}


//--------------------------------------------------------------
// Zeichnet einen String eines Fonts an x,y Position
// mit Vorder- und Hintergrundfarbe (Font = max 16 Pixel breite)
// -> Font muss mit &-Operator uebergeben werden
//--------------------------------------------------------------
void UB_VGA_Font_DrawString(uint16_t x, uint16_t y,char *ptr, UB_VGA_Font *font, uint8_t vg, uint8_t bg)
{
  uint16_t pos;

  pos=x;
  while (*ptr != 0) {
    UB_VGA_Font_DrawChar(pos,y,*ptr,font,vg,bg);
    pos+=font->width;
    ptr++;
  }
}


//--------------------------------------------------------------
// Zeichnet ein Ascii-Zeichen eines Fonts an x,y Position
// mit Vorder- und Hintergrundfarbe (Font = max 32 Pixel breite)
// -> Font muss mit &-Operator uebergeben werden
//--------------------------------------------------------------
void UB_VGA_Font_DrawChar32(uint16_t x, uint16_t y, uint8_t ascii, UB_VGA_Font32 *font, uint8_t vg, uint8_t bg)
{
  uint16_t xn,yn;
  uint32_t start_maske,maske;
  const uint32_t *wert;

  ascii -= 32;
  wert=&font->table[ascii * font->height];

  start_maske=0x80;
  if(font->width>8) start_maske=0x8000;
  if(font->width>16) start_maske=0x80000000;

  for(yn = 0; yn < font->height; yn++) {
    maske=start_maske;
    for(xn = 0; xn < font->width; xn++) {
      if((wert[yn] & maske) == 0x00) {
        // Pixel in Hintergrundfarbe zeichnen
        UB_VGA_SetPixel(xn+x,yn+y,bg);
      }
      else {
        // Pixel in Vordergrundfarbe zeichnen
        UB_VGA_SetPixel(xn+x,yn+y,vg);
      }
      maske=(maske>>1);
    }
  }
}


//--------------------------------------------------------------
// Zeichnet einen String eines Fonts an x,y Position
// mit Vorder- und Hintergrundfarbe (Font = max 32 Pixel breite)
// -> Font muss mit &-Operator uebergeben werden
//--------------------------------------------------------------
void UB_VGA_Font_DrawString32(uint16_t x, uint16_t y,char *ptr, UB_VGA_Font32 *font, uint8_t vg, uint8_t bg)
{
  uint16_t pos;

  pos=x;
  while (*ptr != 0) {
    UB_VGA_Font_DrawChar32(pos,y,*ptr,font,vg,bg);
    pos+=font->width;
    ptr++;
  }
}
