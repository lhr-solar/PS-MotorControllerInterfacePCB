#pragma once

#include "stm32xx_hal.h"
#include <stdio.h>
#include "UART.h"
#include "printf.h"
#include "pinDefs.h"

extern UART_HandleTypeDef* husart1;
uart_status_t initPrintf();