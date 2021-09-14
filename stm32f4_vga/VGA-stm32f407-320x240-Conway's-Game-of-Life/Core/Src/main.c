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

#include "stm32_ub_gol.h"

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
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
char UART_Check_for_string(char * ptr, uint32_t max_size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t rand16(void)
{
  // uint16_t rnd = random();
  // return rnd;
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
  for(uint8_t shift=80 ; shift<160; shift++)
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
    for(uint32_t i=0;i<4e5;i++) __asm volatile ("nop");
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
  for(uint8_t shift=160 ; shift>80; shift--)
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
    for(uint32_t i=0;i<4e5;i++) __asm volatile ("nop");
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
        for(uint16_t i = 16; i<UB_VGA_Get_X()-15; i+=32) writePixel(i, x, VGA_COL_BLUE);
      for(uint16_t x = 16;x<UB_VGA_Get_Y()-15;x+=32)
        for(uint16_t i = 16; i<UB_VGA_Get_X()-15; i+=8) writePixel(i, x, VGA_COL_GREEN);
    }
}

void draw_dot_grid2(void)
{
  for(uint16_t i = 5; i<UB_VGA_Get_Y()-5; i+=30)  { draw_dot_line(5, i, UB_VGA_Get_X()-5, i, rand16()); }
  for(uint16_t i = 5; i<UB_VGA_Get_X()-5; i+=30)  { draw_dot_line(i, 5, i, UB_VGA_Get_Y()-5, rand16()); }
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
  Value1 = read_adc(ADC1, GPIOA, 0x1UL << 1);
  Value2 = read_adc(ADC1, GPIOA, 0x1UL << 2);
  Value3 = read_adc(ADC1, GPIOA, 0x1UL << 3);
  Value4 = read_adc(ADC1, GPIOA, 0x1UL << 4);
  Value5 = read_adc(ADC1, GPIOA, 0x1UL << 5);
  Value6 = read_adc(ADC1, GPIOA, 0x1UL << 6);
  Value7 = read_adc(ADC1, GPIOA, 0x1UL << 7);

  sprintf(buffVal1,"A1 %3d,\n",Value1);
  sprintf(buffVal2,"A2 %3d,\n",Value2);
  sprintf(buffVal3,"A3 %3d,\n",Value3);
  sprintf(buffVal4,"A4 %3d,\n",Value4);
  sprintf(buffVal5,"A5 %3d,\n",Value5);
  sprintf(buffVal6,"A6 %3d,\n",Value6);
  sprintf(buffVal7,"A7 %3d,\n",Value7);

//   UG_PutString(5,    30, "read adc channel:",  &AvrileSerif_Regular12pt8b,  1, VGA_COL_RED,  0);
//   UG_PutString(5,    60, "������ ���������� �������:",  &AvrileSerif_Regular8pt8b,  1, VGA_COL_GREEN,  0);

  setTextSize(1);
  UG_PutString(45,   10, buffVal1, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_GREEN,  0);
  UG_PutString(45,   50, buffVal2, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_YELLOW, 0);
  UG_PutString(45,   90, buffVal3, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_YELLOW, 0);
  UG_PutString(45,  130, buffVal4, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_YELLOW, 0);
  UG_PutString(45,  170, buffVal5, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_YELLOW, 0);
  UG_PutString(45,  210, buffVal6, VGA_COL_MAGENTA,0, &font_Arial_27X27); //, 1, VGA_COL_YELLOW, 0);

  for(uint32_t i=0;i<1e7;i++) __asm volatile ("nop");
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_RNG_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  // SysTick->CTRL = 0; // disable SysTick

  // Init VGA-Screen
  UB_VGA_Screen_Init(320, 240);

  printf("\rstarted display\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  for (uint32_t i=0; i<sinus_points; i++)
  {
    sin_init(i);
  }

  GOL_Init();
  GOL_SetCellsRandom();
  GOL_ShowAllCells();

  while(1)
  {
    GOL_UpdateAllCells();
    GOL_ShowAllCells();
    for(uint32_t i=0;i<1e6;i++) __asm volatile ("nop");

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLQ_DIV_7);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* DMA2_Stream5_IRQn interrupt configuration */
  NVIC_SetPriority(DMA2_Stream5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA2_Stream5_IRQn);
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