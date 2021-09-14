#ifndef __STARTUP_H
#define __STARTUP_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "usbd_def.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

#ifdef __cplusplus
extern "C" {
#endif

extern USBD_HandleTypeDef hUsbDeviceFS;
extern RTC_HandleTypeDef hrtc;

void initialize(void);
void setup(void);
void loop(void);

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif

