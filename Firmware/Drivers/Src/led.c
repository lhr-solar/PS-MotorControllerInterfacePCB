#include "led.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "common.h"

static const led_t leds[] = {
    // Motor Controller Interface LEDs
    {HEARTBEAT_PIN,        HEARTBEAT_PORT, HEARTBEAT_LED},
    {SOFTWARE_OC_PIN,      SOFTWARE_OC_PORT, 0},
    {HSS_ENABLE_PIN,       HSS_ENABLE_PORT, HSS_FAULT_LED},
    {OTEMP_PIN,            OTEMP_PORT, 0},
    {FAULT_PIN,            FAULT_PORT, FAULT_LED},
    {DEBUG_LED1_PIN,       DEBUG_LED1_PORT, 0},
    {DEBUG_LED2_PIN,       DEBUG_LED2_PORT, 0},

    // PSOM LEDs
    {PSOM_LED1_PIN,        PSOM_LED1_PORT, 0},
    {PSOM_LED2_PIN,        PSOM_LED2_PORT, 0},
    {PSOM_LED3_PIN,        PSOM_LED3_PORT, 0},
    {PSOM_LED4_PIN,        PSOM_LED4_PORT, 0}
};

void LEDs_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (int i = 0; i < ((sizeof(leds))/(sizeof(leds[0]))); i++) {
        GPIO_InitStruct.Pin = leds[i].Pin;
        HAL_GPIO_Init(leds[i].Port, &GPIO_InitStruct);
        HAL_GPIO_WritePin(leds[i].Port, leds[i].Pin, GPIO_PIN_RESET);
    }
}
void statusLEDs_toggle(status_leds_t led)
{
    for (int i = 0; i < ((sizeof(leds))/(sizeof(leds[0]))); i++) {
        if (leds[i].type == led) {
            HAL_GPIO_TogglePin(leds[i].Port, leds[i].Pin);
            return;
        }
    }
}
void statusLEDs_write(status_leds_t led, pin_status_t newState)
{
    for (int i = 0; i < ((sizeof(leds))/(sizeof(leds[0]))); i++) {
        if (leds[i].type == led) {
            HAL_GPIO_WritePin(leds[i].Port, leds[i].Pin, newState);
            return;
        }
    }
}
