#pragma once
#include "stm32xx_hal.h"

/**
 * @brief Initialize PWM1
 * @param timHandle Pointer to addresses of pwm.c static variables
 * @note If failure, FAULT led will blink
 */
void PWM1_Init(TIM_HandleTypeDef* timHandle);

/**
 * @brief Set the duty cycle of PWM1
 * @param duty duty cycle % value
 */
void PWM1_SetDuty(uint8_t duty);

/**
 * @brief Initialize PWM2
 * @param timHandle Pointer to addresses of pwm.c static variables
 * @note If failure, FAULT led will blink
 */
void PWM2_Init(TIM_HandleTypeDef* timHandle);

/**
 * @brief Set the duty cycle of PWM2
 * @param duty duty cycle % value
 */
void PWM2_SetDuty(uint8_t duty);

// Pointers to pwm handles defined in pwm.c
extern TIM_HandleTypeDef *pwm1TimHandle;
extern TIM_HandleTypeDef *pwm2TimHandle;

// Shared PWM configurations (for now)
#define PWM_PULSE 500 
#define PWM_TIMER_CLK_HZ 80000000UL
#define PWM_PERIOD 999 // 1 MHz / (999 + 1) = 1 kHz PWM
#define PWM_PRESCALER 79 // 80 MHz / (79 + 1) = 1 MHz timer tick
#define PWM_MAX_COUNT (PWM_PERIOD + 1)

// PWM1 GPIO Configurations
#define PWM1_TIMER_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE()
#define PWM1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM1_GPIO_PORT GPIOB
#define PWM1_GPIO_PIN GPIO_PIN_10

// PWM2 GPIO Configurations
#define PWM2_TIMER_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE()
#define PWM2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define PWM2_GPIO_PORT GPIOA
#define PWM2_GPIO_PIN GPIO_PIN_5
