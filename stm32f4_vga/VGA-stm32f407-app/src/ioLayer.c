/**
 ***************************************************************
 *@file 	ioLayer.c
 *@author 	Menno Janssen and Benno Driessen
 *@date		19 may 2018
 *@brief	IO layer of the Demo program. All the hardware systems are initialized in this file.
 ***************************************************************
 */
#include "ioLayer.h"
extern volatile int charcounter;//UART buffer counter
extern char string[100];//UART buffer


/**
 * @brief initializes the VGA screen, the delay function, the uart and the uart interrupt.
 * @param void
 * @retval void
 */
void IO_init(void)
{

	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen

	DELAY_init();	// Init delay lib

	UART_init();	// To enable UART
	UART_INT_init(); // To use UART on interrupt base
}
