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
can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length);
can_status_t CANbus_recv(uint16_t id, CAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout);
can_status_t can_unpack(uint16_t id, const uint8_t rx_data[8], void *msg);



