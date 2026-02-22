#pragma once
#include "stm32xx_hal.h"

// PWM1 GPIO Configurations
#define PWM1_TIMER_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE()
#define PWM1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()


// PWM2 GPIO Configurations
#define PWM2_TIMER_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE() 
#define PWM2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()