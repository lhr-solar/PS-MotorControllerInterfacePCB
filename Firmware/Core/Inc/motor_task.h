#pragma once
#include "stm32xx_hal.h"
void Init_Task(void);  // initializes motor task
void Run_Task(void); // runs motor task

extern TIM_HandleTypeDef pwm1TimHandle;
extern TIM_HandleTypeDef pwm2TimHandle;
