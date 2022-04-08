#ifndef __STM32F4xx_LL_DMA_FLAGS_H
#define __STM32F4xx_LL_DMA_FLAGS_H

#include "stm32f4xx_ll_dma.h"

/* Clear Stream x half transfer flag. */
static inline
void LL_DMA_ClearFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LIFCR_CHTIF0, DMA_LIFCR_CHTIF1, DMA_LIFCR_CHTIF2, DMA_LIFCR_CHTIF3,
    DMA_HIFCR_CHTIF4, DMA_HIFCR_CHTIF5, DMA_HIFCR_CHTIF6, DMA_HIFCR_CHTIF7
  };

  if(Stream < 4)
    WRITE_REG(DMAx->LIFCR , flag_t[Stream]);
  else
    WRITE_REG(DMAx->HIFCR , flag_t[Stream]);
}

/* Clear Stream x transfer complete flag. */
static inline
void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LIFCR_CTCIF0, DMA_LIFCR_CTCIF1, DMA_LIFCR_CTCIF2, DMA_LIFCR_CTCIF3,
    DMA_HIFCR_CTCIF4, DMA_HIFCR_CTCIF5, DMA_HIFCR_CTCIF6, DMA_HIFCR_CTCIF7
  };

  if(Stream < 4)
    WRITE_REG(DMAx->LIFCR , flag_t[Stream]);
  else
    WRITE_REG(DMAx->HIFCR , flag_t[Stream]);
}

/* Clear Stream x transfer error flag. */
static inline
void LL_DMA_ClearFlag_TE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LIFCR_CTEIF0, DMA_LIFCR_CTEIF1, DMA_LIFCR_CTEIF2, DMA_LIFCR_CTEIF3,
    DMA_HIFCR_CTEIF4, DMA_HIFCR_CTEIF5, DMA_HIFCR_CTEIF6, DMA_HIFCR_CTEIF7
  };

  if(Stream < 4)
    WRITE_REG(DMAx->LIFCR , flag_t[Stream]);
  else
    WRITE_REG(DMAx->HIFCR , flag_t[Stream]);
}

/* Clear Stream x direct mode error flag. */
static inline
void LL_DMA_ClearFlag_DME(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LIFCR_CDMEIF0, DMA_LIFCR_CDMEIF1, DMA_LIFCR_CDMEIF2, DMA_LIFCR_CDMEIF3,
    DMA_HIFCR_CDMEIF4, DMA_HIFCR_CDMEIF5, DMA_HIFCR_CDMEIF6, DMA_HIFCR_CDMEIF7
  };

  if(Stream < 4)
    WRITE_REG(DMAx->LIFCR , flag_t[Stream]);
  else
    WRITE_REG(DMAx->HIFCR , flag_t[Stream]);
}

/* Clear Stream x FIFO error flag. */
static inline
void LL_DMA_ClearFlag_FE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LIFCR_CFEIF0, DMA_LIFCR_CFEIF1, DMA_LIFCR_CFEIF2, DMA_LIFCR_CFEIF3,
    DMA_HIFCR_CFEIF4, DMA_HIFCR_CFEIF5, DMA_HIFCR_CFEIF6, DMA_HIFCR_CFEIF7
  };

  if(Stream < 4)
    WRITE_REG(DMAx->LIFCR , flag_t[Stream]);
  else
    WRITE_REG(DMAx->HIFCR , flag_t[Stream]);
}

/* Get Stream x half transfer flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LISR_HTIF0, DMA_LISR_HTIF1, DMA_LISR_HTIF2, DMA_LISR_HTIF3,
    DMA_HISR_HTIF4, DMA_HISR_HTIF5, DMA_HISR_HTIF6, DMA_HISR_HTIF7
  };

  uint32_t flag = flag_t[Stream];

  if(Stream < 4)
    return (READ_BIT(DMAx->LISR ,flag)==(flag));
  else
    return (READ_BIT(DMAx->HISR ,flag)==(flag));
}

/* Get Stream x transfer complete flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LISR_TCIF0, DMA_LISR_TCIF1, DMA_LISR_TCIF2, DMA_LISR_TCIF3,
    DMA_HISR_TCIF4, DMA_HISR_TCIF5, DMA_HISR_TCIF6, DMA_HISR_TCIF7
  };

  uint32_t flag = flag_t[Stream];

  if(Stream < 4)
    return (READ_BIT(DMAx->LISR ,flag)==(flag));
  else
    return (READ_BIT(DMAx->HISR ,flag)==(flag));
}

/* Get Stream x transfer error flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_TE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LISR_TEIF0, DMA_LISR_TEIF1, DMA_LISR_TEIF2, DMA_LISR_TEIF3,
    DMA_HISR_TEIF4, DMA_HISR_TEIF5, DMA_HISR_TEIF6, DMA_HISR_TEIF7
  };

  uint32_t flag = flag_t[Stream];

  if(Stream < 4)
    return (READ_BIT(DMAx->LISR ,flag)==(flag));
  else
    return (READ_BIT(DMAx->HISR ,flag)==(flag));
}

/* Get Stream x direct mode error flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_DME(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LISR_DMEIF0, DMA_LISR_DMEIF1, DMA_LISR_DMEIF2, DMA_LISR_DMEIF3,
    DMA_HISR_DMEIF4, DMA_HISR_DMEIF5, DMA_HISR_DMEIF6, DMA_HISR_DMEIF7
  };

  uint32_t flag = flag_t[Stream];

  if(Stream < 4)
    return (READ_BIT(DMAx->LISR ,flag)==(flag));
  else
    return (READ_BIT(DMAx->HISR ,flag)==(flag));
}

/* Get Stream x FIFO error flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_FE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if(Stream > 7) return;

  uint32_t flag_t[8] =
  {
    DMA_LISR_FEIF0, DMA_LISR_FEIF1, DMA_LISR_FEIF2, DMA_LISR_FEIF3,
    DMA_HISR_FEIF4, DMA_HISR_FEIF5, DMA_HISR_FEIF6, DMA_HISR_FEIF7
  };

  uint32_t flag = flag_t[Stream];

  if(Stream < 4)
    return (READ_BIT(DMAx->LISR ,flag)==(flag));
  else
    return (READ_BIT(DMAx->HISR ,flag)==(flag));
}

#endif // __STM32F4xx_LL_DMA_FLAGS_H
