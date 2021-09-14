/**
 ***************************************************************
 *@file 	ioLayer.c
 *@author 	Menno Janssen and Benno Driessen
 *@date		19 may 2018
 *@brief	Logic layer of the demo program. This file contains a way to parse the incoming string and call the functions in the VGA_lib to draw the shapes.
 ***************************************************************
 */
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "LogicLayer.h"


//-------------------------------------------------------------
//internal functions
//--------------------------------------------------------------

uint8_t lineStructFiller(struct parsed *parsedData, char* tok);
uint8_t rectangleStructFiller(struct parsed *parsedData, char* tok);
uint8_t ellipseStructFiller(struct parsed *parsedData, char* tok);
uint8_t triangleStructFiller(struct parsed *parsedData, char* tok);
uint8_t getColor(const char* color);
uint8_t textStructFiller(struct parsed *parsedData, char* tok);
uint8_t clearscreenStructFiller(struct parsed *parsedData, char* tok);
uint8_t delayStructFiller(struct parsed *parsedData, char* tok);
uint8_t bitmapStructFiller(struct parsed *parsedData, char* tok);
const char *error_msg(int code);
extern char string[100];//UART buffer
char str2[100];
extern volatile int charcounter;//UART buffer counter
uint8_t error = 0;



/**
 * @brief Uses an X-macro to get the error message (defined in logicLayer.h) for a given error code.
 * @param code: the error code that needs to be displayed.
 * @retval error message
 */
const char *error_msg(int code)
{
    switch (code) {
        ERROR_CODES(ERROR_TEXT)
    }

    return "Unknown error";
}

/**
 * @brief Takes the data from the UART and separetes the data on every comma using strtok.
 * The functions checks for a command, this can be:
 * 	- "lijn"
 * 	- "clearscherm"
 * 	- "driehoek"
 * 	- "ellips"
 * 	- "bitmap"
 * 	- "tekst"
 * 	- "wacht"
 * 	- "rechthoek"
 * 	When the command is detected the appropriate function to fill the parsedData struct will be called.
 *
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @retval void
 */

