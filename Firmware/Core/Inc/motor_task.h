#pragma once
#include "stm32xx_hal.h"

void Init_Task(void);
void Run_Task(void); 

// Extern extends visibility across src files #learning
extern TIM_HandleTypeDef pwm1TimHandle;
extern TIM_HandleTypeDef pwm2TimHandle;
