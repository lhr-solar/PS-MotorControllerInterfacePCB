#include "motor_task.h"
#include "common.h"
#include "stm32xx_hal.h"
#include "Debugging.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    initPrintf();
    
    // Initialize motor task and create RTOS task
    init_status_t init_status = Motor_Task_Init();
    if (init_status != INIT_OK) {
        Error_Handler();
    }
    
    // Start the RTOS scheduler
    vTaskStartScheduler();

    // Unreachable
    Error_Handler();
    
    while (1);
}

