#ifndef __STM32_TICK_H__
#define __STM32_TICK_H__

#include "main.h"

#define  DWT_CYCCNT   *(volatile uint32_t *)0xE0001004
#define  DWT_CONTROL  *(volatile uint32_t *)0xE0001000
#define  SCB_DEMCR    *(volatile uint32_t *)0xE000EDFC

#define ticks_get()          (DWT_CYCCNT)
#define ticks_set(val)       {DWT_CYCCNT = val;}

#define ticks_time_start(v)  {v = DWT_CYCCNT;}
#define ticks_time_ms(v)     ((DWT_CYCCNT - v) / (SystemCoreClock / 10e3L))
#define ticks_time_us(v)     ((DWT_CYCCNT - v) / (SystemCoreClock / 10e6L))

static inline uint32_t
GetSystemClockHz(void)
{
  uint32_t frequency = 0U;
  /* Get SYSCLK source -------------------------------------------------------*/
  switch (LL_RCC_GetSysClkSource())
  {
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:  /* HSI used as system clock  source */
      frequency = HSI_VALUE; break;
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:  /* HSE used as system clock  source */
      frequency = HSE_VALUE; break;
    default:
      frequency = HSI_VALUE; break;
  }
  return frequency;
}

uint32_t SysTick_config(uint32_t reload);
void ticks_init();

#endif // #ifndef __STM32_TICK_H__
