//--------------------------------------------------------------
// File     : stm32_ub_vga_320x240.c
// Datum    : 26.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, TIM, MISC, DMA
// Funktion : VGA Ausgabe per GPIO (320x240 Pixel, 8bit Farbe)
//
// Hinweis  : PB11      = HSync-Signal
//            PB12      = VSync-Signal
//            PE8+PE9   = Farbe BLAU
//            PE10-PE12 = Farbe GR�N
//            PE13-PE15 = Farbe ROT
//
// Benutzt  : TIM1, TIM2
//            DMA2, Channel6, Stream5
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_screen.h"
#include "stdint.h"
#include "stdlib.h"


VGA_t VGA;

//--------------------------------------------------------------
// Display RAM
//--------------------------------------------------------------
uint8_t VGA_RAM1[(VGA_DISPLAY_X+1)*VGA_DISPLAY_Y];

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_VGA_InitIO(void);
void P_VGA_InitTIM(void);
void P_VGA_InitINT(void);
void P_VGA_InitDMA(void);





//--------------------------------------------------------------
// Init vom VGA-Modul
//--------------------------------------------------------------
void UB_VGA_Screen_Init(void)
{
  uint16_t xp,yp;

  // init aller Variabeln
  VGA.hsync_cnt=0;
  VGA.start_adr=0;
  VGA.dma2_cr_reg=0;

  // RAM loeschen mit schwarz (komplett)
  for(yp=0;yp<VGA_DISPLAY_Y;yp++) {
    for(xp=0;xp<(VGA_DISPLAY_X+1);xp++) {
      VGA_RAM1[(yp*(VGA_DISPLAY_X+1))+xp]=0;
    }
  }

  // init der IO-Pins
  P_VGA_InitIO();
  // init der Timer
  P_VGA_InitTIM();
  // init vom DMA
  P_VGA_InitDMA();
  // init der Interrupts
  P_VGA_InitINT();

  //-----------------------
  // Register sichern
  //-----------------------
  // Inhalt vom CR-Register auslesen und sichern
  VGA.dma2_cr_reg=DMA2_Stream5->CR;
}


//--------------------------------------------------------------
// F�llt den Screen mit einer Farbe
//--------------------------------------------------------------
void UB_VGA_FillScreen(uint8_t color)
{
  uint16_t xp,yp;

  for(yp=0;yp<VGA_DISPLAY_Y;yp++) {
    for(xp=0;xp<VGA_DISPLAY_X;xp++) {
      UB_VGA_SetPixel(xp,yp,color);
    }
  }
}


//--------------------------------------------------------------
// Setzt ein Pixel auf dem Screen mit einer Farbe
// Wichtig : das letzte Pixel+1 jeder Zeile MUSS "schwarz" bleiben
//--------------------------------------------------------------
void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp>=VGA_DISPLAY_X) xp=0;
  if(yp>=VGA_DISPLAY_Y) yp=0;

  // Pixel ins RAM1 schreiben
  VGA_RAM1[(yp*(VGA_DISPLAY_X+1))+xp]=color;
}


//--------------------------------------------------------------
// interne Funktionen
// init aller IO-Pins
//--------------------------------------------------------------
void P_VGA_InitIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;


  //---------------------------------------------
  // init der RGB-Pins (PE8 bis PE15)
  // als normale GPIOs
  //---------------------------------------------
 
  // Clock Enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  // Config als Digital-Ausgang
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
        GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // Startpegel (Lo an allen Pins)
  GPIOE->BSRRH = VGA_GPIO_HINIBBLE;


  //---------------------------------------------
  // init vom H-Sync Pin (PB11)
  // per Timer2 und CH4
  //---------------------------------------------

  // Clock Enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  // Config des Pins als Digital-Ausgang
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Alternative-Funktion mit dem IO-Pin verbinden
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);


  //---------------------------------------------
  // init vom V-Sync Pin (PB12)
  // als normaler GPIO
  //---------------------------------------------

  // Clock Enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  // Config des Pins als Digital-Ausgang
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Startpegel (Hi)
  GPIOB->BSRRL = GPIO_Pin_12;
}


//--------------------------------------------------------------
// interne Funktionen
// init aller Timer
//--------------------------------------------------------------
void P_VGA_InitTIM(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;


  //---------------------------------------------
  // init vom Timer1
  // f�r Pixeldata per DMA
  //---------------------------------------------

  // Clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  // Timer1 init
  TIM_TimeBaseStructure.TIM_Period =  VGA_TIM1_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = VGA_TIM1_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


  //---------------------------------------------
  // init vom Timer2  
  // CH4 f�r HSYNC-Signal
  // CH3 f�r DMA Trigger start
  //---------------------------------------------

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // Timer2 init
  TIM_TimeBaseStructure.TIM_Period = VGA_TIM2_HSYNC_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = VGA_TIM2_HSYNC_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  // Timer2 Channel 3 (f�r DMA Trigger start)
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = VGA_TIM2_HTRIGGER_START-VGA_TIM2_DMA_DELAY;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  // Timer2 Channel 4 (f�r HSYNC)
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = VGA_TIM2_HSYNC_IMP;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);


  //---------------------------------------------
  // alle Timer enable
  //---------------------------------------------

  // Timer1 enable
  TIM_ARRPreloadConfig(TIM1, ENABLE);

  // Timer2 enable
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE);

}

