#pragma once
#include "CAN.h"
#include "common.h"
#include "pinDefs.h"


// MOCO CAN IDs
#define MOTOR_CONTROLLER_IDENTIFICATION  0x240
#define MOTOR_STATUS                    0x241
#define MOTOR_CONTROLLER_BUS            0x242
#define VELOCITY                        0x243
#define MOTOR_CONTROLLER_PHASE_CURRENT   0x244
#define MOTOR_VOLTAGE_VECTOR             0x245
#define MOTOR_CURRENT_VECTOR             0x246
#define MOTOR_BACKEMF                   0x247
#define LOW_VOLTAGE_RAIL_MEASUREMENT    0x248
#define DSP_VOLTAGE_RAIL_MEASUREMENT    0x249
#define MOTOR_TEMPERATURE                0x24B
#define DSP_BOARD_TEMPERATURE            0x24C


#define CONTROL_MODE                     0x580


// CAN function declarations
bool CAN_Init();
can_status_t CAN_Send_Motor_Command(uint16_t id, uint8_t data[], uint8_t length);



