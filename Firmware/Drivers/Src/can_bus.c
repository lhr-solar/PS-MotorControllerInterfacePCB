#include "can_bus.h"
#include <stm32l4xx_hal_can.h>

/**
 * @brief Initialize CAN GPIO pins 
 */
bool CAN_Init()
{
    // Initialize GPIOs
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable CAN clocks
    CAN1_CLK_ENABLE();
    CAN_GPIO_CLK_ENABLE();

    // Configure CAN RX pin (PB8)
    GPIO_InitStruct.Pin = CAN_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = CAN_RX_AF;
    HAL_GPIO_Init(CAN_RX_PORT, &GPIO_InitStruct);

    // Configure CAN TX pin (PB9)
    GPIO_InitStruct.Pin = CAN_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = CAN_TX_AF;
    HAL_GPIO_Init(CAN_TX_PORT, &GPIO_InitStruct);
    
    // Create Filter
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    // Setup CAN1 Initialization
    hcan1->Init.Prescaler = 20;
    hcan1->Init.Mode = CAN_MODE_LOOPBACK;
    hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1->Init.TimeSeg1 = CAN_BS1_6TQ;
    hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan1->Init.TimeTriggeredMode = DISABLE;
    hcan1->Init.AutoBusOff = DISABLE;
    hcan1->Init.AutoWakeUp = DISABLE;
    hcan1->Init.AutoRetransmission = ENABLE;
    hcan1->Init.ReceiveFifoLocked = DISABLE;
    
    // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
    // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
    hcan1->Init.TransmitFifoPriority = ENABLE;

    // Initialize CAN1
    if (can_init(hcan1, &sFilterConfig) != CAN_OK) {
        return false;
    }
    // Start CAN1
    if (can_start(hcan1) != CAN_OK) {
        return false;
    }
    return true;
}
/**
 * @brief Send a motor controller command over CAN
 * @param id CAN message ID
 * @param data Data bytes to send
 * @param length Number of data bytes (max 8)
 * @return can_status_t status
 */

can_status_t CAN_Send_Motor_Command(uint16_t id, uint8_t data[], uint8_t length) {
    CAN_TxHeaderTypeDef tx_header = {0};
    tx_header.StdId = id;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.IDE = CAN_ID_STD;
    tx_header.DLC = length;
    tx_header.TransmitGlobalTime = DISABLE;
    
    return can_send(hcan1, &tx_header, data, portMAX_DELAY);
}