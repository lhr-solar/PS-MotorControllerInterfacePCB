
#include "motor_task.h"
#include "pwm.h"
#include "led.h"

TIM_HandleTypeDef pwm1TimHandle = {0};  // PB10
TIM_HandleTypeDef pwm2TimHandle = {0};  // PA5

void Init_Task(void)
{
    // Initialize both PWM outputs
    PWM1_Init(&pwm1TimHandle);
    PWM2_Init(&pwm2TimHandle);  
    
    LED_Init();
}

void Run_Task(void)
{
    // Sets duty cycles for the PWMs
    PWM1_SetDuty(40);
    PWM2_SetDuty(60); 
    
    LED_Toggle(); // hardware verification
}

