#pragma once
#include "pinDefs.h"
typedef enum {
    HSS_OFF = 0,
    HSS_ON = 1
} hss_status_t;

/**
 * @brief HSS_EN_Toggle toggles the HSS switch on and off. (default off)
 * HSS_EN_SetState sets hss_status to a variable of type hss_status_t
 * 
 * @param state 
 * @param void
 * @return 
 */
void HSS_EN_SetState(hss_status_t state);
void HSS_EN_Toggle(void);
