// This test controls the HSS enable pin, every 10 seconds the enable pin will toggle
// The HSS_EN LED should indicate, and PP_OUT, BUCK_OUT, and 24V outputs should toggle

#include "led.h"
#include "hss.h"
#include "pinDefs.h"
#include "common.h"
#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

// Task parameters
#define HSS_TEST_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define HSS_TEST_TASK_STACK configMINIMAL_STACK_SIZE * 2

// Static buffers
StaticTask_t HSS_Test_Task_Buffer;
StackType_t HSS_Test_Task_Stack[HSS_TEST_TASK_STACK];

void HSS_Test_Task(void *pvParameters)
{
    (void)pvParameters; // TODO: change in later PR

    HSS_EN_SetState(HSS_ON);
    
    while(1)
    {
        HSS_EN_Toggle();
        vTaskDelay(pdMS_TO_TICKS(10000));
        HSS_EN_Toggle();
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Initialize HSS (creates mutex)
    if (HSS_Init() != pdTRUE) {
        Error_Handler();
    }
    
    LED_Init();

    // Create HSS Test Task
    xTaskCreateStatic(
        HSS_Test_Task,
        "HSS Test",
        HSS_TEST_TASK_STACK,
        NULL,
        HSS_TEST_TASK_PRIO,
        HSS_Test_Task_Stack,
        &HSS_Test_Task_Buffer
    );
    
    // Start scheduler
    vTaskStartScheduler();

    Error_Handler();
    while (1);
}

