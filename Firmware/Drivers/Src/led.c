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
    static const led_t leds[] = {
    {HEARTBEAT_PIN,        HEARTBEAT_PORT},
    {SOFTWARE_OC_PIN,      SOFTWARE_OC_PORT},
    {HSS_ENABLE_PIN,       HSS_ENABLE_PORT},
    {HSS_FAULT_PIN,        HSS_FAULT_PORT},    // Fixed: was HSS_ENABLE_PIN
    {OTEMP_PIN,            OTEMP_PORT},
    {HSS_LATCH_PIN,        HSS_LATCH_PORT},    // Fixed: was HSS_ENABLE_PIN
    {FAULT_PIN,            FAULT_PORT},
    {DEBUG_LED1_PIN,       DEBUG_LED1_PORT},
    {DEBUG_LED2_PIN,       DEBUG_LED2_PORT}
};

    for (int i = 0, i < ((sizeof(leds))/(sizeof(leds[0]))), i++) {
        GPIO_InitStruct.Pin = leds[i].Pin;
        HAL_GPIO_Init(leds[i].Port, &GPIO_InitStruct);
        HAL_GPIO_WritePin(leds[i].Port, leds[i].Pin, GPIO_PIN_RESET);
    }
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
