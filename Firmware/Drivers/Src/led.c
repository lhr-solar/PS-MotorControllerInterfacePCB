#include "led.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

#define LED_TOGGLE_DELAY 500 // 500 ms

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    // Port A LEDs: LED2 (PA12), HSS_ENABLE (PA4), HSS_FAULT (PA6)
    GPIO_InitStruct.Pin = LED2_PIN | HSS_ENABLE_PIN | HSS_FAULT_PIN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, LED2_PIN | HSS_ENABLE_PIN | HSS_FAULT_PIN, GPIO_PIN_RESET);

    // Port B LEDs: LED1 (PB0), HEARTBEAT (PB6), SOFTWARE_OC (PB7), OTEMP (PB12), LATCH (PB15), FAULT (PB14)
    GPIO_InitStruct.Pin = LED1_PIN | HEARTBEAT_PIN | SOFTWARE_OC_PIN | OTEMP_PIN | HSS_LATCH_PIN | FAULT_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, LED1_PIN | HEARTBEAT_PIN | SOFTWARE_OC_PIN | OTEMP_PIN | HSS_LATCH_PIN | FAULT_PIN, GPIO_PIN_RESET);
}
/*
void LED_Debug_Toggle(void) // Hardware verification debug LEDs
{
    HAL_GPIO_TogglePin(DEBUG_LED1_PORT, DEBUG_LED1_PIN);
    HAL_GPIO_TogglePin(DEBUG_LED2_PORT, DEBUG_LED2_PIN);

    HAL_Delay(LED_TOGGLE_DELAY);
}

void LED_SetState(uint16_t pin, GPIO_TypeDef* port, uint8_t state) // LED helper function 1
{
    HAL_GPIO_WritePin(port, pin, state);
}

void LED_TogglePin(uint16_t pin, GPIO_TypeDef* port) // LED helper function 2
{
    HAL_GPIO_TogglePin(port, pin);
}

*/