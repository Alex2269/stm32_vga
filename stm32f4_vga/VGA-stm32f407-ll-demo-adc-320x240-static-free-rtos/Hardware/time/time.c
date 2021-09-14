#include "time.h"
#include "task.h"

// system functions
uint32_t get_time_reload(void)
{
  return (*((volatile uint32_t *)0xe000e014)); // SYSTICK_LOAD_REG
}

uint32_t get_time_current(void)
{
  return (*((volatile uint32_t *)0xe000e018)); // SYSTICK_CURRENT_VALUE_REG
}

uint32_t get_time_us(void)
{
  volatile uint32_t scale = 1000000 / configTICK_RATE_HZ; /* microsecond */
  return (xTaskGetTickCount() * scale + (get_time_reload() - get_time_current()) * (1.0) / (get_time_reload() / scale)) ;
}

uint32_t get_time_ms(void)
{
  volatile uint32_t scale = 1000 / configTICK_RATE_HZ; /* microsecond */
  return (xTaskGetTickCount() * scale + (get_time_reload() - get_time_current()) * (1.0) / (get_time_reload() / scale)) ;
}

uint32_t get_time_sec(void)
{
  volatile uint32_t scale = 1000 / configTICK_RATE_HZ; /* microsecond */
  return (xTaskGetTickCount() * scale + (get_time_reload() - get_time_current()) * (1.0) / (get_time_reload() / scale)) / 1000;
}

// applicatoin functions
void save_time_us(void)
{
  save_us = get_time_us();
}

uint32_t get_save_time_us(void)
{
  return save_us;
}

void save_time_ms(void)
{
  save_ms = get_time_ms();
}

uint32_t get_save_time_ms(void)
{
  return save_ms;
}

void save_time_sec(void)
{
  save_sec = get_time_sec();
}

uint32_t get_save_time_sec(void)
{
  return save_sec;
}

uint32_t get_time_new_us(void)
{
  return get_time_us() - get_save_time_us();
}

uint32_t get_time_new_ms(void)
{
  return get_time_ms() - get_save_time_ms();
}

uint32_t get_time_new_sec(void)
{
  return get_time_sec() - get_save_time_sec();
}


