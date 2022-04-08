#ifndef __STM32F4XX_LL_DMA_MIX_H
#define __STM32F4XX_LL_DMA_MIX_H

#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_dma_flags.h"

/* Set Configuration register CR. */
static inline
void LL_DMA_SetRegiter_CR(DMA_TypeDef *DMAx, uint32_t Stream, uint32_t Configuration)
{
  if(Stream > 7) return;

  WRITE_REG(((DMA_Stream_TypeDef *)((uint32_t)((uint32_t)DMAx + STREAM_OFFSET_TAB[Stream])))->CR, Configuration);
}

/* Get Configuration register CR. */
static inline
uint32_t LL_DMA_GetRegiter_CR(DMA_TypeDef* DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  return (READ_REG(((DMA_Stream_TypeDef*)((uint32_t)((uint32_t)DMAx + STREAM_OFFSET_TAB[Stream])))->CR));
}

#endif // __STM32F4XX_LL_DMA_MIX_H
