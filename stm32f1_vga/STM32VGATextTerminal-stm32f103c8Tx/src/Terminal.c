/*
 * STM32VGATextTerminal
 *
 * Created on: 21.10.2012
 *     Author: vasyaod (vasyaod@mail.ru)
 *       Link: https://github.com/vasyaod/STM32VGATextTerminal
 */
#include <stdlib.h>
#include <string.h>

#include "Terminal.h"
#include "VGA.h"

uint8_t cursorX = 0;
uint8_t cursorY = 0;

uint8_t resolutionX;
uint8_t resolutionY;

uint8_t *buffer;
/**
 * escapeCodeState - набор состояний автомата, который парсит esc-код:
 *    0 - escape-последовательность отсутствует;
 *    1 - на вход был подан символ 0x1B (начало последовательности);
 *    2 - идет обработка параметра;
 */
uint8_t escapeCodeState;
uint8_t *escapeParameters;
uint32_t escapeParameter1;
uint32_t escapeParameter2;
uint8_t escapeParametersCount;

uint8_t bufLen;
uint8_t tmpChar;

void TerminalInit(void)
{
	buffer = malloc(80);
	escapeCodeState = 0;
	escapeParametersCount = 0;
	bufLen = 0;
}

/**
 * Данная функция призвана обрабатывать escape-последовательности после того
 * как они распарсены.
 */
void TerminalEscapeProcessing(void)
{
	uint8_t code1 = buffer[1];
	uint8_t code2 = buffer[bufLen-1];

	// Esc[Line;ColumnH - переводит курсор в указанное положение.
	// Esc[Line;Columnf - переводит курсор в указанное положение.
	if (code1 == '[' && (code2 == 'H' || code2 == 'f'))
	{
		if (escapeParametersCount == 2)
		{
			cursorY = escapeParameter1 - 1;
			cursorX = escapeParameter2 - 1;

		}

		if (cursorX < 0)
			cursorX = 0;
		else if (cursorX >= resolutionX)
			cursorX = resolutionX-1;

		if (cursorY < 0)
			cursorY = 0;
		else if (cursorY >= resolutionY)
			cursorY = resolutionY-1;

		return;
	}

	// Очистка экрана.
	// Esc[J - Clear screen from cursor down
	// Esc[0J - Clear screen from cursor down
	// Esc[1J - Clear screen from cursor up
	// Esc[2J - Clear entire screen
	if (code1 == '[' && code2 == 'J')    // Переводит курсор в указанное положение.
	{
		if (escapeParametersCount == 0 ||
		    (escapeParametersCount == 1 && escapeParameter1 == 0))
		{
			// ... не реализовано ...
		}
		else if (escapeParametersCount == 1 && escapeParameter1 == 1)
		{
			// ... не реализовано ...
		}
		else if (escapeParametersCount == 1 && escapeParameter1 == 2)
		{
			VGAFillScreen(0);
		}
		return;
	}
}

/**
 * Внутренний метод, читает параметры внутри escape-последовательности.
 */
void TerminalReadParameter(void)
{
	tmpChar = buffer[bufLen-1];
	buffer[bufLen-1] = 0x0;
	if (escapeParametersCount == 1)
		escapeParameter1 = atoi((char*)escapeParameters);
	if (escapeParametersCount == 2)
		escapeParameter2 = atoi((char*)escapeParameters);
	buffer[bufLen-1] = tmpChar;
}

void TerminalPutchar(uint8_t data)
{
	if (escapeCodeState > 0)
	{
		buffer[bufLen] = data;
		bufLen++;

		// Предположим, что конец esc-кода всегда заканчивается на букву.
		if (((data >= 0x41 && data < 0x5A) ||   // A-Z
			(data >= 0x61 && data < 0x7A)))     // a-z
		{
			if (escapeCodeState == 2) // Завершим чтение параметра.
				TerminalReadParameter();

			TerminalEscapeProcessing();
			escapeCodeState = 0;
			bufLen = 0;
		}
		else if (escapeCodeState == 1 && (data >= 0x30 && data < 0x39)) // Встречен цифровой параметр.
		{
			escapeParameters = buffer + bufLen-1;  // Установим указатель на начало параметра.
			escapeParametersCount++;               // Увеличим количество параметров на единицу.
			escapeCodeState = 2;
		}
		else if (escapeCodeState == 2 && !(data >= 0x30 && data < 0x39)) // Окончание параметра.
		{
			TerminalReadParameter();
			escapeCodeState = 1;
		}
	}
	else if (data >= 32 &&  data <= 94+32)
	{
		VGAPrintChar(cursorX, cursorY, data-32);
		cursorX++;
	}

	// LF (ASCII 0x0A) - перевод строки (UNIX);
	// CR (0x0D) - возврат каретки;
	// CR+LF (ASCII 0x0D 0x0A) - windows, dos ...
	else if (data == 0x0A) // Перевод строки...
	{
		cursorY++;
		cursorX = 0;      // Вернем корретку
	}
	else if (data == 0x0D) // Возврат коретки...
	{
		cursorY++;
		cursorX = 0;      // Вернем корретку
		// При возврате корретки ничего не делаем.
	}
	else if (data == 0x1B) // Начало ESC-последовательности.
	{
		escapeCodeState = 1;
		bufLen = 1;
		buffer[0] = data;
		escapeParametersCount = 0;
	}

	if (cursorX>76) // Перенос строки если символы не помещаются на экране
	{
		cursorY++;
		cursorX = 0;      // Вернем корретку
	}

	if (cursorX >= resolutionX) {
		cursorX = resolutionX-1;
	}
	if (cursorY >= resolutionY) {
		VGAMoveRange(resolutionX, resolutionY-1,
					 0,1,
					 0,0);
		VGAFillRow(resolutionY-1, 0x0);
		cursorY = resolutionY-1;
	}
}

void TerminalPrintf(char *str)
{
	char ch;
	do {
		ch = *(str++);
		TerminalPutchar(ch);
	} while (ch != 0x00);
}
