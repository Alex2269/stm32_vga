#ifndef __clock_H
#define __clock_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

#include "stm32_adafruit_vga.h"

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

// uint8_t conv2d(const char* p);
void clock(void);

#ifdef __cplusplus
}
#endif

#endif // __clock_H
