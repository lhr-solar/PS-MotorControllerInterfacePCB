#pragma once
#ifndef PINDEFS_H
#define PINDEFS_H

// ===== PWM CONFIGURATION =====

#define PWM_TIMER_INSTANCE TIM2
#define PWM_TIMER_CHANNEL TIM_CHANNEL_3

#define PWM_GPIO_PORT GPIOA
#define PWM_GPIO_PIN GPIO_PIN_2
#define PWM_GPIO_AF GPIO_AF1_TIM2

#define PWM_TIMER_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE()
#define PWM_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define PWM_PULSE 500;

#define PWM_TIMER_CLK_HZ 80000000UL

#define PWM_PERIOD 999 // 1 MHz / (999 + 1) = 1 kHz PWM
#define PWM_PRESCALER 79 // 80 MHz / (79 + 1) = 1 MHz timer tick

// Timer tick = 1/1MHz = 1µs
// PWM freq = 1 kHz

#define PWM_MAX_COUNT (PWM_PERIOD + 1)

#endif