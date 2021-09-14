/**
 *
 * ugui_io.h
 *
**/

#include "main.h"
#include "gpio.h"
#include "ugui.h"

extern UG_GUI gui;

UG_S16 UG_get_width(UG_GUI* g);
UG_S16 UG_get_height(UG_GUI* g);
void UG_set_width(UG_GUI* g, UG_S16 width);
void UG_set_height( UG_GUI* g, UG_S16 height);

/* *** Glogal GUI structure for this. *** */
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c);
UG_RESULT HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
UG_RESULT HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize);

void UserPixelSetFunction(UG_S16 x, UG_S16 y, UG_U16 c);
void UserSetPixel (UG_S16 x, UG_S16 y, UG_COLOR c);

/* [] END OF FILE */
