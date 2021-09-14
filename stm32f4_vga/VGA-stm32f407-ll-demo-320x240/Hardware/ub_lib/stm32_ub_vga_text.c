//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_text.h"
#include "stm32_adafruit_vga.h"
#include "fonts.h"
#include "bitmap.h"
#include "string.h"
#include "tft_font.h"

uint8_t Draw_Bitmap(uint8_t nr,uint16_t xp, uint16_t yp)
{
	uint8_t error = 0;
	uint8_t x, y;
	uint8_t *image;

	//check for out of bounds
	if(xp < 0 || yp < 0 || xp > UB_VGA_Get_X() || yp > UB_VGA_Get_Y())
		error = 1;

	//checks for unknown bitmap
	if(nr < 0 || nr > 5)
		error = 5;

	switch (nr) {
	case 0:
		image = &pijl_rechts[0];
		break;
	case 1:
		image = &pijl_omhoog[0];
		break;
	case 2:
		image = &pijl_links[0];
		break;
	case 3:
		image = &pijl_omlaag[0];
		break;
	case 4:
		image = &smiley_boos[0];
		break;
	case 5:
		image = &smiley_blij[0];
		break;
	default:
		break;
	}

	for (y = 0; y < 48; y++) {
		for (x = 0; x < 48; x++) {
			UB_VGA_SetPixel(x+xp, y+yp, *(image)++);
		}
	}
	return error;
}

void tft_putchar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t f_color, uint8_t b_color)
{
  if(x < 0 || y < 0 || x > UB_VGA_Get_X() || y > UB_VGA_Get_Y()) return;
  if(num<32) return;
  uint8_t temp;
  uint8_t pos,t;
  uint8_t size_x = size;

  num=num-' ';

  for(pos=0;pos<16*size;pos++)
  {
    temp=asc2_1608[(uint16_t)num*16+(pos/size)];
    for(t=0;t<8*size;t++)
    {
      if(temp & 0x01) UB_VGA_SetPixel(x+t,y+pos,f_color);
      else  UB_VGA_SetPixel(x+t,y+pos,b_color);
      if(!size_x)
      {
       temp>>=1;
       size_x = size;
      }
      size_x--;
    }
  }
}

void tft_putchar14x24(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color)
{
  if(x < 0 || y < 0 || x > UB_VGA_Get_X() || y > UB_VGA_Get_Y()) return;
  if(data<32) return;

  #define divider 1
  uint8_t i,j,k,temp;
  for(i=0;i<24/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<14;k++)
      {
        temp=Consolas14x24[(data-' ')*(24/8)*14+k*(24/8)+i];
        if(temp&(0x01<<j))
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,f_color);
        else 
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,b_color);
      }
    }
  }
}

void tft_putchar18x32(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color)
{
  if((x>UB_VGA_Get_X()-18)||(y>UB_VGA_Get_Y()-32)) return;
  if(x < 0 || y < 0 || x > UB_VGA_Get_X() || y > UB_VGA_Get_Y()) return;
  if(data<32) return;

  #define divider 2
  uint8_t i,j,k,temp;
  for(i=0;i<32/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<18;k++)
      {
        temp=Consolas18x32[(data-' ')*(32/8)*18+k*(32/8)+i];
        if(temp&(0x01<<j))
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,f_color);
        else 
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,b_color);
      }
    }
  }
}

void tft_putchar26x48(uint16_t x, uint16_t y, uint8_t data, uint8_t f_color, uint8_t b_color)
{
  if((x>UB_VGA_Get_X()-26)||(y>UB_VGA_Get_Y()-48)) return;
  if(x < 0 || y < 0 || x > UB_VGA_Get_X() || y > UB_VGA_Get_Y()) return;
  if(data<32) return;

  #define divider 2
  uint8_t i,j,k,temp;
  for(i=0;i<48/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<26;k++)
      {
        temp=Consolas26x48[(data-' ')*(48/8)*26+k*(48/8)+i];
        if(temp&(0x01<<j))
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,f_color);
        else 
          UB_VGA_SetPixel(x+k/divider,y+(8*i+j)/divider,b_color);
      }
    }
  }
}

void tft_puts26x48(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color)
{
  #define divider 2
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar26x48(row, column, *(string+i), f_color, b_color);
    row+=26/divider;
    i++;
  }
}

void tft_puts18x32(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color)
{
  #define divider 2
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar18x32(row, column, *(string+i), f_color, b_color);
    row+=18/divider;
    i++;
  }
}

void tft_puts14x24(uint16_t row, uint16_t column, int8_t *string, uint8_t f_color, uint8_t b_color)
{
  #define divider 1
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar14x24(row, column, *(string+i), f_color, b_color);
    row+=14/divider;
    i++;
  }
}

void tft_puts8x16(uint16_t row, uint16_t column, int8_t *string,  uint8_t size, uint8_t f_color, uint8_t b_color)
{
  if(size<1) size = 1;

  uint8_t i=0;
  
  while(*(string+i)!='\0')
  {
    if(column> UB_VGA_Get_X()-16)
    {
      column=0;row+=16;
    }
    if(row> UB_VGA_Get_Y()-16)
    {
      row=column=0;
    }

    tft_putchar(row,column,*(string+i), size, f_color, b_color);
    row+=8*size;
    i++;
  }
}

