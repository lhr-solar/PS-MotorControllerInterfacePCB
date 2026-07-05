#pragma once
#include "pinDefs.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef enum {
    HSS_OFF = GPIO_PIN_RESET,
    HSS_ON = GPIO_PIN_SET
} hss_status_t;

#define HSS_OK 1
#define HSS_FAULT !HSS_OK

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
BaseType_t HSS_EN_SetState(hss_status_t state, TickType_t timeout);

/**
 * @brief Toggle HSS circuit state
 * @note HEARTBEAT led should blink when called
 */
BaseType_t HSS_EN_Toggle(TickType_t timeout);

/**
 * @brief Get the fault status of HSS
 * @return HSS_FAULT if fault is detected, HSS_OK otherwise
 */
hss_status_t HSS_GetFaultStatus(void);