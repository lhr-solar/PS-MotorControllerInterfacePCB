// Tests all status LEDs on the board
// Test sequence: heartbeat, swoc, hss en, hss fault, otemp, latch, fault, status_leds
// Blinks each status LED once, then thrice all at the same time. 
// failure mode: activate error handler, FAULT LED should toggle rapidly

#include "led.h"
#include "pinDefs.h"
#include "common.h"
#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

// Task parameters
#define LED_TEST_TASK_PRIO       (tskIDLE_PRIORITY + 1)
#define LED_TEST_TASK_STACK     configMINIMAL_STACK_SIZE * 2

// Static buffers
StaticTask_t    LED_Test_Task_Buffer;
StackType_t     LED_Test_Task_Stack[LED_TEST_TASK_STACK];

#define TEST_DELAY_TICKS    pdMS_TO_TICKS(250)

// Array of all LEDs to test
typedef struct {
    uint16_t pin;
    GPIO_TypeDef* port;
} led_entry_t;

static const led_entry_t leds[] = {
    {HSS_FAULT_PIN, HSS_FAULT_PORT},
    {HSS_ENABLE_PIN, HSS_ENABLE_PORT},
    {DEBUG_LED1_PIN, DEBUG_LED1_PORT},
    {DEBUG_LED2_PIN, DEBUG_LED2_PORT},
    {SOFTWARE_OC_PIN, SOFTWARE_OC_PORT},
    {HEARTBEAT_PIN, HEARTBEAT_PORT},
    {OTEMP_PIN, OTEMP_PORT},
    {FAULT_PIN, FAULT_PORT},
};

#define LED_COUNT (sizeof(leds) / sizeof(leds[0]))

void LED_Test_Task(void *pvParameters)
{
    (void)pvParameters;

    // Test all LEDs one by one
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, 1);
        vTaskDelay(TEST_DELAY_TICKS);
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, 0);
        vTaskDelay(TEST_DELAY_TICKS);
    }

    // All LEDs blink together (5 times)
    for (uint8_t i = 0; i < 5; i++)
    {
        for (uint8_t j = 0; j < LED_COUNT; j++)
        {
            HAL_GPIO_WritePin(leds[j].port, leds[j].pin, 1);
        }
        vTaskDelay(TEST_DELAY_TICKS);

        for (uint8_t j = 0; j < LED_COUNT; j++)
        {
            HAL_GPIO_WritePin(leds[j].port, leds[j].pin, 0);
        }
        vTaskDelay(TEST_DELAY_TICKS);
    }
    
    // Task complete - delete itself
    vTaskDelete(NULL);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    LED_Init();
    
    // Create LED Test Task
    xTaskCreateStatic(
        LED_Test_Task,
        "LED Test",
        LED_TEST_TASK_STACK,
        NULL,
        LED_TEST_TASK_PRIO,
        LED_Test_Task_Stack,
        &LED_Test_Task_Buffer
    );
    
    // Start scheduler
    vTaskStartScheduler();

    // Should never reach here
    Error_Handler();
    while (1);
}

