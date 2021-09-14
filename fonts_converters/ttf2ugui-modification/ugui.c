#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wparentheses"
/* -------------------------------------------------------------------------------- */
/* -- ÂµGUI - Generic GUI module (C)Achim DÃ¶bler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// ÂµGUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim DÃ¶bler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The ÂµGUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
/* -------------------------------------------------------------------------------- */
/* -- REVISION HISTORY                                                           -- */
/* -------------------------------------------------------------------------------- */
//  Mar 18, 2015  V0.3  Driver support added.
//                      Window and object support added.
//                      Touch support added.
//                      Fixed some minor bugs.
//
//  Oct 20, 2014  V0.2  Function UG_DrawRoundFrame() added.
//                      Function UG_FillRoundFrame() added.
//                      Function UG_DrawArc() added.
//                      Fixed some minor bugs.
//
//  Oct 11, 2014  V0.1  First release.
/* -------------------------------------------------------------------------------- */
#include "ugui.h"

/* Static functions */
 UG_RESULT _UG_WindowDrawTitle( UG_WINDOW* wnd );
 UG_RESULT _UG_WindowClear( UG_WINDOW* wnd );
 void _UG_TextboxUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);
 void _UG_ButtonUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);
 void _UG_ImageUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);
 void _UG_PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc, const UG_FONT* font);

 /* Pointer to the gui */
static UG_GUI* gui;

UG_S16 UG_Init( UG_GUI* g, void (*p)(UG_S16,UG_S16,UG_COLOR), UG_S16 x, UG_S16 y )
{
   UG_U8 i;

   g->pset = (void(*)(UG_S16,UG_S16,UG_COLOR))p;
   g->x_dim = x;
   g->y_dim = y;
   g->console.x_start = 4;
   g->console.y_start = 4;
   g->console.x_end = g->x_dim - g->console.x_start-1;
   g->console.y_end = g->y_dim - g->console.x_start-1;
   g->console.x_pos = g->console.x_end;
   g->console.y_pos = g->console.y_end;
   g->char_h_space = 1;
   g->char_v_space = 1;
   g->font.p = NULL;
   g->font.char_height = 0;
   g->font.char_width = 0;
   g->font.start_char = 0;
   g->font.end_char = 0;
   g->font.widths = NULL;
   g->desktop_color = 0x5E8BEf;
   g->fore_color = C_WHITE;
   g->back_color = C_BLACK;
   g->next_window = NULL;
   g->active_window = NULL;
   g->last_window = NULL;

   /* Clear drivers */
   for(i=0;i<NUMBER_OF_DRIVERS;i++)
   {
      g->driver[i].driver = NULL;
      g->driver[i].state = 0;
   }

   gui = g;
   return 1;
}

UG_S16 UG_SelectGUI( UG_GUI* g )
{
   gui = g;
   return 1;
}

void UG_FontSelect( const UG_FONT* font )
{
   gui->font = *font;
}

void UG_FillScreen( UG_COLOR c )
{
   UG_FillFrame(0,0,gui->x_dim-1,gui->y_dim-1,c);
}

void UG_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n,m;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   /* Is hardware acceleration available? */
   if ( gui->driver[DRIVER_FILL_FRAME].state & DRIVER_ENABLED )
   {
      if( ((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_FILL_FRAME].driver)(x1,y1,x2,y2,c) == UG_RESULT_OK ) return;
   }

   for( m=y1; m<=y2; m++ )
   {
      for( n=x1; n<=x2; n++ )
      {
         gui->pset(n,m,c);
      }
   }
}

