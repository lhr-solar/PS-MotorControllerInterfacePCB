#pragma once
#include "stm32l4xx_hal.h"

// PWM1 Functions
void PWM1_Init(TIM_HandleTypeDef* timHandle);
void PWM1_SetDuty(uint8_t duty);

// PWM2 Functions  
void PWM2_Init(TIM_HandleTypeDef* timHandle);
void PWM2_SetDuty(uint8_t duty);

// Shared PWM configurations (for now)
#define PWM_PULSE 500 
#define PWM_TIMER_CLK_HZ 80000000UL
#define PWM_PERIOD 999 // 1 MHz / (999 + 1) = 1 kHz PWM
#define PWM_PRESCALER 79 // 80 MHz / (79 + 1) = 1 MHz timer tick
#define PWM_MAX_COUNT (PWM_PERIOD + 1)
