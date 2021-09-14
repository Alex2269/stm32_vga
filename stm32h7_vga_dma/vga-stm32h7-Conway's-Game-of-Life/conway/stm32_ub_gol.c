//--------------------------------------------------------------
// File     : stm32_ub_gol.c
// Datum    : 08.01.2015
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : LCD,DMA2D
// Funktion : Conway's Game-Of-Life
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_gol.h"

uint8_t CELL[CELL_ANZ_X][CELL_ANZ_Y];

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
uint8_t p_countNeighbours(uint32_t x, uint32_t y);



//--------------------------------------------------------------
void GOL_Init(void)
{
	uint32_t x,y;

	for(y=0;y<CELL_ANZ_Y;y++) {
		for(x=0;x<CELL_ANZ_X;x++) {
			CELL[x][y]=0x00;
		}
	}
}

//--------------------------------------------------------------
void GOL_SetCellsRandom(void)
{
	uint32_t x,y;
	uint8_t value;

	for(y=0;y<CELL_ANZ_Y;y++) {
		for(x=0;x<CELL_ANZ_X;x++) {
			value=rand16()%(2);  // 0 oder 1
			if(value==0) {
				CELL[x][y]=0x00;
			}
			else {
				CELL[x][y]=0x03;
			}
		}
	}
}

//--------------------------------------------------------------
void GOL_ShowAllCells(void)
{
	uint32_t x,y;
	uint16_t xpos=0,ypos=0;

	for(y=0;y<CELL_ANZ_Y;y++) {
		for(x=0;x<CELL_ANZ_X;x++) {
			if((CELL[x][y]&0x01)==0) {
				fillRect(xpos,ypos,CELL_SIZE_X,CELL_SIZE_Y,CELL_COL_DEAD);
			}
			else {
                uint8_t color = rand16();
				fillRect(xpos,ypos,CELL_SIZE_X,CELL_SIZE_Y, color ? color : CELL_COL_ALIVE );
			}
			xpos+=CELL_SIZE_X;
		}
		xpos=0;
		ypos+=CELL_SIZE_Y;
	}
}

//--------------------------------------------------------------
void GOL_UpdateAllCells(void)
{
	uint32_t x,y;
	uint8_t neighbours;

	// check alive status
	for(y=0;y<CELL_ANZ_Y;y++) {
		for(x=0;x<CELL_ANZ_X;x++) {
			neighbours=p_countNeighbours(x,y);
			// GOL rules :
			// 0,1 => dead
			// 2   => keep status
			// 3   => alive
			// 4-8 => dead
			if((neighbours<2) || (neighbours>3)) CELL[x][y]&=0xFD; // dead
			if(neighbours==3) CELL[x][y]|=0x02; // alive

		}
	}

	// copy alive status
	for(y=0;y<CELL_ANZ_Y;y++) {
		for(x=0;x<CELL_ANZ_X;x++) {
			if((CELL[x][y]&0x02)==0) {
				CELL[x][y]=0x00;
			}
			else {
				CELL[x][y]=0x03;
			}
		}
	}

}


//--------------------------------------------------------------
uint8_t p_countNeighbours(uint32_t x, uint32_t y)
{
	uint8_t ret_wert=0;
	int16_t x_lo=0,y_lo=0;
	int16_t x_hi=0,y_hi=0;

	// check borders
	if(x>0) x_lo=x-1; else x_lo=CELL_ANZ_X-1;
	if(y>0) y_lo=y-1; else y_lo=CELL_ANZ_Y-1;
	if(x<(CELL_ANZ_X-1)) x_hi=x+1; else x_hi=0;
	if(y<(CELL_ANZ_Y-1)) y_hi=y+1; else y_hi=0;

    // check all 8 neighbours
	if((CELL[x_lo][y_lo]&0x01)!=0) ret_wert++;
	if((CELL[x_lo][y]&0x01)!=0) ret_wert++;
	if((CELL[x_lo][y_hi]&0x01)!=0) ret_wert++;
	if((CELL[x][y_lo]&0x01)!=0) ret_wert++;
	if((CELL[x][y_hi]&0x01)!=0) ret_wert++;
	if((CELL[x_hi][y_lo]&0x01)!=0) ret_wert++;
	if((CELL[x_hi][y]&0x01)!=0) ret_wert++;
	if((CELL[x_hi][y_hi]&0x01)!=0) ret_wert++;

	return(ret_wert);
}