void InterpretData(struct parsed *parsedData)
{
	char *tok;
	char tempString[100];
	strcpy(str2, string);

	memset(tempString, 0, sizeof tempString);
	while(1){
		//delay to give the UART some time.
		DELAY_ms(50);
		if(error > 0)
		{
			UART_printf(256, error_msg(error));
			error = 0;
		}
		tok = strtok(string, ",");
		while(tok != NULL)
		{
			strcpy(tempString,tok);
			strcpy(parsedData->text,tok);
			if(strcmp("lijn", tempString)== 0)
				error = lineStructFiller(parsedData, tok);
			else if(strcmp( "clearscherm" ,tempString)== 0)
				error = clearscreenStructFiller(parsedData, tok);
			else if(strcmp( "driehoek", tempString)== 0)
				error = triangleStructFiller(parsedData, tok);
			else if(strcmp( "ellips", tempString)== 0)
				error = ellipseStructFiller(parsedData, tok);
			else if(strcmp( "bitmap", tempString)== 0)
				error = bitmapStructFiller(parsedData, tok);
			else if(strcmp( "tekst", tempString)== 0)
				error = textStructFiller(parsedData, tok);
			else if(strcmp( "wacht", tempString)== 0)
				error = delayStructFiller(parsedData, tok);
			else if(strcmp( "rechthoek", tempString)== 0)
				error = rectangleStructFiller(parsedData, tok);
			else
			{
				//resets the UART counter and clears the buffer
				charcounter = 0;
				memset(string, 0, sizeof string);
				//unknown command error
				error = 6;
				tok = strtok(NULL, ",");
			}
			tok = strtok(NULL, ",");
			memset(tempString, 0, sizeof tempString);
		}
	}

}
/**
 * @brief Fills the Parsedata struct with all the parameters the drawLine functions from the ub_vga_lib needs.
 * These are:
 * 	- x[0] (x_start)
 * 	- y[0] (y_start)
 * 	- x[1] (x_stop)
 * 	- y[1] (y_stop)
 * 	- width
 * 	- color
 * 	After filling the structure it resets the UART and calls the drawLine function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t lineStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	//checks for end of data
	while(tok != NULL)
	{
		//increments to the next datapoint after a comma.
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 2: parsedData->x[1] = strtol(tok, NULL, 10);
		break;
		case 3: parsedData->y[1] = strtol(tok, NULL, 10);
		break;
		case 4: parsedData->width = strtol(tok, NULL, 10);
		break;
		case 5: parsedData->color =  getColor(tok);
		break;
		//too many arguments.
		case 6: error = 7;
		break;
		default:
			break;
		}

		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = UB_VGA_drawLine(parsedData->x[0], parsedData->y[0], parsedData->x[1], parsedData->y[1],parsedData->width, parsedData->color);
	tok = strtok(NULL, ",");
	return error;
}
/**
 * @brief Fills the Parsedata struct with all the parameters the drawRectangle functions from the ub_vga_lib needs.
 * These are:
 * 	- x[0] (x_lo)
 * 	- y[0] (y_lo)
 * 	- x[1] (x_rb)
 * 	- y[1] (y_rb)
 * 	- color
 * 	After filling the structure it resets the UART and calls the drawRectangle function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t rectangleStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 2: parsedData->x[1] = strtol(tok, NULL, 10);
		break;
		case 3: parsedData->y[1] = strtol(tok, NULL, 10);
		break;
		case 4: parsedData->color =  getColor(tok);
		break;
		//too many arguments.
		case 5: error = 7;
		break;
		default:
			break;
		}

		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = UB_VGA_drawRectangle(parsedData->x[0], parsedData->y[0], parsedData->x[1], parsedData->y[1],   parsedData->color);
	return error;
}

/**
 * @brief Fills the Parsedata struct with all the parameters the drawEllipse functions from the ub_vga_lib needs.
 * These are:
 * 	- x[0] (x_mp)
 * 	- y[0] (y_mp)
 * 	- x[1] (x_radius)
 * 	- y[1] (y_radius)
 * 	- color
 * 	After filling the structure it resets the UART and calls the drawEllipse function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t ellipseStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t counter = 0;
	uint8_t error;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 2: parsedData->x[1] = strtol(tok, NULL, 10);
		break;
		case 3: parsedData->y[1] = strtol(tok, NULL, 10);
		break;
		case 4: parsedData->color = getColor(tok);
		break;
		case 5: error = 7;
		break;
		default:
			break;
		}

		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = UB_VGA_drawEllipse(parsedData->x[0], parsedData->y[0], parsedData->x[1], parsedData->y[1],  parsedData->color);
	return error;
}

/**
 * @brief Fills the Parsedata struct with all the parameters the drawTriangle functions from the ub_vga_lib needs.
 * These are:
 * 	- x[0] (x_one)
 * 	- y[0] (y_one)
 * 	- x[1] (x_two)
 * 	- y[1] (y_two)
 * 	- x[2] (x_three)
 * 	- y[2] (y_three)
 * 	- color
 * 	After filling the structure it resets the UART and calls the drawtriangle function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t triangleStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t counter = 0;
	uint8_t error;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 2: parsedData->x[1] = strtol(tok, NULL, 10);
		break;
		case 3: parsedData->y[1] = strtol(tok, NULL, 10);
		break;
		case 4: parsedData->x[2] = strtol(tok, NULL, 10);
		break;
		case 5: parsedData->y[2] = strtol(tok, NULL, 10);
		break;
		case 6: parsedData->color =  getColor(tok);
		break;
		case 7: error = 7;
		break;
		default:
			break;
		}

		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = UB_VGA_drawTriangle(parsedData->x[0], parsedData->y[0], parsedData->x[1], parsedData->y[1], parsedData->x[2], parsedData->y[2],  parsedData->color);
	return error;
}

/**
 * @brief Fills the Parsedata struct with all the parameters the drawBitmap functions from the ub_vga_lib needs.
 * These are:
 * 	- bitmapNr (nr)
 * 	- x[0] (xp)
 * 	- y[1] (yp)
 * 	After filling the structure it resets the UART and calls the drawBitmap function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t bitmapStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->bitmapNr = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 2: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 3: error = 7;
		break;
		default:
			break;
		}

		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = Draw_Bitmap(parsedData->bitmapNr,parsedData->x[0], parsedData->y[0]);
	return error;
}
/**
 * @brief Fills the Parsedata struct with all the parameters the drawText functions from the ub_vga_lib needs.
 * These are:
 * 	- x[0] (x0)
 * 	- y[0] (y0)
 * 	- text (text)
 * 	- color
 * 	After filling the structure it resets the UART and calls the drawText function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t textStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->x[0] = strtol(tok, NULL, 10);
		break;
		case 1: parsedData->y[0] = strtol(tok, NULL, 10);
		break;
		case 2: strcpy(parsedData->text,tok);
		break;
		case 3: parsedData->color =  getColor(tok);
		break;
		case 4: error = 7;
		break;
		default:
			break;
		}
		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = Draw_Text(parsedData->x[0] , parsedData->y[0],parsedData->text , parsedData->color);
	return error;
}

/**
 * @brief Fills the Parsedata struct with all the parameters the DELAY_ms functions from the ub_vga_lib needs.
 * These are:
 * 	- timeMS (time)
 * 	After filling the structure it the DELAY_ms function from the ub_vga_lib. This function does not reset the UART to allow it te receive new data during the delay.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t delayStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->timeMS = strtol(tok, NULL, 10);
		break;
		case 1: error = 7;
		break;
		default:
			break;
		}
		counter++;
	}
	error = DELAY_ms(parsedData->timeMS);//timeMS
	return error;
}

/**
 * @brief Fills the Parsedata struct with all the parameters the clearscreen functions from the ub_vga_lib needs.
 * These are:
 * 	- color
 * 	After filling the structure it resets the UART and calls the clearscreen function from the ub_vga_lib.
 * @param parsedData: The structure containing data such as x and y coordinates and color to use when drawing shapes from the LIB.
 * @param tok: the string token to split the string on a comma.
 * @retval error value
 */
