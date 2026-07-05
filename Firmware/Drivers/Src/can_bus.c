#include "can_bus.h"
#include "stm32xx_hal.h"
#include "common.h"
#include "pinDefs.h"
#include "MotorCAN_can_msgs.h"
#include <string.h>
#include <stm32l4xx_hal_can.h>

CAN_HandleTypeDef *g_hcan1;

/**
 * @brief Initialize CAN
 */
can_status_t CANbus_init(void)
{
    g_hcan1 = hcan1;

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
    g_hcan1->Instance = CAN1;
    g_hcan1->Init.Prescaler = 20;
    g_hcan1->Init.Mode = CAN_MODE_NORMAL;
    g_hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
    g_hcan1->Init.TimeSeg1 = CAN_BS1_13TQ;
    g_hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
    g_hcan1->Init.TimeTriggeredMode = DISABLE;
    g_hcan1->Init.AutoBusOff = DISABLE;
    g_hcan1->Init.AutoWakeUp = DISABLE;
    g_hcan1->Init.AutoRetransmission = DISABLE;
    g_hcan1->Init.ReceiveFifoLocked = DISABLE;
    
    // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
    // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
    hcan1->Init.TransmitFifoPriority = ENABLE;

    // Initialize CAN1
    if (can_init(g_hcan1, &sFilterConfig) != CAN_OK) { 
        return CAN_ERR;
    }
    // Start CAN1
    if (can_start(g_hcan1) != CAN_OK) { 
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
    GPIO_InitStruct.Pin = CAN_RX_PIN|CAN_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(CAN_PORT, &GPIO_InitStruct);

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

can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length, TickType_t timeout) {
    CAN_TxHeaderTypeDef tx_header = {0};
    tx_header.StdId = id;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.IDE = CAN_ID_STD;
    tx_header.DLC = length;
    tx_header.TransmitGlobalTime = DISABLE;
    
    return can_send(g_hcan1, &tx_header, data, timeout);
}

can_status_t CANbus_recv(uint16_t id, CAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout) {
  return can_recv(g_hcan1, id, header, data, timeout);
}