//--------------------------------------------------------------
// File     : stm32_ub_vga_graphic.c
// Datum    : 26.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : STM32_UB_VGA_SCREEN
// Funktion : Grafik-VGA Funktionen
//            (Punkte, Linien, Kreise)
//            und Bilder aus dem Flash anzeigen
//
// Formate  : 1. 8bit-Images (RGB332)
//
// Hinweis  : zum konvertieren der Bilder in das notwendige
//            Format, gibt es das PC-Programm :
//            "ImageGenerator_UB.exe"
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_graphic.h"


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
int16_t P_Graphic_sgn(int16_t x);



//--------------------------------------------------------------
// Zeichnet ein Pixel mit einer Farbe an x,y Position
// 0,0 = links oben
//--------------------------------------------------------------
void UB_VGA_Graphic_DrawPixel(int16_t xpos, int16_t ypos, uint8_t color)
{
  UB_VGA_SetPixel(xpos,ypos,color);
}


//--------------------------------------------------------------
// Zeichnet eine Linie mit einer Farbe
// von x1,y1 nach x2,y2
// 0,0 = links oben
// (benutzt wird der Bresenham-Algorithmus)
//--------------------------------------------------------------
void UB_VGA_Graphic_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
  int16_t x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

  // Entfernung in beiden Dimensionen berechnen
  dx = x2 - x1;
  dy = y2 - y1;

  // Vorzeichen des Inkrements bestimmen
  incx = P_Graphic_sgn(dx);
  incy = P_Graphic_sgn(dy);
  if(dx<0) dx = -dx;
  if(dy<0) dy = -dy;

  // feststellen, welche Entfernung größer ist
  if (dx>dy) {
    // x ist schnelle Richtung
    pdx=incx; pdy=0;    // pd. ist Parallelschritt
    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
    es =dy;   el =dx;   // Fehlerschritte schnell, langsam
  }
  else {
    // y ist schnelle Richtung
    pdx=0;    pdy=incy; // pd. ist Parallelschritt
    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
    es =dx;   el =dy;   // Fehlerschritte schnell, langsam
  }

  // Initialisierungen vor Schleifenbeginn
  x = x1;
  y = y1;
  err = (el>>1);
  UB_VGA_Graphic_DrawPixel(x,y,color);

  // Pixel berechnen
  for(t=0; t<el; ++t) // t zaehlt die Pixel, el ist auch Anzahl
  {
    // Aktualisierung Fehlerterm
    err -= es;
    if(err<0) {
      // Fehlerterm wieder positiv (>=0) machen
      err += el;
      // Schritt in langsame Richtung, Diagonalschritt
      x += ddx;
      y += ddy;
    }
    else {
      // Schritt in schnelle Richtung, Parallelschritt
      x += pdx;
      y += pdy;
    }
    UB_VGA_Graphic_DrawPixel(x,y,color);
  }
}


//--------------------------------------------------------------
// Zeichnet einen Kreis mit einer Farbe
// Mittelpunkt = x1,y1
// Radius = radius
// 0,0 = links oben
// (benutzt wird der Bresenham-Algorithmus)
//--------------------------------------------------------------
void UB_VGA_Graphic_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint8_t color)
{
  int16_t f=1-radius, ddF_x=0, ddF_y=-2*radius, x=0, y=radius;

  UB_VGA_Graphic_DrawPixel(x0, y0 + radius, color);
  UB_VGA_Graphic_DrawPixel(x0, y0 - radius, color);
  UB_VGA_Graphic_DrawPixel(x0 + radius, y0, color);
  UB_VGA_Graphic_DrawPixel(x0 - radius, y0, color);

  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;

    UB_VGA_Graphic_DrawPixel(x0 + x, y0 + y, color);
    UB_VGA_Graphic_DrawPixel(x0 - x, y0 + y, color);
    UB_VGA_Graphic_DrawPixel(x0 + x, y0 - y, color);
    UB_VGA_Graphic_DrawPixel(x0 - x, y0 - y, color);
    UB_VGA_Graphic_DrawPixel(x0 + y, y0 + x, color);
    UB_VGA_Graphic_DrawPixel(x0 - y, y0 + x, color);
    UB_VGA_Graphic_DrawPixel(x0 + y, y0 - x, color);
    UB_VGA_Graphic_DrawPixel(x0 - y, y0 - x, color);
  }
}



#if VGA_GRAPHIC_USE_IMG==1
//--------------------------------------------------------------
// Zeichnet ein 8bit Image (aus dem Flash)
// linke untere Ecke = xpos,ypos
// -> Image muss mit &-Operator uebergeben werden
// Return_wert :
//     VGA_GRAPHIC_OK , Bild gezeichnet
//  != VGA_GRAPHIC_OK , fehler beim zeichnen
// Konvertierungs-Programm : "ImageGenerator_UB.exe"
//--------------------------------------------------------------
VGA_GRAPHIC_ERR_t UB_VGA_Graphic_DrawImage(UB_VGA_Image *img, int16_t xpos, int16_t ypos)
{
  VGA_GRAPHIC_ERR_t ret_wert=VGA_GRAPHIC_OK;
  uint16_t xn,yn;
  uint8_t color;
  const uint8_t *wert;

  wert=&img->table[0];


  if(img->width>VGA_DISPLAY_Y) return(VGA_GRAPHIC_WIDTH_ERR);
  if(img->height>VGA_DISPLAY_X) return(VGA_GRAPHIC_HEIGHT_ERR);

  for(yn = 0; yn < img->height; yn++) {
    for(xn = 0; xn < img->width; xn++) {
      color = wert[yn*img->width+xn];
      UB_VGA_Graphic_DrawPixel(yn+xpos,VGA_DISPLAY_Y-xn-1-ypos,color);
    }
  }

  return(ret_wert);
}
#endif



//--------------------------------------------------------------
// interne Funktion
// Signum funktion
//  Return_Wert
//    1 ,wenn x > 0
//    0 ,wenn x = 0
//   -1 ,wenn x < 0
//--------------------------------------------------------------
int16_t P_Graphic_sgn(int16_t x)
{
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}
