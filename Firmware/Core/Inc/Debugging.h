#pragma once

#include "stm32xx_hal.h"
#include <stdio.h>
#include "UART.h"
#include "printf.h"
#include "pinDefs.h"

extern UART_HandleTypeDef* husart1;
void initPrintf();