uint8_t clearscreenStructFiller(struct parsed *parsedData, char* tok)
{
	uint8_t error;
	uint8_t counter = 0;
	while(tok != NULL)
	{
		tok = strtok(NULL, ",");

		switch(counter)
		{
		case 0: parsedData->color =  getColor(tok);
		break;
		case 1: error = 7;
		break;
		default:
			break;
		}
		counter++;
	}
	charcounter = 0;
	memset(string, 0, sizeof string);
	error = UB_VGA_FillScreen(parsedData->color);
	return error;
}

/**
 * @brief This function converts a string into a color value. Accepted string are:
 *  - "rood"
 *  - "blauw"
 *  - "groen"
 *  - "zwart"
 *  - "wit"
 *  - "cyaan"
 *  - "magenta"
 *  - "geel"
 *  - "lichtblauw"
 *  - "lichtgroen"
 *  - "lichtrood"
 *  - "lichtcyaan"
 *  - "lichtmagenta"
 *  - "bruin"
 *  - "grijs"
 *  - "roze"
 *  - "paars"
 * @param color: The color as a string.
 * @retval the color value.
 */
uint8_t getColor(const char* color)
{
	if(strcmp("rood", color)== 0)
		return VGA_COL_RED;
	else if(strcmp("blauw", color)== 0)
		return VGA_COL_BLUE;
	else if(strcmp("groen", color)== 0)
		return VGA_COL_GREEN;
	else if(strcmp("zwart", color)== 0)
		return VGA_COL_BLACK;
	else if(strcmp("wit", color)== 0)
		return VGA_COL_WHITE;
	else if(strcmp("cyaan", color)== 0)
		return VGA_COL_CYAN;
	else if(strcmp("magenta", color)== 0)
		return VGA_COL_MAGENTA;
	else if(strcmp("geel", color)== 0)
		return VGA_COL_YELLOW;
	else if(strcmp("lichtblauw", color)== 0)
		return VGA_COL_LBLUE;
	else if(strcmp("lichtgroen", color)== 0)
		return VGA_COL_LGREEN;
	else if(strcmp("lichtrood", color)== 0)
		return VGA_COL_LRED;
	else if(strcmp("lichtcyaan", color)== 0)
		return VGA_COL_LCYAN;
	else if(strcmp("lichtmagenta", color)== 0)
		return VGA_COL_LMAGENTA;
	else if(strcmp("bruin", color)== 0)
		return VGA_COL_BROWN;
	else if(strcmp("grijs", color)== 0)
		return VGA_COL_GREY;
	else if(strcmp("roze", color)== 0)
		return VGA_COL_PINK;
	else if(strcmp("paars", color)== 0)
		return VGA_COL_PURPLE;
	else return 0;

}
