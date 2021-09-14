#include "vga_adafruit.h"
#include "stm32_ub_vga_screen.h"

uint16_t cursor_x;
uint16_t cursor_y;
uint8_t textsize ;
bool wrap; // If set, 'wrap' text at right edge of display

static UG_GUI* gui;

void writePixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  UB_VGA_SetPixel(xp, yp, color);
}

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  // --
  if (steep)
  {
    swap(x0, y0);
    swap(x1, y1);
  }
  // --
  if (x0 > x1)
  {
    swap(x0, x1);
    swap(y0, y1);
  }
  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int16_t err = dx / 2;
  int16_t ystep;
  // --
  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }
  for (; x0<=x1; x0++)
  {
    // --
    if (steep)
    {
      writePixel(y0, x0, color);
    }
    else
    {
      writePixel(x0, y0, color);
    }
    err -= dy;
    // --
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  drawFastVLine(x, y, h, color);
}

void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  drawFastHLine(x, y, w, color);
}

void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  fillRect(x,y,w,h,color);
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  writeLine(x, y, x, y+h-1, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  writeLine(x, y, x+w-1, y, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  for (int16_t i=x; i<x+w; i++)
  {
    writeFastVLine(i, y, h, color);
  }
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  // Update in subclasses if desired!
  // --
  if(x0 == x1)
  {
    // --
    if(y0 > y1) swap(y0, y1);
    drawFastVLine(x0, y0, y1 - y0 + 1, color);
  }
  else if(y0 == y1)
  {
    // --
    if(x0 > x1) swap(x0, x1);
    drawFastHLine(x0, y0, x1 - x0 + 1, color);
  }
  else
  {
    writeLine(x0, y0, x1, y1, color);
  }
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  writePixel(x0 , y0+r, color);
  writePixel(x0 , y0-r, color);
  writePixel(x0+r, y0 , color);
  writePixel(x0-r, y0 , color);
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    writePixel(x0 + x, y0 + y, color);
    writePixel(x0 - x, y0 + y, color);
    writePixel(x0 + x, y0 - y, color);
    writePixel(x0 - x, y0 - y, color);
    writePixel(x0 + y, y0 + x, color);
    writePixel(x0 - y, y0 + x, color);
    writePixel(x0 + y, y0 - x, color);
    writePixel(x0 - y, y0 - x, color);
  }
}

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // --
    if (cornername & 0x4)
    {
      writePixel(x0 + x, y0 + y, color);
      writePixel(x0 + y, y0 + x, color);
    }
    // --
    if (cornername & 0x2)
    {
      writePixel(x0 + x, y0 - y, color);
      writePixel(x0 + y, y0 - x, color);
    }
    // --
    if (cornername & 0x8)
    {
      writePixel(x0 - y, y0 + x, color);
      writePixel(x0 - x, y0 + y, color);
    }
    // --
    if (cornername & 0x1)
    {
      writePixel(x0 - y, y0 - x, color);
      writePixel(x0 - x, y0 - y, color);
    }
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  writeFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // --
    if (cornername & 0x1)
    {
      writeFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      writeFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    // --
    if (cornername & 0x2)
    {
      writeFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      writeFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  writeFastHLine(x, y, w, color);
  writeFastHLine(x, y+h-1, w, color);
  writeFastVLine(x, y, h, color);
  writeFastVLine(x+w-1, y, h, color);
}

void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  writeFastHLine(x+r , y , w-2*r, color);
  writeFastHLine(x+r , y+h-1, w-2*r, color);
  writeFastVLine(x , y+r , h-2*r, color);
  writeFastVLine(x+w-1, y+r , h-2*r, color);
  drawCircleHelper(x+r , y+r , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r , y+h-r-1, r, 8, color);
}

void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  writeFillRect(x+r, y, w-2*r, h, color);
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r , y+r, r, 2, h-2*r-1, color);
}

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  // --
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  // -
  // --
  if (y1 > y2)
  {
    swap(y2, y1); swap(x2, x1);
  }
  // -
  // --
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  // -
  // --
  if(y0 == y2)
  {
    // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    // --
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    // --
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    writeFastHLine(a, y0, b-a+1, color);
    return;
  }
  // -
  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t
  sa = 0, sb = 0;
  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2. If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  // --
  if(y1 == y2) last = y1; // Include y1 scanline
  else last = y1-1; // Skip it
  for(y=y0; y<=last; y++)
  {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    // --
    if(a > b) swap(a,b);
    writeFastHLine(a, y, b-a+1, color);
  }
  // -
  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2. This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++)
  {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    // --
    if(a > b) swap(a,b);
    writeFastHLine(a, y, b-a+1, color);
  }
  // -
}

void drawBitmap_U8(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      // --
      if(i & 7) byte <<= 1;
      else byte = bitmap[j * byteWidth + i / 8];
      // --
      if(byte & 0x80) writePixel(x+i, y, color);
    }
  }
}

void drawBitmap_U8_color(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      // --
      if(i & 7) byte <<= 1;
      else byte = bitmap[j * byteWidth + i / 8];
      writePixel(x+i, y, (byte & 0x80) ? color : bg);
    }
  }
}

