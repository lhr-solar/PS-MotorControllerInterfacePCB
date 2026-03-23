#include "can_bus.h"
#include <stm32l4xx_hal_can.h>
#include "common.h"
#include "pinDefs.h"
#include "prohelion_wavesculptor22_can_msgs.h"

// no queue

CAN_HandleTypeDef* can_handle = hcan1;

/**
 * @brief Initialize CAN
 */
can_status_t CAN_Init(void)
{

    // removed GPIO init block, it exists in MSP
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
    can_handle->Instance = CAN1;
    can_handle->Init.Prescaler = 20;
    can_handle->Init.Mode = CAN_MODE_NORMAL;
    can_handle->Init.SyncJumpWidth = CAN_SJW_1TQ;
    can_handle->Init.TimeSeg1 = CAN_BS1_13TQ;
    can_handle->Init.TimeSeg2 = CAN_BS2_2TQ;
    can_handle->Init.TimeTriggeredMode = DISABLE;
    can_handle->Init.AutoBusOff = DISABLE;
    can_handle->Init.AutoWakeUp = DISABLE;
    can_handle->Init.AutoRetransmission = DISABLE;
    can_handle->Init.ReceiveFifoLocked = DISABLE;
    
    // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
    // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
    can_handle->Init.TransmitFifoPriority = ENABLE;

    // Initialize CAN1
    if (can_init(can_handle, &sFilterConfig) != CAN_OK) { 
        return CAN_ERR;
    }
    // Start CAN1
    if (can_start(can_handle) != CAN_OK) { 
        return CAN_ERR;
    }
    return CAN_OK;
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcan->Instance==CAN1) {
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  if(hcan->Instance==CAN1) {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }
}

can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length) {
    CAN_TxHeaderTypeDef tx_header = {0};
    tx_header.StdId = id;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.IDE = CAN_ID_STD;
    tx_header.DLC = length;
    tx_header.TransmitGlobalTime = DISABLE;
    
    return can_send(can_handle, &tx_header, data, portMAX_DELAY);
}

/**
 * @brief Receive CAN message matching ID
 * @param id Expected STD ID
 * @param header Rx header output
 * @param data Rx data output (8 bytes)
 * @param timeout RTOS ticks to block
 * @return can_status_t CAN_OK / CAN_ERR / CAN_EMPTY
 */
can_status_t CANbus_recv(uint16_t id, CAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout) {
    return can_recv(can_handle, id, header, data, timeout);
}

// encodes a drive command struct into an array of bytes for can_sendWrite a function that takes the array of bytes you get from CAN and unpacks it into that struct for each message given in prohelion_wavesculptor22_can_msgs.h
static void packMocoCANMessage//(mc_drivecommand_t motorDriveCommand, uint8_t tx_data[8])
{
    // memcpy(&tx_data[4], &(motorDriveCommand.MC_MotorCurrentSetpoint), sizeof(float));
    // memcpy(&tx_data[0], &(motorDriveCommand.MC_MotorVelocitySetpoint), sizeof(float));
}

can_status_t can_unpack(uint16_t id, const uint8_t raw[8], void *msg) {
    switch (id) {
      case CAN_ID_STATUS:
      
    }
}