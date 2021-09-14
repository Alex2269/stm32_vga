#ifndef VGA_DRAW_H
#define VGA_DRAW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void vgaDraw(volatile uint8_t* bitmap, volatile uint16_t* colors, uint8_t* dest);

#ifdef __cplusplus
}
#endif

#endif  // DRAW4_H