void drawXBitmap_U8(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      // --
      if(i & 7) byte >>= 1;
      else byte = bitmap[j * byteWidth + i / 8];
      // --
      if(byte & 0x01) writePixel(x+i, y, color);
    }
  }
}

void drawGrayscaleBitmap_U8(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      writePixel(x+i, y, bitmap[j * w + i]);
    }
  }
}

void drawGrayscaleBitmap_U8_mask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      // --
      if(i & 7) byte <<= 1;
      else byte = mask[j * bw + i / 8];
      // --
      if(byte & 0x80)
      {
        writePixel(x+i, y, bitmap[j * w + i]);
      }
    }
  }
}

void drawRGBBitmap_U16(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      writePixel(x+i, y, bitmap[j * w + i]);
    }
  }
}

void drawRGBBitmap_U16_mask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      // --
      if(i & 7) byte <<= 1;
      else byte = mask[j * bw + i / 8];
      // --
      if(byte & 0x80)
      {
        writePixel(x+i, y, bitmap[j * w + i]);
      }
    }
  }
}

void setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

int16_t getCursorX(void)
{
  return cursor_x;
}

int16_t getCursorY(void)
{
  return cursor_y;
}

void setTextSize(uint8_t s)
{
  textsize = (s > 0) ? s : 1;
}

void UG_PutString( UG_S16 x, UG_S16 y, char* str, UG_COLOR fc, UG_COLOR bc, const UG_FONT* font)
{
  UG_S16 xp,yp;
  UG_U8 cw;
  char chr;

  xp=x;
  yp=y;

  while ( *str != 0 )
  {
    chr = *str++;
    if (chr < font->start_char || chr > font->end_char) continue;

    cw = font->widths ? font->widths[chr - font->start_char] : font->char_width;

    if(chr==' ') xp += cw * textsize;
    else UG_PutChar(chr, xp, yp, fc, bc, font);

    xp += cw * textsize + 2; // + gui->char_h_space;
  }
}

void UG_PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc, const UG_FONT* font)
{
  UG_U16 i,j,k,xo,yo,c,bn,actual_char_width;
  UG_U8 b,bt;
  UG_U32 index;
  UG_COLOR color;

  void(*push_pixel)(UG_COLOR);

  bt = (UG_U8)chr;

  if (bt < font->start_char || bt > font->end_char) return;

  yo = y;
  bn = font->char_width;
  if ( !bn ) return;
  bn >>= 3;
  if ( font->char_width % 8 ) bn++;
  actual_char_width = (font->widths ? font->widths[bt - font->start_char] : font->char_width);

  /*Not accelerated output*/
  if (font->font_type == FONT_TYPE_1BPP)
  {
    index = (bt - font->start_char)* font->char_height * bn;
    for( j=0;j<font->char_height;j++ )
    {
      xo = x;
      c=actual_char_width;
      for( i=0;i<bn;i++ )
      {
        b = font->p[index++];
        for( k=0;(k<8) && c;k++ )
        {
          if( b & 0x01 )
          {
            // writePixel(xo,yo, fc);
            fillRect(xo,yo,textsize,textsize,fc);
          }
          else
          {
            // writePixel(xo,yo, bc);
            fillRect(xo,yo,textsize,textsize,bc);
          }
          b >>= 1;
          xo+=textsize;
          c--;
        }
      }
      yo+=textsize;
    }
  }
  else if (font->font_type == FONT_TYPE_8BPP)
  {
    index = (bt - font->start_char)* font->char_height * font->char_width;
    for( j=0;j<font->char_height;j++ )
    {
      xo = x;
      for( i=0;i<actual_char_width;i++ )
      {
        b = font->p[index++];
        color = (((fc & 0xFF) * b + (bc & 0xFF) * (256 - b)) >> 8) & 0xFF | // blue component
        (((fc & 0xFF00) * b + (bc & 0xFF00) * (256 - b)) >> 8) & 0xFF00| // green component
        (((fc & 0xFF0000) * b + (bc & 0xFF0000) * (256 - b)) >> 8) & 0xFF0000; // red component
        // writePixel(xo,yo, bc);
        fillRect(xo,yo,textsize,textsize,color);
        xo+=textsize;;
      }
      index += font->char_width - actual_char_width;
      yo+=textsize;
    }
  }
}

void draw_dot_line(uint16_t column1, uint16_t row1, uint16_t column2, uint16_t row2, uint16_t color)
{
  #define DOT_SIZE 2
  uint16_t t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;
  delta_x=column2-column1;
  delta_y=row2-row1;
  uRow=column1;
  uCol=row1;
  // --
  if(delta_x>0)incx=DOT_SIZE;
  else if(delta_x==0)incx=0;
  else
  {
    incx=-DOT_SIZE;delta_x=-delta_x;
  }
  // --
  if(delta_y>0)incy=DOT_SIZE;
  else if(delta_y==0)incy=0;
  else
  {
    incy=-DOT_SIZE;delta_y=-delta_y;
  }
  // --
  if(delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  for(t=0;t<=distance+1;t+=DOT_SIZE)
  {
    writePixel(uRow,uCol,color);
    xerr+=delta_x ;
    yerr+=delta_y ;
    // --
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    // --
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

