//--------------------------------------------------------------
// File     : stm32_ub_lcd_vga.c
// Datum    : 30.11.2014
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, LTDC, STM32_UB_SDRAM
// Funktion : Grafik-LCD Funktionen im VGA-Mode
//            Der Zugriff erfolgt ueber den TFT-Controller
//               VGA-Mode = 640x480 Pixel (60Hz)
//            Screen-Auflösung einstellbar im H-File
//               Mode-0=640x480 Pixel
//               Mode-1=320x240 Pixel
//               Mode-2=240x320 Pixel
//            Farbmode = 16bit (5R6G5B = RGB565)
//
// Hinweis  : VGA-Pins :
//              R7=PG6    G7=PD3    B7=PB9
//              R6=PB1    G6=PC7    B6=PB8
//              R5=PA12   G5=PB11   B5=PA3
//              R4=PA11   G4=PB10   B4=PG12
//              R3=PB0    G3=PG10   B3=PG11
//              R2=PC10   G2=PA6    B2=PD6
//
//              HSync=PC6,  VSync=PA4
//
// Das externe SDRAM wird als Grafik-RAM benutzt
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_lcd_vga.h"


LCD_MODE_t  LCD_DISPLAY_MODE;
uint32_t LCD_CurrentFrameBuffer;   // aktuelle Adresse zum zeichnen
uint32_t LCD_CurrentLayer;         // 0=Hintergrund, 1=Vodergrund
uint32_t LCD_CurrentOrientation;   // 0=0Grad, 1=180Grad

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_VGA_InitLTCD(void);
void P_VGA_AFConfig(void);

//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
static uint16_t aktCursorX,aktCursorY;
static uint32_t aktCursorPos;


//--------------------------------------------------------------
// Init vom LCD-Display (im VGA-Mode)
//--------------------------------------------------------------
void UB_LCD_InitVGA(void)
{
  // init vom SDRAM
  UB_SDRAM_Init();
  // init vom LTDC
  P_VGA_InitLTCD();
  // beim init auf Portrait-Mode
  LCD_DISPLAY_MODE=PORTRAIT;


  aktCursorX=0;
  aktCursorY=0;
  aktCursorPos=0;

  LCD_CurrentFrameBuffer=LCD_FRAME_BUFFER;
  LCD_CurrentLayer = 0;
  LCD_CurrentOrientation=0;
}


//--------------------------------------------------------------
// stellt beide Layer auf "Fullscreen-Mode"
//--------------------------------------------------------------
void UB_LCD_LayerInit_Fullscreen(void)
{
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

  #if VGA_DIRECTION==0
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 144 + VGA_OFFSET_H;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_MAXY + 144 - 1 + VGA_OFFSET_H);
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 35 + VGA_OFFSET_V;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_MAXX + 35 - 1 + VGA_OFFSET_V);

  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_MAXY * 2) + 3);
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_MAXY * 2);
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_MAXX;
  #else
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 144 + VGA_OFFSET_H;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_MAXX + 144 - 1 + VGA_OFFSET_H);
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 35 + VGA_OFFSET_V;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_MAXY + 35 - 1 + VGA_OFFSET_V);

  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_MAXX * 2) + 3);
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_MAXX * 2);
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_MAXY;
  #endif

  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;

  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + LCD_FRAME_OFFSET;

  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  LTDC_ReloadConfig(LTDC_IMReload);

  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  LTDC_ReloadConfig(LTDC_IMReload);

  LTDC_DitherCmd(ENABLE);

  LTDC_Cmd(ENABLE);
}


//--------------------------------------------------------------
// Hintergrund-Layer aktivieren
//--------------------------------------------------------------
void UB_LCD_SetLayer_1(void)
{
  LCD_CurrentFrameBuffer = LCD_FRAME_BUFFER;
  LCD_CurrentLayer = 0;
}


//--------------------------------------------------------------
// Vordergrund-Layer aktivieren
//--------------------------------------------------------------
void UB_LCD_SetLayer_2(void)
{
  LCD_CurrentFrameBuffer = LCD_FRAME_BUFFER + LCD_FRAME_OFFSET;
  LCD_CurrentLayer = 1;
}


//--------------------------------------------------------------
// Füllt den aktuellen Layer komplett mit einer Farbe
//--------------------------------------------------------------
void UB_LCD_FillLayer(uint16_t color)
{
  uint32_t index = 0;

  // Bildschirm loeschen
  for (index = 0x00; index < LCD_FRAME_OFFSET; index+=2) {
    *(volatile uint16_t*)(LCD_CurrentFrameBuffer + index) = color;
  }
}


//--------------------------------------------------------------
// setzt Transparenz Wert vom aktuellen Layer
// wert : [0...255] 0=durchsichtig ... 255=solid
//--------------------------------------------------------------
void UB_LCD_SetTransparency(uint8_t wert)
{
  if (LCD_CurrentLayer == 0) {
    LTDC_LayerAlpha(LTDC_Layer1, wert);
  }
  else {
    LTDC_LayerAlpha(LTDC_Layer2, wert);
  }
  LTDC_ReloadConfig(LTDC_IMReload);
}

//--------------------------------------------------------------
// setzt den Cursor auf x,y Position
//--------------------------------------------------------------
void UB_LCD_SetCursor2Draw(uint16_t xpos, uint16_t ypos)
{
  aktCursorX=xpos;
  aktCursorY=ypos;

  #if VGA_DIRECTION==0
  aktCursorPos=LCD_CurrentFrameBuffer+(2*(((LCD_MAXX-aktCursorX-1)*LCD_MAXY)+aktCursorY));
  #else
  aktCursorPos=LCD_CurrentFrameBuffer+(2*((aktCursorY*LCD_MAXX)+aktCursorX));
  #endif
}


