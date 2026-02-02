#include "motor_task.h"
#include "pwm.h"
#include "sysclk.h"
#include "stm32xx_hal.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MotorTask_Init();

    while(1)
    {
        MotorTask_Run();
    }
}

