
#include "graphics_test.h"
#include "stm32_tick.h"

#define M_PI 3.14159

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

uint16_t random_int(int min, int max)
{
  return (uint16_t)rand() % max + min;
}

void lcd_test(void)
{
  for(uint32_t count=0;count<32;count++)
  {
    // BSP_LCD_SetRotation(random_int(0, 4));
    BSP_LCD_Clear(random_int(0, 0xffff));
  }

  // BSP_LCD_SetRotation(1);

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  uint16_t array_sin[BSP_LCD_GetXSize()];
  for(uint16_t i = 0; i < BSP_LCD_GetXSize(); i++) // count point
  {
    array_sin[i]=(uint16_t)16*sin(30*0.2*M_PI*i/180)+25;
  }

  for(uint32_t count=0;count<90;count+=5)
  {
    for(uint16_t i = 1; i < BSP_LCD_GetXSize(); i++)
    {
      BSP_LCD_SetTextColor(random_int(0, 0xffff));
      BSP_LCD_DrawLine(i, array_sin[i]+count*3,i-1,array_sin[i-1]+count*3);
    }
  }
  delay(1500);

  // BSP_LCD_SetRotation(0);
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  for(uint32_t i=0;i<300;i++)
  {
    BSP_LCD_SetTextColor(random_int(0, 0xffff));
    BSP_LCD_DrawCircle(random_int(0, BSP_LCD_GetXSize()),random_int(0, BSP_LCD_GetYSize()), random_int(5, 20));
  }
  delay(1500);

  // BSP_LCD_SetRotation(1);
  BSP_LCD_Clear(LCD_COLOR_YELLOW);
  for(uint32_t i=0;i<300;i++)
  {
    BSP_LCD_SetTextColor(random_int(0, 0xffff));
    BSP_LCD_DrawRect(random_int(0, BSP_LCD_GetXSize()),random_int(0, BSP_LCD_GetYSize()), random_int(10, 40),random_int(10, 40));
  }
  delay(1500);

  // BSP_LCD_SetRotation(3);
  BSP_LCD_Clear(random_int(0, 0xffff));
  BSP_LCD_SetTextColor(random_int(0, 0xffff));
  BSP_LCD_SetFont(&Font12);
  for(uint16_t i=0; i<20; i++)
  {
    BSP_LCD_SetTextColor(random_int(0, 0xffff));
    BSP_LCD_DisplayStringAt(random_int(10, BSP_LCD_GetXSize()),random_int(10, BSP_LCD_GetYSize()), "simple text", RIGHT_MODE);
  }
  delay(2500);
}

