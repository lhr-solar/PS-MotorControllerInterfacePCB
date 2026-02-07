#pragma once
#include "stm32xx_hal.h"
#include "pinDefs.h"

void LED_Init(void);
void LED_Toggle(void);
void LED_SetState(uint16_t pin, GPIO_TypeDef* port, uint8_t state);
void LED_TogglePin(uint16_t pin, GPIO_TypeDef* port);

