//--------------------------------------------------------------
// File     : stm32_ub_vga_screen.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_SCREEN_H
#define __STM32F4_UB_VGA_SCREEN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_dma.h"



//--------------------------------------------------------------
// Standard Farbwerte
// 8bit Farbwert (R3G3B2)
// Red   (3bit) -> Bit7-Bit5
// Green (3bit) -> Bit4-Bit2
// Blue  (2bit) -> Bit1-Bit0
//--------------------------------------------------------------
#define  VGA_COL_BLACK          0x00
#define  VGA_COL_BLUE           0x03
#define  VGA_COL_GREEN          0x1C
#define  VGA_COL_RED            0xE0
#define  VGA_COL_WHITE          0xFF

#define  VGA_COL_CYAN           0x1F
#define  VGA_COL_MAGENTA        0xE3
#define  VGA_COL_YELLOW         0xFC



//--------------------------------------------------------------
// Defines f�r die Aufl�sungen :
// der VGA-Monitor wird im Mode 640x480 betrieben
// das angezeigte Bild besteht aber nur aus 320x240 Pixel
// jedes Pixel wird als 2x2 Punkt am Monitor angezeigt
//--------------------------------------------------------------
#define VGA_DISPLAY_X   320  // Anzahl der Pixel in X-Richtung
#define VGA_DISPLAY_Y   240  // Anzahl der Pixel in Y-Richtung



//--------------------------------------------------------------
// VGA Struktur
//--------------------------------------------------------------
typedef struct {
  uint16_t hsync_cnt;   // Zeilenz�hler
  uint32_t start_adr;   // start_adresse
  uint32_t dma2_cr_reg; // Inhalt vom CR-Register
}VGA_t;

extern VGA_t VGA;

//--------------------------------------------------------------
// Display RAM
// pro Zeile ein Byte mehr f�r das Endbyte
// das ist Wichtig um nach der Zeile auf "schwarz" zu schalten
//--------------------------------------------------------------
extern uint8_t VGA_RAM1[];



//--------------------------------------------------------------
// Timer-1
// Funktion  = Pixelclock (Takt f�r den DMA Transver)
//
// Grundfreq = 2*APB2 (APB2=84MHz) => TIM_CLK=168MHz
// Frq       = 168MHz/1/12 = 14MHz
//
// mit diesem Wert kann die "breite" des angezeigten Bildes 
// beeinflusst werden
//--------------------------------------------------------------
#define VGA_TIM1_PERIODE      11
#define VGA_TIM1_PRESCALE      0



//--------------------------------------------------------------
// Timer-2
// Funktion  = CH4 : HSync-Signal erzeugen an PB11
//             CH3 : Triggerpunkt f�r DMA start
//
// Grundfreq = 2*APB1 (APB1=48MHz) => TIM_CLK=84MHz
// Frq       = 84MHz/1/2668 = 31,48kHz => T = 31,76us
// 1TIC      = 11,90ns
//
// der Wert vom DMA-Delay muss ausprobiert werden
// und ist auch abh�ngig von der Optimierungsstufe
//   gr�ssere Zahlen verschieben das Bild nach links
//   kleinere Zahlen verschieben das Bild nach rechts
//--------------------------------------------------------------
#define  VGA_TIM2_HSYNC_PERIODE   2667
#define  VGA_TIM2_HSYNC_PRESCALE     0

#define  VGA_TIM2_HSYNC_IMP       320  // HSync-Impulsl�nge (3,81us)
#define  VGA_TIM2_HTRIGGER_START  480  // HSync+BackPorch (5,71us)
#define  VGA_TIM2_DMA_DELAY        60  // gleicht das Delay vom DMA START aus (Optimization = none)
//#define  VGA_TIM2_DMA_DELAY        30  // gleicht das Delay vom DMA START aus (Optimization = -O1)


//--------------------------------------------------------------
// VSync-Signal
// Trigger   = Timer2 Update (f=31,48kHz => T = 31,76us)
// 1TIC      = 31,76us
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE        525   // Anzahl der Zeilen pro Bild

#define  VGA_VSYNC_IMP  2               // VSync-Impulsl�nge (63,64us)
#define  VGA_VSYNC_BILD_START      36   // VSync+BackPorch (1,11ms)
#define  VGA_VSYNC_BILD_STOP      514   // VSync+BackPorch+Bildl�nge (16,38ms)





//--------------------------------------------------------------
// Adressen vom PORTE (Register ODR) um per DMA darauf zuzugreifen
// (siehe Seite 53+204 vom Referenz Manual)
//
// Hinweis : weil PE0 und PE1 auf dem Discovery-Board belegt sind,
// werden die 8Bit-Daten an PE8 bis PE15 ausgegeben
// (und deswegen um 8bit nach links geschoben = OFFSET)
// Data-Bit0 => PE8
// Data-Bit7 => PE15
//--------------------------------------------------------------
#define VGA_GPIOE_BASE_ADR     ((uint32_t)0x40021000) // ADR von Port-E
#define VGA_GPIO_ODR_OFFSET    ((uint32_t)0x00000014) // ADR von Register ODR
#define VGA_GPIO_BYTE_OFFSET   ((uint32_t)0x00000001) // Daten um 8bit verschieben

#define VGA_GPIOE_ODR_ADDRESS   (VGA_GPIOE_BASE_ADR | VGA_GPIO_ODR_OFFSET | VGA_GPIO_BYTE_OFFSET)





//--------------------------------------------------------------
// Define um die Farbe "schwarz" an PE8 bis PE15 auszugeben
//--------------------------------------------------------------
#define VGA_GPIO_HINIBBLE  ((uint16_t)0xFF00) // GPIO_Pin_8 bis GPIO_Pin_15





//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_VGA_Screen_Init(void);
void UB_VGA_FillScreen(uint8_t color);
void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color);





//--------------------------------------------------------------
#endif // __STM32F4_UB_VGA_SCREEN_H
