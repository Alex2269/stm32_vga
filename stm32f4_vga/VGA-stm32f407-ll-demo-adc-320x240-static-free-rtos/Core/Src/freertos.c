/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc_cfg.h"

#include "fonts_all_includes.h"
#include "time.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_01 */
osThreadId_t Task_01Handle;
const osThreadAttr_t Task_01_attributes = {
  .name = "Task_01",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_02 */
osThreadId_t Task_02Handle;
const osThreadAttr_t Task_02_attributes = {
  .name = "Task_02",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_03 */
osThreadId_t Task_03Handle;
const osThreadAttr_t Task_03_attributes = {
  .name = "Task_03",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_04 */
osThreadId_t Task_04Handle;
const osThreadAttr_t Task_04_attributes = {
  .name = "Task_04",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask_01(void *argument);
void StartTask_02(void *argument);
void StartTask_03(void *argument);
void StartTask_04(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_01 */
  Task_01Handle = osThreadNew(StartTask_01, NULL, &Task_01_attributes);

  /* creation of Task_02 */
  Task_02Handle = osThreadNew(StartTask_02, NULL, &Task_02_attributes);

  /* creation of Task_03 */
  Task_03Handle = osThreadNew(StartTask_03, NULL, &Task_03_attributes);

  /* creation of Task_04 */
  Task_04Handle = osThreadNew(StartTask_04, NULL, &Task_04_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask_01 */
/**
  * @brief  Function implementing the Task_01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_01 */
void StartTask_01(void *argument)
{
  /* USER CODE BEGIN StartTask_01 */

  char buff_time[12];
  static volatile uint32_t time_ms;
  save_time_ms(); // start chronometry

  /* Infinite loop */
  for(;;)
  {
    analog_read();

    time_ms = get_time_new_ms(); // read chronometry
    sprintf(buff_time,"%ld msec", time_ms);
    fillRect(180, 30, 130, 14, VGA_COL_BLACK);
    UG_PutString(180, 30, buff_time, VGA_COL_CYAN,VGA_COL_BLACK, &font_Arial_13X14);

    osDelay(100);
  }
  /* USER CODE END StartTask_01 */
}

/* USER CODE BEGIN Header_StartTask_02 */
/**
* @brief Function implementing the Task_02 thread.
* @param argument: Not used
* @retval None
*/
volatile uint16_t buff_pump_draw[480] = {0};
volatile uint16_t buff_pump_clean[480] = {0};
/* USER CODE END Header_StartTask_02 */
void StartTask_02(void *argument)
{
  /* USER CODE BEGIN StartTask_02 */
  static volatile uint16_t xpos;

  /* Infinite loop */
  for(;;)
  {
    // sinusoide_scroll();
    // for(uint32_t i=0;i<1e4;i++) __asm volatile ("nop");

    xpos++;
    if(xpos > UB_VGA_Get_X()) xpos = 0;

    buff_pump_draw[xpos] = read_adc(ADC1, GPIOA, 0x1UL << 7)/30+52;
    write_graph_color(xpos, buff_pump_draw, buff_pump_clean, 1, 0x55);

    osDelay(25);
  }
  /* USER CODE END StartTask_02 */
}

/* USER CODE BEGIN Header_StartTask_03 */
/**
* @brief Function implementing the Task_03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_03 */
void StartTask_03(void *argument)
{
  /* USER CODE BEGIN StartTask_03 */
  fillRoundRect(8, UB_VGA_Get_Y()-30, UB_VGA_Get_X()-15, 24, 6, VGA_COL_BLUE);
  drawRoundRect(6, UB_VGA_Get_Y()-31, UB_VGA_Get_X()-11, 26, 6, VGA_COL_RED);
  /* Infinite loop */
  for(;;)
  {
    /***
    fillRect(2, 2, UB_VGA_Get_X()-2, 4, VGA_COL_YELLOW); // horisontal-up
    fillRect(2, UB_VGA_Get_Y()-2, UB_VGA_Get_X()-2, 4, VGA_COL_YELLOW); // horisontal-down
    fillRect(2,   2,   2, UB_VGA_Get_Y()-4, VGA_COL_YELLOW); // vertical-left
    fillRect(318, 2,   2, UB_VGA_Get_Y()-4, VGA_COL_YELLOW); // vertical-right ***/

    drawRoundRect(1, 5, UB_VGA_Get_X()-2, UB_VGA_Get_Y()-6,  10, VGA_COL_BLUE);
    drawRoundRect(2, 6, UB_VGA_Get_X()-4, UB_VGA_Get_Y()-8,  10, VGA_COL_YELLOW);
    drawRoundRect(3, 7, UB_VGA_Get_X()-6, UB_VGA_Get_Y()-10, 10, VGA_COL_CYAN);
    drawRoundRect(4, 8, UB_VGA_Get_X()-8, UB_VGA_Get_Y()-12, 10, VGA_COL_MAGENTA);

    // fillRoundRect(8, UB_VGA_Get_Y()-30, UB_VGA_Get_X()-15, 24, 6, VGA_COL_BLUE);
    // drawRoundRect(6, UB_VGA_Get_Y()-31, UB_VGA_Get_X()-11, 26, 6, VGA_COL_RED);

    // for(volatile uint32_t i=0;i<(UB_VGA_Get_X() * UB_VGA_Get_Y());i++) VGA_RAM1[i]=0x0;
    draw_dot_grid();

    drawRoundRect(97,  9, 76, 92, 8, VGA_COL_RED);
    drawRoundRect(98, 10, 74, 90, 8, VGA_COL_GREEN);
    drawRoundRect(99, 11, 72, 88, 8, VGA_COL_CYAN);

    #define lft 172
    #define win_width 136
    drawRoundRect(lft+1,  9, win_width+6, 92, 8, VGA_COL_RED);
    drawRoundRect(lft+2, 10, win_width+4, 90, 8, VGA_COL_GREEN);
    drawRoundRect(lft+3, 11, win_width+2, 88, 8, VGA_COL_CYAN);

    drawRoundRect(5, 101, UB_VGA_Get_X()-10,  108, 8, VGA_COL_RED);
    drawRoundRect(6, 102, UB_VGA_Get_X()-12,  106, 8, VGA_COL_GREEN);
    drawRoundRect(7, 103, UB_VGA_Get_X()-14,  104, 8, VGA_COL_CYAN);

    // drawRoundRect(99, 11, 72, 88, 8, VGA_COL_BLACK);
    // for(uint32_t i=0;i<5e6;i++) __asm volatile ("nop");
    osDelay(500);
  }
  /* USER CODE END StartTask_03 */
}

/* USER CODE BEGIN Header_StartTask_04 */
/**
* @brief Function implementing the Task_04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_04 */
void StartTask_04(void *argument)
{
  /* USER CODE BEGIN StartTask_04 */

  static volatile uint16_t y_offset, x_offset, level_width;
  static volatile uint16_t val0,val1,val2,val3,val4,val5,val6,val7;
  x_offset = 106;
  y_offset = 92;
  level_width = 1;

  ADC_Init(ADC1);

  /* Infinite loop */
  // #include "stm32_ub_gol.h"
  // GOL_Init();
  // GOL_SetCellsRandom();
  // GOL_ShowAllCells();
  for(;;)
  {
    // GOL_UpdateAllCells();
    // GOL_ShowAllCells();

    write_graph_dot_line(x_offset + 8 * 0, y_offset, x_offset + 8 * 0, y_offset - val0, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 1, y_offset, x_offset + 8 * 1, y_offset - val1, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 2, y_offset, x_offset + 8 * 2, y_offset - val2, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 3, y_offset, x_offset + 8 * 3, y_offset - val3, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 4, y_offset, x_offset + 8 * 4, y_offset - val4, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 5, y_offset, x_offset + 8 * 5, y_offset - val5, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 6, y_offset, x_offset + 8 * 6, y_offset - val6, level_width, VGA_COL_BLACK);
    write_graph_dot_line(x_offset + 8 * 7, y_offset, x_offset + 8 * 7, y_offset - val7, level_width, VGA_COL_BLACK);

    val0 = read_adc(ADC1, GPIOA, 0x1UL << 0)/40;
    val1 = read_adc(ADC1, GPIOA, 0x1UL << 1)/40;
    val2 = read_adc(ADC1, GPIOA, 0x1UL << 2)/40;
    val3 = read_adc(ADC1, GPIOA, 0x1UL << 3)/40;
    val4 = read_adc(ADC1, GPIOA, 0x1UL << 4)/40;
    val5 = read_adc(ADC1, GPIOA, 0x1UL << 5)/40;
    val6 = read_adc(ADC1, GPIOA, 0x1UL << 6)/40;
    val7 = read_adc(ADC1, GPIOA, 0x1UL << 7)/40;

    write_graph_dot_line(x_offset + 8 * 0, y_offset, x_offset + 8 * 0, y_offset - val0, level_width, VGA_COL_CYAN);
    write_graph_dot_line(x_offset + 8 * 1, y_offset, x_offset + 8 * 1, y_offset - val1, level_width, VGA_COL_MAGENTA);
    write_graph_dot_line(x_offset + 8 * 2, y_offset, x_offset + 8 * 2, y_offset - val2, level_width, VGA_COL_YELLOW);
    write_graph_dot_line(x_offset + 8 * 3, y_offset, x_offset + 8 * 3, y_offset - val3, level_width, VGA_COL_GREEN);
    write_graph_dot_line(x_offset + 8 * 4, y_offset, x_offset + 8 * 4, y_offset - val4, level_width, VGA_COL_CYAN);
    write_graph_dot_line(x_offset + 8 * 5, y_offset, x_offset + 8 * 5, y_offset - val5, level_width, VGA_COL_MAGENTA);
    write_graph_dot_line(x_offset + 8 * 6, y_offset, x_offset + 8 * 6, y_offset - val6, level_width, VGA_COL_YELLOW);
    write_graph_dot_line(x_offset + 8 * 7, y_offset, x_offset + 8 * 7, y_offset - val7, level_width, VGA_COL_GREEN);

//     drawRect(97,  9, 76, 92, VGA_COL_RED);
//     drawRect(98, 10, 74, 90, VGA_COL_GREEN);
//     drawRect(99, 11, 72, 88, VGA_COL_CYAN);

    for(uint32_t i=0;i<2e3;i++) __asm volatile ("nop");
    osDelay(50);
  }
  /* USER CODE END StartTask_04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
