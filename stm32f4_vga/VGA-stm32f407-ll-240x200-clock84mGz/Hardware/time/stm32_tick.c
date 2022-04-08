#include "stm32_tick.h"

uint32_t SysTick_config(uint32_t reload)
{
  if ((reload - 1UL) > 10e6L)
  {
    return (1UL); /* Reload value impossible */
  }

  NVIC_SetPriority(SysTick_IRQn, 7UL);  /* set Priority for Systick Interrupt */
  SysTick->CTRL = 0b0110;
  SysTick->LOAD = reload;
  // SysTick->VAL  = 0x0UL;
  SysTick->CTRL = 0b0111;

  return (0UL);                         /* Function successful */
}

void ticks_init()
{
  SCB_DEMCR   |= 0x01000000;
  DWT_CONTROL &= ~1; // disable the counter
  DWT_CYCCNT   = 0xF0000000ul;
  DWT_CONTROL |= 1; // enable the counter
}

void delay_us(uint32_t volatile microsec)
{
  microsec *=(SystemCoreClock/10e6L);
  while(microsec--);
}

void delay(uint32_t volatile msec)
{
  msec *=(SystemCoreClock/10e3L);
  while(msec--);
}
