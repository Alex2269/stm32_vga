#ifndef _TIME_H
#define _TIME_H

#include "FreeRTOS.h"
#include "stdbool.h"

static volatile uint32_t save_us;
static volatile uint32_t save_ms;
static volatile uint32_t save_sec;

// system functions
uint32_t get_time_reload(void);
uint32_t get_time_current(void);
uint32_t get_time_us(void);
uint32_t get_time_ms(void);
uint32_t get_time_sec(void);

// applicatoin functions
void save_time_us(void);
uint32_t get_save_time_us(void);
void save_time_ms(void);
uint32_t get_save_time_ms(void);
void save_time_sec(void);
uint32_t get_save_time_sec(void);
uint32_t get_time_new_us(void);
uint32_t get_time_new_ms(void);
uint32_t get_time_new_sec(void);

#endif // _TIME_H

