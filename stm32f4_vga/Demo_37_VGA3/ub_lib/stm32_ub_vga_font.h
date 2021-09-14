//--------------------------------------------------------------
// File     : stm32_ub_vga_font.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_FONT_H
#define __STM32F4_UB_VGA_FONT_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32_ub_vga_screen.h"




//--------------------------------------------------------------
// Struktur von einem Font (max 16 Pixel Breite)
//-------------------------------------------------------------- 
typedef struct UB_VGA_Font_t {
  const uint16_t *table; // Tabelle mit den Daten
  uint16_t width;        // Breite eines Zeichens (in Pixel)
  uint16_t height;       // Hoehe eines Zeichens  (in Pixel)
}UB_VGA_Font;


//--------------------------------------------------------------
// Struktur von einem Font (max 32 Pixel Breite)
//--------------------------------------------------------------
typedef struct UB_VGA_Font32_t {
  const uint32_t *table; // Tabelle mit den Daten
  uint16_t width;        // Breite eines Zeichens (in Pixel)
  uint16_t height;       // Hoehe eines Zeichens  (in Pixel)
}UB_VGA_Font32;


//--------------------------------------------------------------
// Aktivierung der verschiedenen Fonts
// (unbenutzte einfach auskommentieren)
// fuer alle benutzten Fonts muss das entsprechende C-File
// in der CooCox-IDE hinzugefügt werden
//--------------------------------------------------------------
extern UB_VGA_Font VGA_Arial_7x10;
extern UB_VGA_Font VGA_Arial_8x13;
extern UB_VGA_Font VGA_Arial_10x15;
extern UB_VGA_Font VGA_Arial_11x18;
extern UB_VGA_Font VGA_Arial_13x19;
extern UB_VGA_Font VGA_Arial_14x22;
extern UB_VGA_Font VGA_Arial_16x25;
extern UB_VGA_Font32 VGA_Arial_18x27;





//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_VGA_Font_DrawChar(uint16_t x, uint16_t y, uint8_t ascii, UB_VGA_Font *font, uint8_t vg, uint8_t bg);
void UB_VGA_Font_DrawString(uint16_t x, uint16_t y,char *ptr, UB_VGA_Font *font, uint8_t vg, uint8_t bg);
void UB_VGA_Font_DrawChar32(uint16_t x, uint16_t y, uint8_t ascii, UB_VGA_Font32 *font, uint8_t vg, uint8_t bg);
void UB_VGA_Font_DrawString32(uint16_t x, uint16_t y,char *ptr, UB_VGA_Font32 *font, uint8_t vg, uint8_t bg);




//--------------------------------------------------------------
#endif // __STM32F4_UB_VGA_FONT_H
