#include "motor_task.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"
#include "common.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

void Motor_Task_Init(void)
{
    // Initialize HSS (creates mutex)
    if (HSS_Init() != pdTRUE) {
        Error_Handler();
    }
    
    // Initialize both PWM outputs
    PWM1_Init(pwm1TimHandle);
    PWM2_Init(pwm2TimHandle);  
    
    // Initialize LEDs
    LED_Init();

    // Create the Motor Task
    xTaskCreateStatic(
        Motor_Task,              // Task function
        "Motor Task",            // Task name
        MOTOR_TASK_STACK_SIZE,   // Stack size
        NULL,                    // Parameters
        MOTOR_TASK_PRIO,         // Priority
        Motor_Task_Stack,        // Stack buffer
        &Motor_Task_Buffer       // TCB buffer
    );
}

void Motor_Task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        // Sets duty cycles for the PWMs 
        PWM1_SetDuty(70);
        PWM2_SetDuty(70); 
        
        // Blink HEARTBEAT LED to show working
        // Success_Handler();

        // Enable HSS circuit 
        HSS_EN_SetState(HSS_ON);

        // Delay for task period (10ms = 100Hz)
        vTaskDelay(MOTOR_TASK_PERIOD);
    }
}

