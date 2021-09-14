#ifndef __graph_h
#define __graph_h

#include "stdint.h"
#include "stdbool.h"

void write_graph_color(uint16_t i, uint16_t* buff_draw, uint16_t* buff_clean, uint16_t solid, uint16_t color);
void write_graph_dot_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t solid, uint16_t color);
void write_grid(void);

#endif // __graph_h