//--------------------------------------------------------------
// zeichnet ein Pixel an aktueller XY-Position
// und incrementiert Cursor
//--------------------------------------------------------------
void UB_LCD_DrawPixel(uint16_t color)
{
  *(volatile uint16_t*)(aktCursorPos)=color;
  if(LCD_DISPLAY_MODE==PORTRAIT) {
    aktCursorX++;
    if(aktCursorX>=LCD_MAXX) {
      aktCursorX=0;
      aktCursorY++;
      if(aktCursorY>=LCD_MAXY) aktCursorY=0;
    }
  }
  else {
    aktCursorY++;
    if(aktCursorY>=LCD_MAXY) {
      aktCursorY=0;
      aktCursorX++;
      if(aktCursorX>=LCD_MAXX) aktCursorX=0;
    }
  }
  #if VGA_DIRECTION==0
  aktCursorPos=LCD_CurrentFrameBuffer+(2*(((LCD_MAXX-aktCursorX-1)*LCD_MAXY)+aktCursorY));
  #else
  aktCursorPos=LCD_CurrentFrameBuffer+(2*((aktCursorY*LCD_MAXX)+aktCursorX));
  #endif
}


//--------------------------------------------------------------
// Screen-Mode einstellen
// muss direkt nach dem Init gemacht werden
//
// Mode : [PORTRAIT=default, LANDSCAPE]
//--------------------------------------------------------------
void UB_LCD_SetMode(LCD_MODE_t mode)
{
  if(mode==PORTRAIT) {
    LCD_DISPLAY_MODE=PORTRAIT;
  }
  else {
    LCD_DISPLAY_MODE=LANDSCAPE;
  }
}

//--------------------------------------------------------------
// kopiert kompletten Inhalt von Layer1 in Layer2
// (Hintergrund --> Vordergrund)
//--------------------------------------------------------------
void UB_LCD_Copy_Layer1_to_Layer2(void)
{
  uint32_t index;
  uint32_t quelle = LCD_FRAME_BUFFER;
  uint32_t ziel = LCD_FRAME_BUFFER + LCD_FRAME_OFFSET;

  for (index = 0 ; index < LCD_FRAME_OFFSET;index+=2) {
    *(volatile uint16_t*)(ziel + index) = *(volatile uint16_t*)(quelle + index);
  }
}


//--------------------------------------------------------------
// kopiert kompletten Inhalt von Layer2 in Layer1
// (Vordergrund --> Hintergrund)
//--------------------------------------------------------------
void UB_LCD_Copy_Layer2_to_Layer1(void)
{
  uint32_t index;
  uint32_t quelle = LCD_FRAME_BUFFER + LCD_FRAME_OFFSET;
  uint32_t ziel = LCD_FRAME_BUFFER;

  for (index = 0 ; index < LCD_FRAME_OFFSET;index+=2) {
    *(volatile uint16_t*)(ziel + index) = *(volatile uint16_t*)(quelle + index);
  }
}

//--------------------------------------------------------------
// wechselt den aktiven Layer zum zeichnen
// und zeigt den jeweils anderen Layer an
//--------------------------------------------------------------
void UB_LCD_Refresh(void)
{
  if(LCD_CurrentLayer==0) {
    UB_LCD_SetLayer_2();
    UB_LCD_SetTransparency(0);
  }
  else {
    UB_LCD_SetTransparency(255);
    UB_LCD_SetLayer_1();
  }
}


//--------------------------------------------------------------
// interne Funktion
// Init vom LTCD fuer VGA-Mode
//--------------------------------------------------------------
void P_VGA_InitLTCD(void)
{
  LTDC_InitTypeDef       LTDC_InitStruct;

  // Clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

  P_VGA_AFConfig();

  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

  //---------------------------------------
  // PLLSAI einstellen (auf 25MHz)
  //---------------------------------------
  RCC_PLLSAIConfig(403, 7, 2);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

  RCC_PLLSAICmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);

  //---------------------------------------
  // Timer konfig
  // HSync = 96       VSync = 2
  // HBP   = 48       VBP   = 33
  // HFP   = 16       VFP   = 10
  //
  // Landscape  : W=640, H=480
  //---------------------------------------
  LTDC_InitStruct.LTDC_HorizontalSync = 96;      // HSync
  LTDC_InitStruct.LTDC_VerticalSync = 2;         // VSync
  LTDC_InitStruct.LTDC_AccumulatedHBP = 144;     // HSync+HBP
  LTDC_InitStruct.LTDC_AccumulatedVBP = 35;      // VSync+VBP
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 784; // HSync+HBP+W
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 515; // VSync+VBP+H
  LTDC_InitStruct.LTDC_TotalWidth = 800;         // HSync+HBP+W+HFP
  LTDC_InitStruct.LTDC_TotalHeigh = 525;         // VSync+VBP+H+VFP

  LTDC_Init(&LTDC_InitStruct);
}



//--------------------------------------------------------------
// interne Funktion
// alle AF-Funktionen
//--------------------------------------------------------------
void P_VGA_AFConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                         RCC_AHB1Periph_GPIOG, ENABLE);



  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                             GPIO_Pin_11 | GPIO_Pin_12;

  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, 0x09);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                             GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;

  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;

  GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, 0x09);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_10 | \
                             GPIO_Pin_11 | GPIO_Pin_12;

  GPIO_Init(GPIOG, &GPIO_InitStruct);

}

