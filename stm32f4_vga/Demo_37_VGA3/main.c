//--------------------------------------------------------------
// File     : main.c
// Datum    : 06.10.2013
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der VGA-Font-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_font.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  UB_VGA_Screen_Init(); // init vom Screen

  // Bildschirm löschen
  UB_VGA_FillScreen(VGA_COL_BLUE);

  // schwarzen Text auf weisem Grund zeichnen mit verschiedenen Fonts
  UB_VGA_Font_DrawString(10,10,"Arial-7x10",&VGA_Arial_7x10,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,30,"Arial-8x13",&VGA_Arial_8x13,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,50,"Arial-10x15",&VGA_Arial_10x15,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,70,"Arial-11x18",&VGA_Arial_11x18,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,100,"Arial-13x19",&VGA_Arial_13x19,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,130,"Arial-14x22",&VGA_Arial_14x22,VGA_COL_BLACK,VGA_COL_WHITE);
  UB_VGA_Font_DrawString(10,160,"Arial-16x25",&VGA_Arial_16x25,VGA_COL_BLACK,VGA_COL_WHITE);
  // 32 Bit Font
  UB_VGA_Font_DrawString32(10,190,"Arial-18x27",&VGA_Arial_18x27,VGA_COL_BLACK,VGA_COL_WHITE);

  while(1)
  {

  }
}

