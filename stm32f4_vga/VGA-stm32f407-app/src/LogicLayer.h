/**
 ***************************************************************
 *@file 	ioLayer.h
 *@author 	Menno Janssen and Benno Driessen
 *@date		19 may 2018
 *@brief	Logic layer of the demo program. This file contains a way to parse the incoming string and call the functions in the VGA_lib to draw the shapes.
 ***************************************************************
 */

#ifndef __LOGICLAYER_H
#define __LOGICLAYER_H


/****************Libraries******************************/
#include "main.h"


/****************Defines********************************/

//Available error codes. This macro turns the integer code into a a string containing the error message.
#define ERROR_CODES(X)                                          			\
    X(0,    E_SUCCESS,            		"No error at all!")         		\
    X(1,   OUT_OF_BOUND,               	"x or y out of bounds")       	 	\
    X(2,   INVALID_COLOR,         		"Invalid color")       				\
    X(3,   INCORRECT_LINE_WIDTH,       	"Line width is incorrect")        	\
    X(4,   RADIUS_TOO_BIG,            	"Radius too big")     				\
    X(5,   BITMAP_NOT_AVAILABLE,  		"Bitmap number does not exist") 	\
	X(6,   INVALID_COMMAND,  			"Invalid command")        			\
	X(7,   TOO_MANY_ARGUMENTS,  		"Too many arguments")       		\
	X(8,   TOO_FEW_ARGUMENTS,  			"Too few arguments")        		\
	X(9,   TIME_NEGATIVE,  				"Time cannot be negative")        	\


#define ERROR_ENUM(ID, NAME, TEXT) NAME = ID,
#define ERROR_TEXT(ID, NAME, TEXT) case ID: return TEXT;

/****************Function Prototypes********************/

//struct for command data. The data is gathered from the UARD and parsed by the parse function.
enum {
    ERROR_CODES(ERROR_ENUM)
};
typedef struct parsed{
	char text[100];
	char* font;
	uint16_t x[3];
	uint16_t y[3];
	uint8_t color;
	uint16_t width;
	uint8_t bitmapNr;
	uint8_t timeMS;
}parser;






#endif// __LOGICLAYER_H
