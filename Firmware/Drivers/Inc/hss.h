#pragma once
#include "pinDefs.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef enum {
    HSS_OFF = 0,
    HSS_ON = 1
} hss_status_t;

/**
 * @brief Initialize HSS driver (creates mutex)
 * @return pdTRUE if successful, pdFALSE if failed
 */
BaseType_t HSS_Init(void);

/**
 * @brief Set the status of HSS to ON or OFF
 * @param state Assigned to global variable hss_current_state
 * @note HEARTBEAT led should blink when called
 */
void HSS_EN_SetState(hss_status_t state);

/**
 * @brief Toggle HSS circuit ON and OFF
 * @note HEARTBEAT led should blink when called
 */
void HSS_EN_Toggle(void);