void UG_FillRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
{
   UG_S16  x,y,xd;

   if ( x2 < x1 )
   {
      x = x2;
      x2 = x1;
      x1 = x;
   }
   if ( y2 < y1 )
   {
      y = y2;
      y2 = y1;
      y1 = y;
   }

   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   UG_FillFrame(x1 + r, y1, x2 - r, y2, c);

   while ( x <= y )
   {
     if( y > 0 )
     {
        UG_DrawLine(x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r, c);
        UG_DrawLine(x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r, c);
     }
     if( x > 0 )
     {
        UG_DrawLine(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, c);
        UG_DrawLine(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, c);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
}

void UG_DrawMesh( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n,m;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   for( m=y1; m<=y2; m+=2 )
   {
      for( n=x1; n<=x2; n+=2 )
      {
         gui->pset(n,m,c);
      }
   }
}

void UG_DrawFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_DrawLine(x1,y1,x2,y1,c);
   UG_DrawLine(x1,y2,x2,y2,c);
   UG_DrawLine(x1,y1,x1,y2,c);
   UG_DrawLine(x2,y1,x2,y2,c);
}

void UG_DrawRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
{
   UG_S16 n;
   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   if ( r > x2 ) return;
   if ( r > y2 ) return;

   UG_DrawLine(x1+r, y1, x2-r, y1, c);
   UG_DrawLine(x1+r, y2, x2-r, y2, c);
   UG_DrawLine(x1, y1+r, x1, y2-r, c);
   UG_DrawLine(x2, y1+r, x2, y2-r, c);
   UG_DrawArc(x1+r, y1+r, r, 0x0C, c);
   UG_DrawArc(x2-r, y1+r, r, 0x03, c);
   UG_DrawArc(x1+r, y2-r, r, 0x30, c);
   UG_DrawArc(x2-r, y2-r, r, 0xC0, c);
}

void UG_DrawPixel( UG_S16 x0, UG_S16 y0, UG_COLOR c )
{
   gui->pset(x0,y0,c);
}

void UG_DrawCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
{
   UG_S16 x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      gui->pset(x0 - x, y0 + y, c);
      gui->pset(x0 - x, y0 - y, c);
      gui->pset(x0 + x, y0 + y, c);
      gui->pset(x0 + x, y0 - y, c);
      gui->pset(x0 - y, y0 + x, c);
      gui->pset(x0 - y, y0 - x, c);
      gui->pset(x0 + y, y0 + x, c);
      gui->pset(x0 + y, y0 - x, c);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}

void UG_FillCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
{
   UG_S16  x,y,xd;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   while ( x <= y )
   {
     if( y > 0 )
     {
        UG_DrawLine(x0 - x, y0 - y,x0 - x, y0 + y, c);
        UG_DrawLine(x0 + x, y0 - y,x0 + x, y0 + y, c);
     }
     if( x > 0 )
     {
        UG_DrawLine(x0 - y, y0 - x,x0 - y, y0 + x, c);
        UG_DrawLine(x0 + y, y0 - x,x0 + y, y0 + x, c);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
   UG_DrawCircle(x0, y0, r,c);
}

void UG_DrawArc( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_U8 s, UG_COLOR c )
{
   UG_S16 x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      // Q1
      if ( s & 0x01 ) gui->pset(x0 + x, y0 - y, c);
      if ( s & 0x02 ) gui->pset(x0 + y, y0 - x, c);

      // Q2
      if ( s & 0x04 ) gui->pset(x0 - y, y0 - x, c);
      if ( s & 0x08 ) gui->pset(x0 - x, y0 - y, c);

      // Q3
      if ( s & 0x10 ) gui->pset(x0 - x, y0 + y, c);
      if ( s & 0x20 ) gui->pset(x0 - y, y0 + x, c);

      // Q4
      if ( s & 0x40 ) gui->pset(x0 + y, y0 + x, c);
      if ( s & 0x80 ) gui->pset(x0 + x, y0 + y, c);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}

void UG_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   /* Is hardware acceleration available? */
   if ( gui->driver[DRIVER_DRAW_LINE].state & DRIVER_ENABLED )
   {
      if( ((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_DRAW_LINE].driver)(x1,y1,x2,y2,c) == UG_RESULT_OK ) return;
   }

   dx = x2 - x1;
   dy = y2 - y1;
   dxabs = (dx>0)?dx:-dx;
   dyabs = (dy>0)?dy:-dy;
   sgndx = (dx>0)?1:-1;
   sgndy = (dy>0)?1:-1;
   x = dyabs >> 1;
   y = dxabs >> 1;
   drawx = x1;
   drawy = y1;

   gui->pset(drawx, drawy,c);

   if( dxabs >= dyabs )
   {
      for( n=0; n<dxabs; n++ )
      {
         y += dyabs;
         if( y >= dxabs )
         {
            y -= dxabs;
            drawy += sgndy;
         }
         drawx += sgndx;
         gui->pset(drawx, drawy,c);
      }
   }
   else
   {
      for( n=0; n<dyabs; n++ )
      {
         x += dxabs;
         if( x >= dyabs )
         {
            x -= dyabs;
            drawx += sgndx;
         }
         drawy += sgndy;
         gui->pset(drawx, drawy,c);
      }
   }
}

void UG_PutString( UG_S16 x, UG_S16 y, char* str )
{
   UG_S16 xp,yp;
   UG_U8 cw;
   char chr;

   xp=x;
   yp=y;

   while ( *str != 0 )
   {
      chr = *str++;
	  if (chr < gui->font.start_char || chr > gui->font.end_char) continue;
      if ( chr == '\n' )
      {
         xp = gui->x_dim;
         continue;
      }
	  cw = gui->font.widths ? gui->font.widths[chr - gui->font.start_char] : gui->font.char_width;

      if ( xp + cw > gui->x_dim - 1 )
      {
         xp = x;
         yp += gui->font.char_height+gui->char_v_space;
      }

      UG_PutChar(chr, xp, yp, gui->fore_color, gui->back_color);

      xp += cw + gui->char_h_space;
   }
}

void UG_PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc )
{
	_UG_PutChar(chr,x,y,fc,bc,&gui->font);
}

void UG_ConsolePutString( char* str )
{
   char chr;
   UG_U8 cw;

   while ( *str != 0 )
   {
      chr = *str;
      if ( chr == '\n' )
      {
         gui->console.x_pos = gui->x_dim;
         str++;
         continue;
      }

      cw = gui->font.widths ? gui->font.widths[chr - gui->font.start_char] : gui->font.char_width;
      gui->console.x_pos += cw+gui->char_h_space;

      if ( gui->console.x_pos+cw > gui->console.x_end )
      {
         gui->console.x_pos = gui->console.x_start;
         gui->console.y_pos += gui->font.char_height+gui->char_v_space;
      }
      if ( gui->console.y_pos+gui->font.char_height > gui->console.y_end )
      {
         gui->console.x_pos = gui->console.x_start;
         gui->console.y_pos = gui->console.y_start;
         UG_FillFrame(gui->console.x_start,gui->console.y_start,gui->console.x_end,gui->console.y_end,gui->console.back_color);
      }

      UG_PutChar(chr, gui->console.x_pos, gui->console.y_pos, gui->console.fore_color, gui->console.back_color);
      str++;
   }
}

void UG_ConsoleSetArea( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   gui->console.x_start = xs;
   gui->console.y_start = ys;
   gui->console.x_end = xe;
   gui->console.y_end = ye;
}

void UG_ConsoleSetForecolor( UG_COLOR c )
{
   gui->console.fore_color = c;
}

void UG_ConsoleSetBackcolor( UG_COLOR c )
{
   gui->console.back_color = c;
}

void UG_SetForecolor( UG_COLOR c )
{
   gui->fore_color = c;
}

void UG_SetBackcolor( UG_COLOR c )
{
   gui->back_color = c;
}

UG_S16 UG_GetXDim( void )
{
   return gui->x_dim;
}

UG_S16 UG_GetYDim( void )
{
   return gui->y_dim;
}

void UG_FontSetHSpace( UG_U16 s )
{
   gui->char_h_space = s;
}

void UG_FontSetVSpace( UG_U16 s )
{
   gui->char_v_space = s;
}


/* -------------------------------------------------------------------------------- */
/* -- INTERNAL FUNCTIONS                                                         -- */
/* -------------------------------------------------------------------------------- */
void _UG_PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc, const UG_FONT* font)
{
   UG_U16 i,j,k,xo,yo,c,bn,actual_char_width;
   UG_U8 b,bt;
   UG_U32 index;
   UG_COLOR color;
   void(*push_pixel)(UG_COLOR);

   bt = (UG_U8)chr;

   switch ( bt )
   {
      case 0xF6: bt = 0x94; break; // ö
      case 0xD6: bt = 0x99; break; // Ö
      case 0xFC: bt = 0x81; break; // ü
      case 0xDC: bt = 0x9A; break; // Ü
      case 0xE4: bt = 0x84; break; // ä
      case 0xC4: bt = 0x8E; break; // Ä
      case 0xB5: bt = 0xE6; break; // µ
      case 0xB0: bt = 0xF8; break; // °
   }

   if (bt < font->start_char || bt > font->end_char) return;
   
   yo = y;
   bn = font->char_width;
   if ( !bn ) return;
   bn >>= 3;
   if ( font->char_width % 8 ) bn++;
   actual_char_width = (font->widths ? font->widths[bt - font->start_char] : font->char_width);

   /* Is hardware acceleration available? */
   if ( gui->driver[DRIVER_FILL_AREA].state & DRIVER_ENABLED )
   {
	   //(void(*)(UG_COLOR))
      push_pixel = ((void*(*)(UG_S16, UG_S16, UG_S16, UG_S16))gui->driver[DRIVER_FILL_AREA].driver)(x,y,x+actual_char_width-1,y+font->char_height-1);
	   
      if (font->font_type == FONT_TYPE_1BPP)
	  {
	      index = (bt - font->start_char)* font->char_height * bn;
		  for( j=0;j<font->char_height;j++ )
		  {
			 c=actual_char_width;
			 for( i=0;i<bn;i++ )
			 {
				b = font->p[index++];
				for( k=0;(k<8) && c;k++ )
				{
				   if( b & 0x01 )
				   {
					  push_pixel(fc);
				   }
				   else
				   {
					  push_pixel(bc);
				   }
				   b >>= 1;
				   c--;
				}
			 }
	 	 }
	  }
	  else if (font->font_type == FONT_TYPE_8BPP)
	  {
		   index = (bt - font->start_char)* font->char_height * font->char_width;
		   for( j=0;j<font->char_height;j++ )
		   {
			  for( i=0;i<actual_char_width;i++ )
			  {
				 b = font->p[index++];
				 color = (((fc & 0xFF) * b + (bc & 0xFF) * (256 - b)) >> 8) & 0xFF |//Blue component
				         (((fc & 0xFF00) * b + (bc & 0xFF00) * (256 - b)) >> 8)  & 0xFF00|//Green component
				         (((fc & 0xFF0000) * b + (bc & 0xFF0000) * (256 - b)) >> 8) & 0xFF0000; //Red component
				 push_pixel(color);
			  }
			  index += font->char_width - actual_char_width;
		  }
	  }
   }
   else
   {
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
                  gui->pset(xo,yo,fc);
               }
               else
               {
                  gui->pset(xo,yo,bc);
               }
               b >>= 1;
               xo++;
               c--;
             }
           }
           yo++;
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
               color = (((fc & 0xFF) * b + (bc & 0xFF) * (256 - b)) >> 8) & 0xFF |//Blue component
                       (((fc & 0xFF00) * b + (bc & 0xFF00) * (256 - b)) >> 8)  & 0xFF00|//Green component
                       (((fc & 0xFF0000) * b + (bc & 0xFF0000) * (256 - b)) >> 8) & 0xFF0000; //Red component
               gui->pset(xo,yo,color);
               xo++;
            }
            index += font->char_width - actual_char_width;
            yo++;
         }
      }
   }
}

