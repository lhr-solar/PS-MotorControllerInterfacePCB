#pragma once

#include "can_bus.h"
#include "MotorCAN_can_msgs.h"
#include <string.h>

can_status_t can_unpack_status(uint16_t id, const uint8_t rx_data[8], mc_status_t * status);

can_status_t can_unpack_temp(uint16_t id, const uint8_t rx_data[8], mc_motor_tempmeasurement_t * temp);
