/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    uart.c:
          UART2 driver for SpARM-board v1

    pin-info:
           PA2 - TX
           PA3 - RX

To enable UART use the following line:
	UART_init();
To use UART on interrupt base use:
	UART_INT_init();

This file initialize the UART on the ARM-board v5.
To send data to the UART use:
	UART_printf(*string);

To read the UART without interrupt use:
	char = USART2->DR & 0xFF;

In the interrupt routine the char is send back to the terminal
*/

#ifndef __UART_H
#define __UART_H

/****************Libraries******************************/

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_pwr.h"
#include "misc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


/****************Function Prototypes********************/
void 		UART_init		(void);
signed int 	UART_printf		(size_t length, const char *pFormat, ...);
void 		UART_INT_init	(void);
#endif//__UART_H
