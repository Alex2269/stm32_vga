/**
 *
 * ugui_io.c
 *
**/

#include "main.h"
#include "gpio.h"
#include "ugui.h"

#include "ugui_io.h"

UG_GUI gui;

UG_S16 _width;
UG_S16 _height;

static inline void
delay_us(uint32_t volatile microsec)
{
  microsec *=(SystemCoreClock/1000000)/5;
  while(microsec--);
}

static inline void
delay(uint32_t volatile msec)
{
  msec *=(SystemCoreClock/1000)/5;
  while(msec--);
}

UG_S16 UG_get_width(UG_GUI* g)
{
  return g->x_dim;
}

UG_S16 UG_get_height(UG_GUI* g)
{
  return g->y_dim;
}

void UG_set_width(UG_GUI* g, UG_S16 width)
{
  g->x_dim = width;
}

void UG_set_height( UG_GUI* g, UG_S16 height)
{
  g->y_dim = height;
}

/* *** Glogal GUI structure for this. *** */

void Display_Init()
{
  // Initialize global structure and set PSET to this.PSET.
  // UG_Init(0, Display_PSet, UG_get_width(&gui), UG_get_height(&gui));
  // Register accelerators.
  UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
  UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
  UG_DriverRegister(DRIVER_DRAW_IMAGE, (void*)HW_DrawImage);
}


#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{
  if((x < 0) ||(x >= UG_get_width(&gui)) || (y < 0) || (y >= UG_get_height(&gui))) return;
  UB_VGA_SetPixel(x, y, c);
}
#pragma GCC pop_options

void UserSetPixel (UG_S16 x, UG_S16 y, UG_COLOR c) {
  /*
   * This line is specific to your board and libraries, and will need to be changed unless
   * you're using STM32Cube's BSP for the STM32F7 Disco Board. If not, you can remove
   * the "0xFF000000 |" part, it's just a way to fix uGUI not currently supporting the alpha channel.
   */
  UB_VGA_SetPixel(x, y, 0xFF000000 | c);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
UG_RESULT HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
  if((x1 < 0) || (x1 >= UG_get_width(&gui)) || (y1 < 0) || (y1 >= UG_get_height(&gui))) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= UG_get_width(&gui)) || (y2 < 0) || (y2 >= UG_get_height(&gui))) return UG_RESULT_FAIL;

  // If it is a vertical or a horizontal line, draw it.
  // If not, then use original drawline routine.

  if ((x1 == x2) || (y1 == y2))
  {
    HW_FillFrame(x1, y1, x2, y2, c);
    return UG_RESULT_OK;
  }
  return UG_RESULT_FAIL;
}
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
  uint16_t loopx, loopy;
  if((x1 < 0) || (x1 >= UG_get_width(&gui)) || (y1 < 0) || (y1 >= UG_get_height(&gui))) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= UG_get_width(&gui)) || (y2 < 0) || (y2 >= UG_get_height(&gui))) return UG_RESULT_FAIL;

  for (loopx = x1; loopx < x2 + 1; loopx++)
  {
    for (loopy = y1; loopy < y2 + 1; loopy++)
    {
      UB_VGA_SetPixel(loopx, loopy, c);
    }
  }
  return UG_RESULT_OK;
}
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize)
{
  if((x1 < 0) || (x1 >= UG_get_width(&gui)) || (y1 < 0) || (y1 >= UG_get_height(&gui))) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= UG_get_width(&gui)) || (y2 < 0) || (y2 >= UG_get_height(&gui))) return UG_RESULT_FAIL;

  uint16_t *ptr = image;

  for (uint16_t loopy = y1; loopy <= y2; loopy++)
  {
    for (uint16_t loopx = x1; loopx <= x2; loopx++)
    {
      UB_VGA_SetPixel(loopx, loopy, *(ptr++));
    }
  }

  return UG_RESULT_OK;
}
#pragma GCC pop_options

/* [] END OF FILE */
