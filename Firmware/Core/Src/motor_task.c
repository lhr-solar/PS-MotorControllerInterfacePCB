#include "motor_task.h"
#include "pwm.h"
#include "led.h"

void MotorTask_Init(void)
{
    PWM_Init();
    LED_Init();
}

void MotorTask_Run(void)
{
    while (1)
    {
        PWM_SetDuty(40);
        HAL_Delay(10);
        LED_Toggle();
    }
}