//--------------------------------------------------------------
// interne Funktionen
// init aller Interrupts
//--------------------------------------------------------------
void P_VGA_InitINT(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  //---------------------------------------------
  // init vom DMA Interrupt
  // f�r TransferComplete Interrupt
  // DMA2, Stream5, Channel6
  //---------------------------------------------

  DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  //---------------------------------------------
  // init vom Timer2 Interrupt
  // f�r HSync-Counter per Update
  // f�r DMA Trigger START per CH3  
  //---------------------------------------------

  TIM_ITConfig(TIM2,TIM_IT_CC3,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//--------------------------------------------------------------
// interne Funktionen
// init vom DMA
//--------------------------------------------------------------
void P_VGA_InitDMA(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  //---------------------------------------------
  // DMA per Timer1 Update
  // (siehe Seite 217 vom Referenz Manual)
  // DMA=2, Channel=6, Stream=5
  //---------------------------------------------

  // Clock Enable (DMA)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  // DMA init (DMA2, Channel6, Stream5)
  DMA_Cmd(DMA2_Stream5, DISABLE);
  DMA_DeInit(DMA2_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)VGA_GPIOE_ODR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&VGA_RAM1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = VGA_DISPLAY_X+1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);

  // DMA-Timer1 enable
  TIM_DMACmd(TIM1,TIM_DMA_Update,ENABLE);
}



//--------------------------------------------------------------
// Interrupt
// wird bei Timer2 Interrupt aufgerufen
//   Bei CC3-Interrupt    -> starten vom DMA
//
// VORSICHT !! aus Performance Gr�nden wird das Interrupt-Flag
//             nicht abgefragt (es gibt nur eine INT-Quelle
//--------------------------------------------------------------
void TIM2_IRQHandler(void)
{

  // Interrupt von Timer2 CH3 ist aufgetreten (f�r Trigger start)
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);

  VGA.hsync_cnt++;
  if(VGA.hsync_cnt>=VGA_VSYNC_PERIODE) {
    // Alle Zeilen wurden angezeigt
    VGA.hsync_cnt=0;
    // Adresspointer auf erste Zeile
    VGA.start_adr=(uint32_t)(&VGA_RAM1[0]);
  }

  // HSync-Pegel erzeugen
  if(VGA.hsync_cnt<VGA_VSYNC_IMP) {
    // HSync-Pegel auf Lo
    GPIOB->BSRRH = GPIO_Pin_12;
  }
  else {
    // HSync-Pegel auf Hi
    GPIOB->BSRRL = GPIO_Pin_12;
  }

  // Test ob DMA gestartet werden muss
  if((VGA.hsync_cnt>=VGA_VSYNC_BILD_START) && (VGA.hsync_cnt<=VGA_VSYNC_BILD_STOP)) {
    // nur wenn Zeile im Bild sichtbar => DMA Transfer starten

    // DMA2 init
	DMA2_Stream5->CR=VGA.dma2_cr_reg;
    // Adresse setzen
    DMA2_Stream5->M0AR=VGA.start_adr;
    // Timer1 start
    TIM1->CR1|=TIM_CR1_CEN;
    // DMA2 enable
    DMA2_Stream5->CR|=DMA_SxCR_EN;

    // Test ob Adresspointer hochgesetzt werden muss
    if((VGA.hsync_cnt & 0x01)!=0) {
      // nur bei jeder Zweiten Zeile hochsetzen
      VGA.start_adr+=(VGA_DISPLAY_X+1);
    }
  }
}

//--------------------------------------------------------------
// Interrupt
// wird bei DMA Interrupt aufgerufen
//   Bei TransferCompleteInterrupt -> stoppen vom DMA
//
// BUG : das umschalten auf "schwarz" dauert hier zu lange
// FIX : aus dem Grund ist jede Zeile 1 Pixel l�nger als n�tig
//       und dieses letzte Pixel hat immer die Farbe "schwarz"
//--------------------------------------------------------------
void DMA2_Stream5_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
  {
    // TransferInterruptComplete Interrupt von DMA2 ist aufgetreten
    DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
    // wenn alle Pixel einer Zeile ausgegeben wurden => DMA Transfer stoppen

    // Timer1 stop
    TIM1->CR1&=~TIM_CR1_CEN;
    // DMA2 disable
    DMA2_Stream5->CR=0;
    // auf "schwarz" schalten
    GPIOE->BSRRH = VGA_GPIO_HINIBBLE;
  }
}


