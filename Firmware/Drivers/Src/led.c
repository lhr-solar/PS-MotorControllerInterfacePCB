#include "led.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "common.h"

void LEDs_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    // Port A LEDs: LED1 (PA12), HSS_ENABLE (PA4), HSS_FAULT (PA6)
    GPIO_InitStruct.Pin = DEBUG_LED1_PIN | HSS_ENABLE_PIN | HSS_FAULT_PIN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, DEBUG_LED1_PIN | HSS_ENABLE_PIN | HSS_FAULT_PIN, GPIO_PIN_RESET);

    // Port B LEDs: LED2 (PB0), HEARTBEAT (PB6), SOFTWARE_OC (PB7), OTEMP (PB12), LATCH (PB15), FAULT (PB14)
    GPIO_InitStruct.Pin = DEBUG_LED2_PIN | HEARTBEAT_PIN | SOFTWARE_OC_PIN | OTEMP_PIN | HSS_LATCH_PIN | FAULT_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, DEBUG_LED2_PIN | HEARTBEAT_PIN | SOFTWARE_OC_PIN | OTEMP_PIN | HSS_LATCH_PIN | FAULT_PIN, GPIO_PIN_RESET);
}

void statusLEDs_toggle(status_leds_t led)
{
    switch(led){
        case HEARTBEAT_LED:
            HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
            break;
        case FAULT_LED:
            HAL_GPIO_TogglePin(FAULT_PORT, FAULT_PIN);
            break;
        case HSS_FAULT_LED:
            HAL_GPIO_TogglePin(HSS_FAULT_PORT, HSS_FAULT_PIN);
            break;
        default:
            break;
    }
}
void statusLEDs_write(status_leds_t led, pin_status_t newState)
{
        switch(led){
        case HEARTBEAT_LED:
            HAL_GPIO_WritePin(HEARTBEAT_PORT, HEARTBEAT_PIN, newState);
            break;
        case FAULT_LED:
            HAL_GPIO_WritePin(FAULT_PORT, FAULT_PIN, newState);
            break;
        case HSS_FAULT_LED:
            HAL_GPIO_WritePin(HSS_FAULT_PORT, HSS_FAULT_PIN, newState);
            break;
        default:
            break;
    }
}

void canLED_heartbeat(void)
{
    while(1){
    HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
    HAL_Delay(500);
  }
}

