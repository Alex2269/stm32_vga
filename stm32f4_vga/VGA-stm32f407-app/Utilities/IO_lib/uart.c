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

#include "uart.h"

/****************Globals********************************/
char string[100];
volatile int charcounter = 0;

void UART_init(void)
/* UART Initialize
 * Initializes the pins and UART configurations
 */
{
	/* --------------------------- System Clocks Configuration -----------------*/
	/* USART2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   // USART2_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // USART2_RX

	USART_InitTypeDef USART_InitStructure;

/* USARTx configuration ------------------------------------------------------*/
/* USARTx configured as follow:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - One Stop Bit
      - No parity
      - Hardware flow control disabled (RTS and CTS signals)
      - Receive and transmit enabled
*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void UART_INT_init(void)
/* UART Interrupt initialize
 * Initialize the NVIC for interrupt handling.
 */
{
	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_IRQHandler(void)
/* UART IRQ Handler
 * This function handles the interrupt. In this function the char can be put
 * in an global or something can be done here.
 */
{
//	if ( charcounter == 0 )
//		memset(string, 0, sizeof(string));

	// check if the USART2 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		char c = USART2->DR & 0xFF;

		if(c > 64 && c < 91)
			c += 32;
		if(c <= 13)
		{
			charcounter = 0;
			//memset(string, 0, sizeof(string));
		}
		else
		{
			string[charcounter] = c;
			charcounter++;
		}


		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // Wait for Empty

	}
}

signed int UART_printf(size_t length, const char *pFormat, ...)
/* UART printf
 * This function translates a string with variables to a string and puts in on the UART
 */
{
    va_list    ap;
    signed int rc;
    char c[length];
    char *pString = c;

    va_start(ap, pFormat);
    rc = vsnprintf(pString, length, pFormat, ap);
    va_end(ap);

    volatile unsigned char i;
	for (i=0; pString[i]; i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
		USART_SendData(USART2, pString[i]);
	}

    return rc;
}
