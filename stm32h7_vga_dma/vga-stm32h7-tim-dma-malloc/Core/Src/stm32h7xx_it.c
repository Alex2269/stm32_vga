/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_ub_vga_screen.h"
extern VGA_t VGA;
extern uint8_t *VGA_RAM1;
// extern uint8_t VGA_RAM1[];

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  SCB_CleanDCache_by_Addr((uint32_t*) VGA_RAM1, UB_VGA_Get_X()/2);

  TIM2->SR &= ~TIM_DIER_CC3IE; // __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC3);
  VGA.hsync_cnt++;
  if (VGA.hsync_cnt >= VGA_VSYNC_PERIODE)
  {
    VGA.hsync_cnt = 0;
    // Adresspointer first dot
    VGA.start_adr = (uint32_t)VGA_RAM1;
  }

  // HSync-Pixel
  GPIOB->BSRR = (VGA.hsync_cnt < VGA_VSYNC_IMP) ? VGA_VSYNC_Pin << 16u: VGA_VSYNC_Pin;
  // if(VGA.hsync_cnt < VGA_VSYNC_IMP) GPIOB->BSRR = VGA_VSYNC_Pin << 16u; else GPIOB->BSRR = VGA_VSYNC_Pin;
  // if(VGA.hsync_cnt % 360) GPIOB->BSRR = VGA_VSYNC_Pin << 16u; else GPIOB->BSRR = VGA_VSYNC_Pin;

  // Test for DMA start
  if((VGA.hsync_cnt >= VGA_VSYNC_BILD_START) && (VGA.hsync_cnt <= VGA_VSYNC_BILD_STOP))
  {
    // after FP start => DMA Transfer

    // DMA2 init
    DMA2_Stream5->CR = VGA.dma2_cr_reg;

    // set adress
    DMA2_Stream5->M0AR = VGA.start_adr;

    // Timer1 start Enable timer counter.
    TIM1->CR1 |= TIM_CR1_CEN; // LL_TIM_EnableCounter(TIM1)

    DMA2_Stream5->CR |= DMA_SxCR_EN; // LL_DMA_EnableStream(DMA2, 5);

    // Test Adrespointer for high
    // if(VGA.hsync_cnt & 0x01)
    VGA.start_adr += UB_VGA_Get_X(); // inc after Hsync
  }

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream5 global interrupt.
  */
void DMA2_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream5_IRQn 0 */
  // DMA2_Stream5->CR |= DMA_SxCR_EN; // LL_DMA_EnableStream(DMA2, 5);

  /* Disable DMA stream. */
  DMA2_Stream5->CR &= ~DMA_SxCR_EN; // LL_DMA_DisableStream(DMA2, 5);

  /* Disable timer counter. */
  TIM1->CR1 &= ~TIM_CR1_CEN; // LL_TIM_DisableCounter(TIM1);

  /* switch on black */
  GPIOE->BSRR = VGA_GPIO_HINIBBLE << 16u;

  /* Enable Common interrupts*/
  DMA2_Stream5->CR    |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;

  DMA2_Stream5->FCR &= ~DMA_SxFCR_FEIE; // LL_DMA_DisableIT_FE(DMA_TypeDef *DMA2, 5)  Disable FIFO error interrupt.

  // DMA2->HIFCR |= DMA_HIFCR_CHTIF5;      // LL_DMA_ClearFlag_HT5(DMA2);   Clear Stream 5 half transfer flag.
  DMA2->HIFCR |= DMA_HIFCR_CTCIF5;      // LL_DMA_ClearFlag_TC5(DMA2);   Clear Stream 5 transfer complete flag.
  DMA2->HIFCR |= DMA_HIFCR_CTEIF5;      // LL_DMA_ClearFlag_TE5(DMA2);   Clear Stream 5 transfer error flag.
  DMA2->HIFCR |= DMA_HIFCR_CDMEIF5;     // LL_DMA_ClearFlag_DME5(DMA2);  Clear Stream 5 direct mode error flag.

  // DMA2->HIFCR |= DMA_HIFCR_CFEIF5;      // LL_DMA_ClearFlag_FE5(DMA2);   Clear Stream 5 FIFO error flag.
  // DMA2_Stream5->CR &= (uint32_t)(~DMA_SxCR_DBM);  /* Disable the double buffer mode. */

  SCB_CleanDCache_by_Addr((uint32_t*) VGA_RAM1, UB_VGA_Get_X()/2);

  /* USER CODE END DMA2_Stream5_IRQn 0 */

  /* USER CODE BEGIN DMA2_Stream5_IRQn 1 */

  /* USER CODE END DMA2_Stream5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