void _UG_PutText(UG_TEXT* txt)
{
   UG_U16 sl,rc,wl;
   UG_S16 xp,yp;
   UG_S16 xs=txt->a.xs;
   UG_S16 ys=txt->a.ys;
   UG_S16 xe=txt->a.xe;
   UG_S16 ye=txt->a.ye;
   UG_U8  align=txt->align;
   UG_S16 char_width=txt->font->char_width;
   UG_S16 char_height=txt->font->char_height;
   UG_S16 char_h_space=txt->h_space;
   UG_S16 char_v_space=txt->v_space;
   UG_U16 i,j,k,xo,yo,cw,bn;
   UG_U8  b,bt;
   UG_U32 index;
   char chr;

   char* str = txt->str;
   char* c = str;

   if ( txt->font->p == NULL ) return;
   if ( str == NULL ) return;
   if ( (ye - ys) < txt->font->char_height ) return;

   rc=1;
   c=str;
   while ( *c != 0 )
   {
      if ( *c == '\n' ) rc++;
      c++;
   }

   yp = 0;
   if ( align & (ALIGN_V_CENTER | ALIGN_V_BOTTOM) )
   {
      yp = ye - ys + 1;
      yp -= char_height*rc;
      yp -= char_v_space*(rc-1);
      if ( yp < 0 ) return;
   }
   if ( align & ALIGN_V_CENTER ) yp >>= 1;
   yp += ys;

   while( 1 )
   {
      sl=0;
      c=str;
      wl = 0;
      while( (*c != 0) && (*c != '\n') )
      {
         if (*c < txt->font->start_char || *c > txt->font->end_char) {c++; continue;}
         sl++;
         wl += (txt->font->widths ? txt->font->widths[*c - txt->font->start_char] : char_width) + char_h_space;
         c++;
      }
      wl -= char_h_space;

      xp = xe - xs + 1;
      xp -= wl;
      if ( xp < 0 ) return;

      if ( align & ALIGN_H_LEFT ) xp = 0;
      else if ( align & ALIGN_H_CENTER ) xp >>= 1;
      xp += xs;

      while( (*str != '\n') )
      {
         chr = *str++;
         if ( chr == 0 ) return;
         _UG_PutChar(chr,xp,yp,txt->fc,txt->bc,txt->font);
         xp += (txt->font->widths ? txt->font->widths[chr - txt->font->start_char] : char_width) + char_h_space;
      }
      str++;
      yp += char_height + char_v_space;
   }
}

#ifdef USE_POSTRENDER_EVENT
void _UG_SendObjectPostrenderEvent(UG_WINDOW *wnd,UG_OBJECT *obj)
{
	UG_MESSAGE msg;
	msg.event = OBJ_EVENT_POSTRENDER;
	msg.type = MSG_TYPE_OBJECT;
	msg.id = obj->type;
	msg.sub_id = obj->id;
	msg.src = obj;

	wnd->cb(&msg);
}
#endif

/* -------------------------------------------------------------------------------- */
/* -- MISCELLANEOUS FUNCTIONS                                                    -- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- WINDOW FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- BUTTON FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- TEXTBOX FUNCTIONS                                                          -- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- IMAGE FUNCTIONS                                                            -- */
/* -------------------------------------------------------------------------------- */

