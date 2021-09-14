#ifndef __DWT_IMPL_H
#define __DWT_IMPL_H

#if DEBUG_STATISTIC_EN


#include "stm32h7xx.h"

#define DWT_ENABLE()                        (DWT->CTRL |= (1UL))
#define DWT_DISABLE()                       (DWT->CTRL &= (~1UL))
#define DWT_GET_CYCCNT()                    ((unsigned int)(DWT->CYCCNT))
#define CPU_COST_NANOSEC(begin, end)        ((end - begin) * (1000000000 / 480000000))

#else

#define DWT_ENABLE()
#define DWT_DISABLE()
#define DWT_GET_CYCCNT()                    (0)
#define CPU_COST_NANOSEC(begin, end)        (0)

#endif

#endif
