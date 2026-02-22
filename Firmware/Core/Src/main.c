#include "motor_task.h"
#include "common.h"
#include "stm32xx_hal.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Initialize motor task and create RTOS task
    Motor_Task_Init();
    
    // Start the RTOS scheduler
    vTaskStartScheduler();

    Error_Handler();
    while (1);
}

