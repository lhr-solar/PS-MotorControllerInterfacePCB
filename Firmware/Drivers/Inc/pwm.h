#pragma once
#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef pwmTimHandle;

void PWM_Init(void);
void PWM_SetDuty(uint8_t duty);
