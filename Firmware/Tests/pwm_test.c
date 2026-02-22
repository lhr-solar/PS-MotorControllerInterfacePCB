// Tests Fan A and Fan B functionality
// Runs loop of duty cycles starting from 0%, 25%, 50%, 75%, 100%. LED1 turns on if code is active

#include "stm32xx_hal.h"
#include "pwm.h"
#include "led.h"
#include "common.h"
#include "motor_task.h"
#include "FreeRTOS.h"
#include "task.h"

// Task parameters
#define PWM_TEST_TASK_PRIO       (tskIDLE_PRIORITY + 1)
#define PWM_TEST_TASK_STACK     configMINIMAL_STACK_SIZE * 2

// Static buffers
StaticTask_t    PWM_Test_Task_Buffer;
StackType_t     PWM_Test_Task_Stack[PWM_TEST_TASK_STACK];

// Test duty cycles
static const uint8_t duty_cycles[] = {0, 25, 50, 75, 100};
#define NUM_TESTS (sizeof(duty_cycles) / sizeof(duty_cycles[0]))

void PWM_Test_Task(void *pvParameters)
{
    (void)pvParameters;
    
    uint8_t test_index = 0;
    
    while (1) 
    {
        // Cycle through duty cycles
        PWM1_SetDuty(duty_cycles[test_index]);  // PB10
        PWM2_SetDuty(duty_cycles[test_index]);  // PA5 
        
        // Toggle debug LED to show task is running
        LED_Debug_Toggle();
        
        // Move to next duty cycle
        test_index = (test_index + 1) % NUM_TESTS;
        
        // Delay 1 second between changes
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    
    // Initialize PWM outputs
    PWM1_Init(pwm1TimHandle);  
    PWM2_Init(pwm2TimHandle);  
    
    // Initialize LEDs
    LED_Init();
    
    // Create PWM Test Task
    xTaskCreateStatic(
        PWM_Test_Task,
        "PWM Test",
        PWM_TEST_TASK_STACK,
        NULL,
        PWM_TEST_TASK_PRIO,
        PWM_Test_Task_Stack,
        &PWM_Test_Task_Buffer
    );
    
    // Start scheduler
    vTaskStartScheduler();

    // Should never reach here
    Error_Handler();
    while (1);
}

