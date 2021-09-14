/* USER CODE BEGIN Header */

/*** iso-8859-5 code for support cyrilic symbols ***/

/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  *
  ****************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "rng.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "fonts_all_includes.h"
#include "vga_adafruit.h"

//stdc
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include "icons.h"
#include "Red_Button.h"
#include "Green_button.h"
#include "Yellow_Button.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_INPUT_LEN 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern uint8_t penguin[];

char buffVal0[12];
char buffVal1[12];
char buffVal2[12];
char buffVal3[12];
char buffVal4[12];
char buffVal5[12];
char buffVal6[12];
char buffVal7[12];

uint16_t Value0;
uint16_t Value1;
uint16_t Value2;
uint16_t Value3;
uint16_t Value4;
uint16_t Value5;
uint16_t Value6;
uint16_t Value7;

char input_buff[MAX_INPUT_LEN] = {};

uint32_t wait_time = 0;
uint32_t start_time = 0;
// uint32_t interpreter_data_len = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
char UART_Check_for_string(char * ptr, uint32_t max_size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stdlib.h"
uint16_t rand16(void)
{
  return (uint16_t)(READ_REG(RNG->DR));
}

#define sinus_points 255
#define minimal_amplitude 5

float PI = 3.14159;
float sin_table_a[sinus_points];
float sin_table_b[sinus_points];
float sin_table_c[sinus_points];

void sin_init(uint16_t i)
{
  sin_table_c[(i+(sinus_points/3*1)) & (sinus_points)]=
  sin_table_b[(i+(sinus_points/3*2)) & (sinus_points)]=
  sin_table_a[(i+(sinus_points/3*3)) & (sinus_points)]=
  ((sin((float) i*(2*PI/sinus_points)))*(sinus_points/2));
}

void sinusoide_scroll(void)
{
  for(uint16_t shift=80 ; shift<(UB_VGA_Get_Y()-128); shift++)
  {
    for(uint16_t i = 0; i<UB_VGA_Get_X() ; i++)
    {
      static volatile uint16_t x;
      x = i & sinus_points;
      if(x>=sinus_points) x-=sinus_points;
        writePixel(i, (sin_table_a[x]/2)+shift, VGA_COL_BLUE);
        writePixel(i, (sin_table_b[x]/2)+shift, VGA_COL_GREEN);
        writePixel(i, (sin_table_c[x]/2)+shift, VGA_COL_RED);
    }
    for(uint32_t i=0;i<5e5;i++) __asm volatile ("nop");
    for(uint16_t i = 0; i<UB_VGA_Get_X() ; i++)
    {
      static volatile uint16_t x;
      x = i & sinus_points;
      if(x>=sinus_points) x-=sinus_points;
      writePixel(i, (sin_table_a[x]/2)+shift, 0);
      writePixel(i, (sin_table_b[x]/2)+shift, 0);
      writePixel(i, (sin_table_c[x]/2)+shift, 0);
    }
  }
  for(uint16_t shift=(UB_VGA_Get_Y()-128) ; shift>80; shift--)
  {
    for(uint16_t i = UB_VGA_Get_X(); i>0 ; i--)
    {
      static volatile uint16_t x;
      x = i & sinus_points;
      if(x>=sinus_points) x-=sinus_points;
      writePixel(i, (sin_table_a[x]/2)+shift, VGA_COL_BLUE);
      writePixel(i, (sin_table_b[x]/2)+shift, VGA_COL_GREEN);
      writePixel(i, (sin_table_c[x]/2)+shift, VGA_COL_RED);
    }
    for(uint32_t i=0;i<5e5;i++) __asm volatile ("nop");
    for(uint16_t i = UB_VGA_Get_X(); i>0 ; i--)
    {
      static volatile uint16_t x;
      x = i & sinus_points;
      if(x>=sinus_points) x-=sinus_points;
      writePixel(i, (sin_table_a[x]/2)+shift, 0);
      writePixel(i, (sin_table_b[x]/2)+shift, 0);
      writePixel(i, (sin_table_c[x]/2)+shift, 0);
    }
  }
}

void draw_dot_grid(void)
{
    for(uint8_t i=0;i<0b11111111;i+=0b11)
    {
      /*
      for(uint16_t x = 20; x<UB_VGA_Get_Y()-16; x+= 4)
          for(uint16_t i = 4; i<UB_VGA_Get_X()-16; i+=32)writePixel(i, x, VGA_COL_BLUE);
      for(uint16_t x = 10; x<UB_VGA_Get_Y()-16; x+=32)
          for(uint16_t i = 4; i<UB_VGA_Get_X()-16; i+= 4)writePixel(i, x, VGA_COL_GREEN);
      */
      for(uint16_t x = 16;x<UB_VGA_Get_Y()-15;x+= 8)
        for(uint16_t i = 16; i<UB_VGA_Get_X()-15; i+=32) fillRect(i,x,1,1,VGA_COL_BLUE);
      for(uint16_t x = 16;x<UB_VGA_Get_Y()-15;x+=32)
        for(uint16_t i = 16; i<UB_VGA_Get_X()-15; i+=8) fillRect(i,x,1,1,VGA_COL_GREEN);
    }
}

void draw_dot_grid2(void)
{
  for(uint16_t i = 5; i<UB_VGA_Get_Y()-5; i+=30)  { draw_dot_line(5, i, UB_VGA_Get_X()-5, i, 0xff); }
  for(uint16_t i = 5; i<UB_VGA_Get_X()-5; i+=30)  { draw_dot_line(i, 5, i, UB_VGA_Get_Y()-5, 0xff); }
}

void draw_grid(void)
{
  for(uint16_t i = 5; i<UB_VGA_Get_Y()-5; i+=30)  { writeLine(5, i, UB_VGA_Get_X()-5, i, rand16()); }
  for(uint16_t i = 5; i<UB_VGA_Get_X()-5; i+=30)  { writeLine(i, 5, i, UB_VGA_Get_Y()-5, rand16()); }
}

void draw_pixels(void)
{
  for(uint32_t i=0;i<1e5;i++)
  {
    writePixel(rand16()%UB_VGA_Get_X(), rand16()%UB_VGA_Get_Y(), 0);
    if(i%100) writePixel(rand16()%UB_VGA_Get_X(), rand16()%UB_VGA_Get_Y(), rand16());
    // for(uint32_t i=0;i<10;i++) __asm volatile ("nop");
  }
}

void analog_read(void)
{
//   Value1 = adc_get_value(ADC1, GPIOA, 0x1UL << 1);
//   Value2 = adc_get_value(ADC1, GPIOA, 0x1UL << 2);
//   Value3 = adc_get_value(ADC1, GPIOA, 0x1UL << 3);
//   Value4 = adc_get_value(ADC1, GPIOA, 0x1UL << 4);
//   Value5 = adc_get_value(ADC1, GPIOA, 0x1UL << 5);
//   Value6 = adc_get_value(ADC1, GPIOA, 0x1UL << 6);
//   Value7 = adc_get_value(ADC1, GPIOA, 0x1UL << 7);
//
//   sprintf(buffVal1,"A1 %3d,\n",Value1);
//   sprintf(buffVal2,"A= %3d,\n",'A');
//   sprintf(buffVal3,"A3 %3d,\n",Value3);
//   sprintf(buffVal4,"A4 %3d,\n",Value4);
//   sprintf(buffVal5,"A5 %3d,\n",Value5);
//   sprintf(buffVal6,"A6 %3d,\n",Value6);
//   sprintf(buffVal7,"A7 %3d,\n",Value7);
//
//   LCD_Font(15,   20, "read adc channel:",  &AvrileSerif_Regular12pt8b,  1, VGA_COL_RED,  0);
//
//   // LCD_Font(45,   60, buffVal1, &ArialMT12pt8b,  1, VGA_COL_RED,    0);
//   LCD_Font(15,   50, buffVal2, &sofia6pt7b,  1, VGA_COL_GREEN,  0);
//   LCD_Font(15,   70, buffVal3, &sofia8pt7b,  1, VGA_COL_CYAN, 0);
//   LCD_Font(15,   90, buffVal4, &sofia10pt7b, 1, VGA_COL_CYAN, 0);
//   LCD_Font(15,  110, buffVal5, &sofia12pt7b, 1, VGA_COL_CYAN, 0);
//   LCD_Font(15,  135, buffVal6, &sofia14pt7b, 1, VGA_COL_CYAN, 0);
//   LCD_Font(15,  165, buffVal7, &sofia16pt7b, 1, VGA_COL_CYAN, 0);
//
//   for(uint32_t i=0;i<1e7;i++) __asm volatile ("nop");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */
  UB_VGA_ll_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  // Init VGA-Screen
  UB_VGA_Screen_Init(800,600);

  printf("\rstarted display\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  for (uint32_t i=0; i<sinus_points; i++)
  {
    sin_init(i);
  }
  
  while (1)
  {
    // fillRect(0, 0, 320, 240, 0);
    // for(uint32_t i=0;i<50;i++) analog_read();

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    for(uint32_t i = 0; i < 2; i++) sinusoide_scroll();

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    draw_dot_grid();
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    draw_dot_grid2();
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    draw_grid();
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    draw_pixels();
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0);
    for(uint32_t i = 0; i < 1e3; i++)
    {
      fillRoundRect(rand16()%UB_VGA_Get_X(), rand16()%UB_VGA_Get_Y(), 35, 25, 10, rand16());
    } for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0x0);

    setTextSize(1);
    UG_PutString(45,  10,  "font_Arial_13", VGA_COL_CYAN,0,    &font_Arial_13X14);  // , 1, rand16(),0);
    UG_PutString(45,  25,  "font_Arial_15", VGA_COL_RED,0,     &font_Arial_15X16);  // , 1, rand16(),0);
    UG_PutString(45,  44,  "font_Arial_19", VGA_COL_GREEN,0,   &font_Arial_19X19);  // , 1, rand16(),0);
    UG_PutString(45,  70,  "font_Arial_23", VGA_COL_BLUE,0,    &font_Arial_23X23);  // , 1, rand16(),0);
    UG_PutString(45,  100, "font_Arial_27", VGA_COL_MAGENTA,0, &font_Arial_27X27);  // , 1, rand16(),0);
    UG_PutString(45,  136, "font_Arial_31", VGA_COL_YELLOW,0,  &font_Arial_31X30);   // , 1, rand16(),0);
    UG_PutString(45,  180, "font_Arial_37", VGA_COL_CYAN,0,    &font_Arial_37X38);   // , 1, rand16(),0);

    for(uint32_t i=0;i<2e9;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0x0);
    drawRGBBitmap_U16(55, 55, penguin, 40, 40);
    drawRGBBitmap_U16(155,55, icon_40x40, 40, 40);
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    fillRect(0, 0, UB_VGA_Get_X(), UB_VGA_Get_Y(), 0x0);
    drawRGBBitmap_U16(22, 34,   (uint16_t*)Green_button, 120, 72);
    drawRGBBitmap_U16(22, 130,  (uint16_t*)Yellow_Button, 80, 40);
    drawRGBBitmap_U16(174, 34,  (uint16_t*)Green_button, 120, 72);
    drawXBitmap_U8(174, 130, (uint16_t*)Red_Button,    80, 40,0x55);
    for(uint32_t i=0;i<4e8;i++) __asm volatile ("nop");

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_HSI48_Enable();

   /* Wait till HSI48 is ready */
  while(LL_RCC_HSI48_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(480);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  LL_Init1msTick(480000000);

  LL_SetSystemCoreClock(480000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
