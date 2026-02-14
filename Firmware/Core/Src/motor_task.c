#include "motor_task.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"

TIM_HandleTypeDef pwm1TimHandle = {0};  // PB10 TIM2_CH3
TIM_HandleTypeDef pwm2TimHandle = {0};  // PA5 TIM2_CH1

void Init_Task(void)
{
    // Initialize both PWM outputs
    PWM1_Init(&pwm1TimHandle);
    PWM2_Init(&pwm2TimHandle);  
    
    // Initialize LEDs
    LED_Init();
}

void Run_Task(void)
{
    // Sets duty cycles for the PWMs
    PWM1_SetDuty(70);
    PWM2_SetDuty(70); 
    
    // If flashed properly and reset, debug LEDs PB0 and PA12 will toggle
    LED_Debug_Toggle();

    // Enable HSS circuit
    LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 1);
}

