#include "unpack.h"

can_status_t can_unpack_status(uint16_t id, const uint8_t rx_data[8], void *dest) { 

  if (id == CAN_ID_MC_STATUS) {

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
    return CAN_ERR;
}

can_status_t can_unpack_temp(uint16_t id, const uint8_t rx_data[8], void *dest) {

  if (id == CAN_ID_MC_MOTOR_TEMPMEASUREMENT) {
        memcpy(dest, rx_data, CAN_DLC_MC_MOTOR_TEMPMEASUREMENT);
        return CAN_OK;
  }
    return CAN_ERR;
}

/*
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
        HAL_GPIO_WritePin(SOFTWARE_OC_PORT, SOFTWARE_OC_PIN, 1);
        return CAN_ERR;
*/