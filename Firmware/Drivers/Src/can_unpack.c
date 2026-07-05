#include "can_unpack.h"

can_status_t can_unpack_status(uint16_t id, const uint8_t rx_data[8], mc_status_t * status) { 

  if (id == CAN_ID_MC_STATUS) {

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

      status->MC_ActiveMotor  = rx_data[3]; // [32:47]
      status->MC_TxErrorCount = rx_data[5]; // [48:55]
      status->MC_RxErrorCount = rx_data[6]; // [56:63]

      return CAN_OK;
  }
    return CAN_ERR;
}

can_status_t can_unpack_temp(uint16_t id, const uint8_t rx_data[8], mc_motor_tempmeasurement_t * temp) {

  // MC_MotorTemp is float bits 0–31 (32 bits) unit Â°C
  // MC_HeatsinkTemp is float bits 32–63 (32 bits) unit Â°C
  if (id != CAN_ID_MC_MOTOR_TEMPMEASUREMENT) {
    return CAN_ERR;
  }

  memcpy(&temp->MC_MotorTemp, rx_data, sizeof(temp->MC_MotorTemp));
  memcpy(&temp->MC_HeatsinkTemp, rx_data + 4, sizeof(temp->MC_HeatsinkTemp));

  return CAN_OK;
}