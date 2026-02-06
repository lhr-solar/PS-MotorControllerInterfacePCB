
#include "motor_task.h"
#include "sysclk.h"
#include "stm32xx_hal.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    Init_Task();

    while(1)
    {
        Run_Task();
    }
}

void Error_Handler(void)
{
  while (1)
  {
    // baka
  }
}

