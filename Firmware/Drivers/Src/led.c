#include "led.h"
#include "stm32xx_hal.h"

#define LED_TOGGLE_DELAY 500 // ms

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(LED_TOGGLE_DELAY);
}

