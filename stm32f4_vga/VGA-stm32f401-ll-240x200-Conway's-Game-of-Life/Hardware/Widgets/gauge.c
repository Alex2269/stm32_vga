
#include "gauge.h"
#include "stm32_tick.h"

#define M_PI 3.14159

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

void init_gauge(uint16_t coordinate_x, uint16_t coordinate_y)
{
 static volatile uint8_t grawflag = 0 ;
 float sx = 0, sy = 0; // Saved x & y multipliers
 
 uint16_t x00=0, x11=0, y00=0, y11=0, x22=0, y22=0;
 coordinate_x += 120;
 coordinate_y += 120;

 if(grawflag) return;
 grawflag=1;

 for(uint16_t i = 0; i<=380; i ++)
 {
  dev.pointX[i] =   cos((i - 100)*0.0174532925);
  dev.pointY[i] =   sin((i - 100)*0.0174532925);
 }
  // Draw gauge
  for(uint16_t i = 270; i<=370; i += 10)
  {
    sx = dev.pointX[i];
    sy = dev.pointY[i];

    x00 = sx*102+coordinate_x; // coordinates for points
    y00 = sy*102+coordinate_y;

    x11 = sx*118+coordinate_x; // coordinates for lines
    y11 = sy*118+coordinate_y;

    x22 = sx*132+coordinate_x; // coordinates for letters
    y22 = sy*132+coordinate_y;

    // Draw minute markers
    //GaugeWritePixel(x00, y00, LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    if(i==270||i==360) BSP_LCD_FillCircle(x00, y00, 3); // draw min & max red points
    // Draw main quadrant dots
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    if(i%90) BSP_LCD_FillCircle(x00, y00, 2);

    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_DrawLine(x00, y00, x11, y11); // draw small lines

    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayChar(x22, y22, ('0'-27+i/10)); // draw letters
  }
}

uint8_t char2digit(const char* p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t rotate, uint16_t color, uint16_t val)
{
  init_gauge(coordinate_x, coordinate_y);

  float sx = 0, sy = 0;    // Saved x & y multipliers
  float beg_sx, beg_sy, end_sx, end_sy;   // Saved x & y coords
  float bal_sx, bal_sy;   // Saved x & y coords fot bal

  static uint16_t  oldX1,oldY1,oldX2,oldY2,oldbalx,oldbaly; // for clean screen, saved old values

  coordinate_x += 120;
  coordinate_y += 120;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawLine(oldX1, oldY1, oldX2, oldY2); // Erase old ray
  BSP_LCD_FillCircle(oldbalx, oldbaly, 2);

  // Pre-compute hand degrees, x & y coords for a fast screen update

  sx = dev.pointX[val];
  sy = dev.pointY[val];

  beg_sx = sx*8+coordinate_x; // center x point
  beg_sy = sy*8+coordinate_y; // center y point

  end_sx = sx*95+coordinate_x; // end x point
  end_sy = sy*95+coordinate_y; // end y point

  bal_sx = sx*85+coordinate_x; // end x point
  bal_sy = sy*85+coordinate_y; // end y point

  BSP_LCD_SetTextColor(color);
  BSP_LCD_DrawLine(beg_sx,beg_sy, end_sx,end_sy); // draw ray

  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  BSP_LCD_FillCircle(bal_sx,bal_sy, 2);
  // save values for clean ray
  oldX1 = beg_sx; oldY1 = beg_sy;
  oldX2 = end_sx; oldY2 = end_sy;
  oldbalx = bal_sx;
  oldbaly = bal_sy;

  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillCircle(coordinate_x, coordinate_y, 2);
  BSP_LCD_DrawCircle(coordinate_x, coordinate_y, 6); // draw center circle
}
