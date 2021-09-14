//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.11.2014
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der VGA-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
// In Configuration diese Define hinzufügen :
// "STM32F429_439xx" , "__ASSEMBLY__" , "USE_STDPERIPH_DRIVER"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_lcd_vga.h"
#include "stm32_ub_font.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  // init fuer VGA-Monitor
  UB_LCD_InitVGA();
  UB_LCD_LayerInit_Fullscreen();
  // auf Hintergrund schalten
  UB_LCD_SetLayer_1();
  // Hintergrund komplett mit einer Farbe füllen
  UB_LCD_FillLayer(RGB_COL_WHITE);
  // auf Vordergrund schalten
  UB_LCD_SetLayer_2();
  // Vordergrund komplett mit einer Farbe füllen
  UB_LCD_FillLayer(RGB_COL_GREEN);

  UB_Font_DrawString(10,10,"Arial-10x15",&Arial_10x15,RGB_COL_BLACK,RGB_COL_WHITE);


  while(1)
  {

  }
}

