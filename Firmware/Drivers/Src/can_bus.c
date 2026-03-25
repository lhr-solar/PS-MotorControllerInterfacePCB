#include "can_bus.h"
#include "stm32xx_hal.h"
#include "common.h"
#include "pinDefs.h"
#include "MotorCAN_can_msgs.h"

// no queue

CAN_HandleTypeDef* can_handle;

/**
 * @brief Initialize CAN
 */
can_status_t CAN_Init(CAN_HandleTypeDef* hcan)
{

    can_handle = hcan;
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

can_status_t can_unpack(uint16_t id, const uint8_t rx_data[8], void *dest) { //
    switch (id) {
      case CAN_ID_MC_STATUS: {
        mc_status_t *status = (mc_status_t*)dest;
        memset(status, 0, sizeof(mc_status_t));

        uint64_t raw = 0;
        for (int i = 0; i < 8; i++) { // can message array into 64 bit integer
          raw |= ((uint64_t)rx_data[i]) << (8 * i);
        }
          status->MC_LIMIT_OutputVoltagePWM = (raw >> 0) & 0x1; // [0:0]
          status->MC_LIMIT_MotorCurrent     = (raw >> 1) & 0x1; // [1:1]
          status->MC_LIMIT_Velocity         = (raw >> 2) & 0x1; // [2:2]
          status->MC_LIMIT_BusCurrent       = (raw >> 3) & 0x1; // [3:3]
          status->MC_LIMIT_BusVoltageUpper  = (raw >> 4) & 0x1; // [4:4]
          status->MC_LIMIT_BusVoltageLower  = (raw >> 5) & 0x1; // [5:5]
          status->MC_LIMIT_MotorTemp        = (raw >> 6) & 0x1; // [6:6]

          status->MC_LIMIT_Reserved = (raw >> 7) & 0x1FF; // [7:15]

          status->MC_FAULT_HardwareOverCurrent     = (raw >> 16) & 0x1; // [16:16]
          status->MC_FAULT_SoftwareOverCurrent     = (raw >> 17) & 0x1; // [17:17]
          status->MC_FAULT_DcBusOverVoltage        = (raw >> 18) & 0x1; // [18:18]
          status->MC_FAULT_BadMotorPositionHallSeq = (raw >> 19) & 0x1; // [19:19]
          status->MC_FAULT_WatchdogCausedLastReset = (raw >> 20) & 0x1; // [20:20]
          status->MC_FAULT_ConfigRead              = (raw >> 21) & 0x1; // [21:21]
          status->MC_FAULT_15vRailUnderVoltage     = (raw >> 22) & 0x1; // [22:22]
          status->MC_FAULT_DesaturationFault       = (raw >> 23) & 0x1; // [23:23]
          status->MC_FAULT_MotorOverSpeed          = (raw >> 24) & 0x1; // [24:24]

          status->MC_FAULT_Reserved = (raw >> 25) & 0x7F; // [25:31]

          status->MC_ActiveMotor  = (raw >> 32) & 0xFFFF; // [32:47]
          status->MC_TxErrorCount = (raw >> 48) & 0xFF; // [48:55]
          status->MC_RxErrorCount = (raw >> 56) & 0xFF; // [56:63]

            return CAN_OK;
        }
      case CAN_ID_MC_DRIVECOMMAND:
        memcpy(dest, rx_data, CAN_DLC_MC_DRIVECOMMAND);
        return CAN_OK;
      case CAN_ID_MC_POWERCOMMAND:
        memcpy(dest, rx_data, CAN_DLC_MC_POWERCOMMAND);
        return CAN_OK;
      case CAN_ID_MC_RESETCOMMAND:
        memcpy(dest, rx_data, CAN_DLC_MC_RESETCOMMAND);
        return CAN_OK;
      case CAN_ID_MC_INFO: 
        memcpy(dest, rx_data, CAN_DLC_MC_INFO);
        return CAN_OK;
      case CAN_ID_MC_BUSMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_BUSMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_VELOCITYMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_VELOCITYMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_PHASECURRENTMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_PHASECURRENTMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_MOTORVOLTAGEVECTORMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_MOTORVOLTAGEVECTORMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_MOTORCURRENTVECTORMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_MOTORCURRENTVECTORMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_BACKEMFMEASUREMENTPREDICTION:
        memcpy(dest, rx_data, CAN_DLC_MC_BACKEMFMEASUREMENTPREDICTION);
        return CAN_OK;
      case CAN_ID_MC_15VRAILMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_15VRAILMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_3V319VRAILMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_3V319VRAILMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_MOTOR_TEMPMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_MOTOR_TEMPMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_DSPBOARDTEMPMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_DSPBOARDTEMPMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_ODOMETERBUSAHMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_ODOMETERBUSAHMEASUREMENT);
        return CAN_OK;
      case CAN_ID_MC_SLIPSPEEDMEASUREMENT:
        memcpy(dest, rx_data, CAN_DLC_MC_SLIPSPEEDMEASUREMENT);
        return CAN_OK;
      default:
        return CAN_ERR;
    }
}
