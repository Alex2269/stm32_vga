#include "adc_cfg.h"

void ADC_StartCalibration(ADC_TypeDef *ADCx)
{
  LL_ADC_Enable(ADCx);
  for (volatile uint8_t i = 128 ;i; i--)  {}
  // while (LL_ADC_IsCalibrationOnGoing(ADCx) != 0) {}
}

void ADC_Init(ADC_TypeDef *ADCx)
{
  LL_RCC_SetRTCClockSource(LL_ADC_CLOCK_SYNC_PCLK_DIV6);
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  // ADC_CommonInitStruct.Multimode = LL_ADC_INJ_TRIG_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADCx), &ADC_CommonInitStruct);
  /** Common config 
  */
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
  LL_ADC_Init(ADCx, &ADC_InitStruct);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = 1;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  LL_ADC_REG_Init(ADCx, &ADC_REG_InitStruct);

  if (ADCx == ADC1)
  {
    // vrefint
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_15CYCLES);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_VREFINT);
  }

  ADC_StartCalibration(ADCx);
}

uint16_t adc_get_value(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  ADCx = ADC1; // for 401 exist only ADC1
  if (ADCx == ADC1)
  {
    LL_ADC_Enable(ADCx);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  }
  if (ADCx == ADC1)
  {
    LL_ADC_Enable(ADCx);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  }

  LL_APB2_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  if(GPIOx == GPIOA)
  {
    if(Pin == LL_GPIO_PIN_0)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_1)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_2)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_2);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_3)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_3);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_4)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_5)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_6)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_6);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_7)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_7);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

  }

  if(GPIOx == GPIOB)
  {
    if(Pin == LL_GPIO_PIN_0)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_8);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

    if(Pin == LL_GPIO_PIN_1)
    {
      LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
      LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_3CYCLES);
    }

  }

  LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_ANALOG);

  if(Pin == 0x10000) // temperature sensor
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_TEMPSENSOR);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_3CYCLES);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
  }

  if(Pin == 0x20000) // vrefint
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_3CYCLES);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_VREFINT);
  }

  uint32_t ADC_value = 0;

  LL_ADC_REG_StartConversionSWStart(ADCx);
  while (LL_ADC_IsActiveFlag_EOCS(ADCx) == 0); // Wait until the conversion is completed
  ADC_value = LL_ADC_REG_ReadConversionData12(ADCx);
  LL_ADC_REG_StopConversionExtTrig(ADCx);

  LL_ADC_Disable(ADCx);
  return ADC_value;
}

uint16_t read_adc(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  #define ADCX_COUNT 4
  uint32_t val = 0;

  for (uint8_t i =0; i<ADCX_COUNT; i++)
  {
    val += adc_get_value(ADCx, GPIOx, Pin);
  }

  val /= ADCX_COUNT;
  return (uint16_t)val * 3300.0 / 4096.0;
}

