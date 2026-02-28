#pragma once
#include "pinDefs.h"
#include "stm32xx_hal.h"
#include "stdint.h"

/**
 * @brief Initializes all status LEDs
 * @note HEARTBEAT led should blink when called
 */

 
 typedef enum {
    HEARTBEAT_LED,
    FAULT_LED,
    HSS_FAULT_LED
 } status_leds_t;

 typedef enum {
    OFF = GPIO_PIN_RESET,
    ON = GPIO_PIN_SET
 } pin_status_t;

void LEDs_Init(void);

