#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "hss.h"

// Global variable to track HSS state
static hss_status_t hss_current_state = HSS_OFF;

void HSS_EN_SetState(hss_status_t state)
{
    hss_current_state = state;
    HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, state);
}

void HSS_EN_Toggle(void)
{
    // Toggle the state
    hss_current_state = (hss_current_state == HSS_ON) ? HSS_OFF : HSS_ON;
    HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, hss_current_state);
}


