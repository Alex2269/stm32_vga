#ifndef __gauge_H
#define __gauge_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

#include "stm32_adafruit_vga.h"

struct
{
  float pointX[400];
  float pointY[400];
} dev;

void compute_arrow(void);
uint8_t char2digit(const char* p);
void GaugeCircleSector(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t rotate, uint16_t color, uint16_t val);

#ifdef __cplusplus
}
#endif

#endif // __gauge_H
