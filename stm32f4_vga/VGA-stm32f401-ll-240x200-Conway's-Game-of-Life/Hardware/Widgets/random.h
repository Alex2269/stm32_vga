#ifndef __common_H
#define __common_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stdint.h"

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

uint16_t random_int(int min, int max)
{
  return (uint16_t)rand() % max + min;
}

#ifdef __cplusplus
}
#endif

#endif // __common_H
