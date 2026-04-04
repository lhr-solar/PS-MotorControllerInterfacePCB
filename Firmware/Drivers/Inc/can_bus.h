#pragma once
#include "CAN.h"
#include "common.h"
#include "pinDefs.h"

#define CAN_INTERRUPT_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

extern CAN_HandleTypeDef *hcan1;

// CAN function declarations
can_status_t CANbus_init(void);
/**
 * @brief Send a motor controller command over CAN
 * @param id CAN message ID
 * @param data Data bytes to send
 * @param length Number of data bytes (max 8)
 * @return can_status_t status
 * 
 */
can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length, TickType_t timeout);
/**
 * @brief Receive CAN message matching ID
 * @param id Expected STD ID
 * @param header Rx header output
 * @param data Rx data output (8 bytes)
 * @param timeout RTOS ticks to block
 * @return can_status_t CAN_OK / CAN_ERR / CAN_EMPTY
 */
can_status_t CANbus_recv(uint16_t id, CAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout);



