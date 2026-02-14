#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "hss.h" 
#include "led.h"

void HSS_EN_Toggle(void)
{
    LED_SetState(HSS_ENABLE_PIN, HSS_ENABLE_PORT, 1);
}

