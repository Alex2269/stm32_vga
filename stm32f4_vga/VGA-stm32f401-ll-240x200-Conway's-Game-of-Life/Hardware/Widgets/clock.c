#ifdef __cplusplus
 extern "C" {
#endif 

#include "clock.h"
#include "stm32_tick.h"

#define M_PI 3.14159

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

float mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx=120, osy=120, omx=120, omy=120, ohx=120, ohy=120;  // Saved H, M, S x & y coords
uint16_t x00=0, x11=0, y00=0, y11=0;
uint32_t targetTime = 0;                    // for next 1 second timeout
uint8_t hh, mm, ss;
uint8_t conv2d(const char* p);  // Get H, M, S from compile time
bool initial = 1;

uint8_t conv2d(const char* p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void clock(void)
{
  float sx = 0, sy = 1;
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_DrawCircle(120, 121, 110);
  BSP_LCD_DrawCircle(120, 121, 115);

  // Draw 12 lines
  for(int i = 0; i<360; i+= 30) 
  {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    x00 = sx*114+120;
    y00 = sy*114+120;
    x11 = sx*100+120;
    y11 = sy*100+120;
    BSP_LCD_DrawLine(x00, y00, x11, y11);
  }

  // Draw 60 dots
  for(int i = 0; i<360; i+= 6) 
  {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    x00 = sx*102+120;
    y00 = sy*102+120;
    // Draw minute markers
    BSP_LCD_DrawPixel(x00, y00, LCD_COLOR_WHITE);
    // Draw main quadrant dots
    if(i==0 || i==180) BSP_LCD_FillCircle(x00, y00, 2);
    if(i==90 || i==270) BSP_LCD_FillCircle(x00, y00, 2);
  }
  BSP_LCD_FillCircle(120, 121, 3);

  for(uint16_t i=0;i<2;i++)
  {
    if(!i) ss++; // Advance second
    delay(5);
  }
  if (ss==60) 
  {
    ss=0;
    mm++; // Advance minute
    if(mm>59) 
    {
      mm=0;
      hh++; // Advance hour
      if (hh>23) 
      {
        hh=0;
      }
    }
  }

  // Pre-compute hand degrees, x & y coords for a fast screen update
  sdeg = ss*6; // 0-59 -> 0-354
  mdeg = mm*6+sdeg*0.01666667; // 0-59 -> 0-360 - includes seconds
  hdeg = hh*30+mdeg*0.0833333; // 0-11 -> 0-360 - includes minutes and seconds
  hx = cos((hdeg-90)*0.0174532925); 
  hy = sin((hdeg-90)*0.0174532925);
  mx = cos((mdeg-90)*0.0174532925); 
  my = sin((mdeg-90)*0.0174532925);
  sx = cos((sdeg-90)*0.0174532925); 
  sy = sin((sdeg-90)*0.0174532925);

  if (ss==0 || initial) 
  {
    initial = 0;
    // Erase hour and minute hand positions every minute
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawLine(ohx, ohy, 120, 121);
    ohx = hx*62+121; 
    ohy = hy*62+121;
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawLine(omx, omy, 120, 121);
    omx = mx*84+120; 
    omy = my*84+121;
  }

  // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawLine(osx, osy, 120, 121);
  osx = sx*90+121; 
  osy = sy*90+121;
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawLine(osx, osy, 120, 121);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawLine(ohx, ohy, 120, 121);
  BSP_LCD_DrawLine(omx, omy, 120, 121);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawLine(osx, osy, 120, 121);
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillCircle(120, 121, 3);
}

