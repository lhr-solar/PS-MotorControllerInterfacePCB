#include "motor_task.h"
#include "sysclk.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

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

void Error_Handler(void) //if called, will flash LED1 and LED2 rapidly
{
  while (1) 
  {
    HAL_GPIO_TogglePin(LED1_PORT, LED1_PIN);
    HAL_GPIO_TogglePin(LED2_PORT, LED2_PIN);
    HAL_Delay(50);
  }
}


