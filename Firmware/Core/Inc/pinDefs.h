// Port Configurations (PWM1, PWM2, Debug LEDs, Status LEDs, TACHs)
#pragma once
#include "stm32xx_hal.h"

// PWM1 configuration PB10 (TIM2_CH3)
#define PWM1_TIMER_INSTANCE TIM2
#define PWM1_TIMER_CHANNEL TIM_CHANNEL_3

#define PWM1_GPIO_PORT GPIOB
#define PWM1_GPIO_PIN GPIO_PIN_10
#define PWM1_GPIO_AF GPIO_AF1_TIM2

// PWM2 configuration PA5 (TIM2_CH1)
#define PWM2_TIMER_INSTANCE TIM2
#define PWM2_TIMER_CHANNEL TIM_CHANNEL_1

#define PWM2_GPIO_PORT GPIOA
#define PWM2_GPIO_PIN GPIO_PIN_5
#define PWM2_GPIO_AF GPIO_AF1_TIM2

// Status LEDs
typedef struct {
    uint16_t Pin;
    GPIO_TypeDef *Port;
} led_t;

// PB6
#define HEARTBEAT_PIN GPIO_PIN_6
#define HEARTBEAT_PORT GPIOB

// PB7
#define SOFTWARE_OC_PIN GPIO_PIN_7
#define SOFTWARE_OC_PORT GPIOB

// PA4
#define HSS_ENABLE_PIN GPIO_PIN_4
#define HSS_ENABLE_PORT GPIOA

// PA6
#define HSS_FAULT_PIN GPIO_PIN_6
#define HSS_FAULT_PORT GPIOA

// PB12
#define OTEMP_PIN GPIO_PIN_12
#define OTEMP_PORT GPIOB

// PB15
#define HSS_LATCH_PIN GPIO_PIN_15
#define HSS_LATCH_PORT GPIOB

// PB14
#define FAULT_PIN GPIO_PIN_14
#define FAULT_PORT GPIOB

// PB0
#define DEBUG_LED1_PIN GPIO_PIN_0
#define DEBUG_LED1_PORT GPIOB

// PA12
#define DEBUG_LED2_PIN GPIO_PIN_12
#define DEBUG_LED2_PORT GPIOA

// BUCK SENSE (PA2)
#define BUCK_SENSE_PIN GPIO_PIN_2
#define BUCK_SENSE_PORT GPIOA

// Tach Input Configurations (timer input capture)

// PB3 TIM2_CH2
#define FAN_A_TACH_PIN        GPIO_PIN_3
#define FAN_A_TACH_PORT       GPIOB
#define FAN_A_TACH_AF         GPIO_AF1_TIM2 

// PA11 TIM1_CH4
#define FAN_B_TACH_PIN        GPIO_PIN_11
#define FAN_B_TACH_PORT       GPIOA
#define FAN_B_TACH_AF         GPIO_AF1_TIM1 

