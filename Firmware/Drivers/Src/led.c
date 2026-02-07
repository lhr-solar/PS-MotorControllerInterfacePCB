#include "led.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

#define LED_TOGGLE_DELAY 500 // ms

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure all status LED pins as outputs
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    // Status LED (used for hardware verification)
    GPIO_InitStruct.Pin = LED_PIN;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    // HEARTBEAT LED (PB6)
    GPIO_InitStruct.Pin = HEARTBEAT_PIN;
    HAL_GPIO_Init(HEARTBEAT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(HEARTBEAT_PORT, HEARTBEAT_PIN, GPIO_PIN_RESET);

    // SOFTWARE_OC LED (PB7)
    GPIO_InitStruct.Pin = SOFTWARE_OC_PIN;
    HAL_GPIO_Init(SOFTWARE_OC_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SOFTWARE_OC_PORT, SOFTWARE_OC_PIN, GPIO_PIN_RESET);

    // HSS_ENABLE LED (PA4)
    GPIO_InitStruct.Pin = HSS_ENABLE_PIN;
    HAL_GPIO_Init(HSS_ENABLE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, GPIO_PIN_RESET);

    // HSS_FAULT LED (PA6)
    GPIO_InitStruct.Pin = HSS_FAULT_PIN;
    HAL_GPIO_Init(HSS_FAULT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(HSS_FAULT_PORT, HSS_FAULT_PIN, GPIO_PIN_RESET);

    // OTEMP LED (PB12)
    GPIO_InitStruct.Pin = OTEMP_PIN;
    HAL_GPIO_Init(OTEMP_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(OTEMP_PORT, OTEMP_PIN, GPIO_PIN_RESET);

    // LATCH LED (PB15)
    GPIO_InitStruct.Pin = LATCH_PIN;
    HAL_GPIO_Init(LATCH_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, GPIO_PIN_RESET);

    // FAULT LED (PB14)
    GPIO_InitStruct.Pin = FAULT_PIN;
    HAL_GPIO_Init(FAULT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(FAULT_PORT, FAULT_PIN, GPIO_PIN_RESET);
}

void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(LED_TOGGLE_DELAY);
}

void LED_SetState(uint16_t pin, GPIO_TypeDef* port, uint8_t state)
{
    HAL_GPIO_WritePin(port, pin, (GPIO_PinState)state);
}

void LED_TogglePin(uint16_t pin, GPIO_TypeDef* port)
{
    HAL_GPIO_TogglePin(port, pin);
}

