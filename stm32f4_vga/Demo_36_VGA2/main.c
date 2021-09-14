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
// Funktion : Demo der VGA-Graphic-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_graphic.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  UB_VGA_Screen_Init(); // Init vom Screen

  // LCD mit blau löschen
  UB_VGA_FillScreen(VGA_COL_BLUE);

  // Ein Image (aus dem Flash) Zeichnen
  UB_VGA_Graphic_DrawImage(&Emo1_VGA_Image,0,0);

  // rote Linie zeichnen
  UB_VGA_Graphic_DrawLine(10,20,100,150,VGA_COL_RED);

  // schwarzen kreis zeichnen
  UB_VGA_Graphic_DrawCircle(70,75,50,VGA_COL_BLACK);



  while(1)
  {

  }
}

