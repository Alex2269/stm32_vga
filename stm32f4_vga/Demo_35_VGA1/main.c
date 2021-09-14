//--------------------------------------------------------------
// File     : main.c
// Datum    : 27.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der VGA-Library (320x240, 8bit)
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_screen.h"

int main(void)
{
  uint32_t n;

  SystemInit(); // Quarz Einstellungen aktivieren

  UB_VGA_Screen_Init(); // Init vom VGA-Screen

  // Screen mit einer Farbe loeschen
  UB_VGA_FillScreen(VGA_COL_BLUE);

  // einen roten Strich zeichnen
  for(n=0;n<100;n++) {
    UB_VGA_SetPixel(10+n,50,VGA_COL_RED);
  }

  while(1)
  {

  }
}

