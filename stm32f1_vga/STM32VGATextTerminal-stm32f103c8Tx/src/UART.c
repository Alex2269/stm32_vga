/*
 * STM32VGATextTerminal
 *
 * Created on: 05.11.2012
 *     Author: vasyaod (vasyaod@mail.ru)
 *       Link: https://github.com/vasyaod/STM32VGATextTerminal
 */

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <misc.h>

#include "Terminal.h"

/**
 * Функция инициализирует UART.
 */
void initUART()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Enable USARTy Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

//	USART_ClockInitTypeDef USART_ClockInitStructure;
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
//	USART_ClockInit(USART1, &USART_ClockInitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure the USARTy
	USART_Init(USART1, &USART_InitStructure);

	// Configure interrupted the USARTy
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);

	// Enable the USARTy
	USART_Cmd(USART1, ENABLE);

}
void USART1_IRQHandler() {
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {

		// Читаем принятые данные и сбрасываем флаг.
		uint8_t data = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		TerminalPutchar(data);
	}
}
