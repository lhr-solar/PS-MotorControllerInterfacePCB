// Tests all status LEDs on the board
// Test sequence: each LED should blink (heartbeat, swoc, hss en, hss fault, otemp, latch, fault, status_leds), after all LEDs should blink thrice at the same time. if fails, then status LEDS PB0 and PA12 should toggle rapidly

#include "led.h"
#include "pinDefs.h"
#include "sysclk.h"
#include "stm32xx_hal.h"

#define TEST_DELAY 250 // ms between LED toggles

// toggles status LEDs rapidly on error
void Error_Handler(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_GPIO_TogglePin(GPIO_A12_PORT, GPIO_A12_PIN);
        HAL_Delay(50);
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Initialize LEDs
    if (HAL_GPIO_Init(LED_PORT, &(GPIO_InitTypeDef){.Pin=LED_PIN, .Mode=GPIO_MODE_OUTPUT_PP, .Pull=GPIO_NOPULL, .Speed=GPIO_SPEED_FREQ_LOW}) != HAL_OK) {
        Error_Handler();
    }
    
    // Test all LEDs one by one
    LED_SetState(HEARTBEAT_PIN, HEARTBEAT_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(HEARTBEAT_PIN, HEARTBEAT_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(SOFTWARE_OC_PIN, SOFTWARE_OC_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(SOFTWARE_OC_PIN, SOFTWARE_OC_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(HSS_FAULT_PIN, HSS_FAULT_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(HSS_FAULT_PIN, HSS_FAULT_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(OTEMP_PIN, OTEMP_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(OTEMP_PIN, OTEMP_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(LATCH_PIN, LATCH_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(LATCH_PIN, LATCH_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(FAULT_PIN, FAULT_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(FAULT_PIN, FAULT_PORT, 0);
    HAL_Delay(TEST_DELAY);

    LED_SetState(LED_PIN, LED_PORT, 1);
    HAL_Delay(TEST_DELAY);
    LED_SetState(LED_PIN, LED_PORT, 0);
    HAL_Delay(TEST_DELAY);

    // All LEDs blink together (3 times)
    for (uint8_t i = 0; i < 3; i++)
    {
        LED_SetState(HEARTBEAT_PIN, HEARTBEAT_PORT, 1);
        LED_SetState(SOFTWARE_OC_PIN, SOFTWARE_OC_PORT, 1);
        LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 1);
        LED_SetState(HSS_FAULT_PIN, HSS_FAULT_PORT, 1);
        LED_SetState(OTEMP_PIN, OTEMP_PORT, 1);
        LED_SetState(LATCH_PIN, LATCH_PORT, 1);
        LED_SetState(FAULT_PIN, FAULT_PORT, 1);
        LED_SetState(LED_PIN, LED_PORT, 1);
        HAL_Delay(200);

        LED_SetState(HEARTBEAT_PIN, HEARTBEAT_PORT, 0);
        LED_SetState(SOFTWARE_OC_PIN, SOFTWARE_OC_PORT, 0);
        LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 0);
        LED_SetState(HSS_FAULT_PIN, HSS_FAULT_PORT, 0);
        LED_SetState(OTEMP_PIN, OTEMP_PORT, 0);
        LED_SetState(LATCH_PIN, LATCH_PORT, 0);
        LED_SetState(FAULT_PIN, FAULT_PORT, 0);
        LED_SetState(LED_PIN, LED_PORT, 0);
        HAL_Delay(200);
    }

    return 0;
}

