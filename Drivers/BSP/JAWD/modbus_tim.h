#ifndef __MODBUS_TIM_H__
#define __MODBUS_TIM_H__


#include "stm32f4xx_hal.h"
#include "EventRecorder.h"


void bsp_InitHardTimer(void);
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);


#endif
