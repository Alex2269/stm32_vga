#ifndef __graphics_test_H
#define __graphics_test_H

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

uint16_t random_int(int min, int max);
void lcd_test(void);

#ifdef __cplusplus
}
#endif

#endif // __graphics_test_H
