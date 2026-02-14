// This test controls the HSS enable pin, every 10 seconds the enable pin will toggle
// The HSS_EN LED should indicate, and PP_OUT, BUCK_OUT, and 24V outputs should toggle

// test validated! ✅

#include "led.h"
#include "pinDefs.h"
#include "sysclk.h"
#include "stm32xx_hal.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    LED_Init();

    while(1)
    {
        LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 1);

        /* 
        HAL_Delay(10000);
        LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 0);
        HAL_Delay(10000);
        */
    }

    return 0;
}