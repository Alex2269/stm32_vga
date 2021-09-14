/**
 ***************************************************************
 *@file 	main.c
 *@author 	Menno Janssen and Benno Driessen
 *@date		29 may 2018
 *@brief	Main program body can also be seen as the front layer.
 ***************************************************************
 */

#include "main.h"
#include "stdint.h"
#include "stm32_ub_vga_shapes.h"

uint16_t rand16(void)
{
  uint16_t rnd = random();
  return rnd;
}

extern char string[100];
int main(void)
{
  IO_init();
  // parser parsedData;

  int x1,x2,y1,y2;
  int i,j,k,l;

  x1 = 50;
  y1 = 50;
  x2 = 150;
  y2 = 150;

  while(1)
  {
    UB_VGA_FillScreen(0);
    for(uint32_t i=0;i<5e5;i++)
    {
      // UB_VGA_drawLine(rand16()%320, rand16()%240, rand16()%320, rand16()%240, 1, rand16());
      UB_VGA_SetPixel(rand16()%320, rand16()%240, 0);   
      if(i%100) UB_VGA_SetPixel(rand16()%320, rand16()%240, rand16());
      // Draw_Text(rand16()%320, rand16()%240, "Hello!", rand16());
      for(uint32_t i=0;i<10;i++) __asm volatile ("nop");
    }

    UB_VGA_FillScreen(0);
    for(uint32_t i=0;i<2e2;i++)
    {
      // UB_VGA_drawLine(rand16()%320, rand16()%240, rand16()%320, rand16()%240, 1, rand16());
      // UB_VGA_SetPixel(rand16()%320, rand16()%240, rand16());
      Draw_Text(rand16()%320, rand16()%240, "Hello!", rand16());
      for(uint32_t i=0;i<1e6;i++) __asm volatile ("nop");
    }

	for(i=x1;i<=x2;i++) UB_VGA_SetPixel(i,y1,127);
	for(j=x1;j<=x2;j++) UB_VGA_SetPixel(j,y2,200);
	for(k=y1;k<=y2;k++) UB_VGA_SetPixel(x1,k,50);
	for(l=y1;l<=y2;l++) UB_VGA_SetPixel(x2,l,255);
  }
}


