#pragma once

#include <stdint.h>

/* ================= CAN ID Macros ================= */

#define CAN_ID_MC_DRIVECOMMAND 0x401
#define CAN_ID_MC_POWERCOMMAND 0x402
#define CAN_ID_MC_RESETCOMMAND 0x403
#define CAN_ID_MC_INFO 0x420
#define CAN_ID_MC_STATUS 0x421
#define CAN_ID_MC_BUSMEASUREMENT 0x422
#define CAN_ID_MC_VELOCITYMEASUREMENT 0x423
#define CAN_ID_MC_PHASECURRENTMEASUREMENT 0x424
#define CAN_ID_MC_MOTORVOLTAGEVECTORMEASUREMENT 0x425
#define CAN_ID_MC_MOTORCURRENTVECTORMEASUREMENT 0x426
#define CAN_ID_MC_BACKEMFMEASUREMENTPREDICTION 0x427
#define CAN_ID_MC_15VRAILMEASUREMENT 0x428
#define CAN_ID_MC_3V319VRAILMEASUREMENT 0x429
#define CAN_ID_MC_MOTOR_TEMPMEASUREMENT 0x42B
#define CAN_ID_MC_DSPBOARDTEMPMEASUREMENT 0x42C
#define CAN_ID_MC_ODOMETERBUSAHMEASUREMENT 0x42E
#define CAN_ID_MC_SLIPSPEEDMEASUREMENT 0x437

/* ================= CAN Length Macros ================= */

#define CAN_DLC_MC_DRIVECOMMAND 8
#define CAN_DLC_MC_POWERCOMMAND 4
#define CAN_DLC_MC_RESETCOMMAND 1
#define CAN_DLC_MC_INFO 8
#define CAN_DLC_MC_STATUS 8
#define CAN_DLC_MC_BUSMEASUREMENT 8
#define CAN_DLC_MC_VELOCITYMEASUREMENT 8
#define CAN_DLC_MC_PHASECURRENTMEASUREMENT 8
#define CAN_DLC_MC_MOTORVOLTAGEVECTORMEASUREMENT 8
#define CAN_DLC_MC_MOTORCURRENTVECTORMEASUREMENT 8
#define CAN_DLC_MC_BACKEMFMEASUREMENTPREDICTION 8
#define CAN_DLC_MC_15VRAILMEASUREMENT 8
#define CAN_DLC_MC_3V319VRAILMEASUREMENT 8
#define CAN_DLC_MC_MOTOR_TEMPMEASUREMENT 8
#define CAN_DLC_MC_DSPBOARDTEMPMEASUREMENT 8
#define CAN_DLC_MC_ODOMETERBUSAHMEASUREMENT 8
#define CAN_DLC_MC_SLIPSPEEDMEASUREMENT 8
#define dihthard 69


/* ================= Message Structs ================= */

typedef struct {
    float MC_MotorVelocitySetpoint;
    float MC_MotorCurrentSetpoint;
} mc_drivecommand_t;

typedef struct {
    float MC_MotorPowerSetpoint;
} mc_powercommand_t;

typedef struct {
    uint8_t MC_Reset;
} mc_resetcommand_t;

typedef struct {
    uint32_t MC_TritiumID;
    uint32_t MC_SerialNumber;
} mc_info_t;

typedef struct {
    uint8_t MC_LIMIT_OutputVoltagePWM;
    uint8_t MC_LIMIT_MotorCurrent;
    uint8_t MC_LIMIT_Velocity;
    uint8_t MC_LIMIT_BusCurrent;
    uint8_t MC_LIMIT_BusVoltageUpper;
    uint8_t MC_LIMIT_BusVoltageLower;
    uint8_t MC_LIMIT_MotorTemp;
    uint16_t MC_LIMIT_Reserved;
    uint8_t MC_FAULT_HardwareOverCurrent;
    uint8_t MC_FAULT_SoftwareOverCurrent;
    uint8_t MC_FAULT_DcBusOverVoltage;
    uint8_t MC_FAULT_BadMotorPositionHallSeq;
    uint8_t MC_FAULT_WatchdogCausedLastReset;
    uint8_t MC_FAULT_ConfigRead;
    uint8_t MC_FAULT_15vRailUnderVoltage;
    uint8_t MC_FAULT_DesaturationFault;
    uint8_t MC_FAULT_MotorOverSpeed;
    uint8_t MC_FAULT_Reserved;
    uint16_t MC_ActiveMotor;
    uint8_t MC_TxErrorCount;
    uint8_t MC_RxErrorCount;
} mc_status_t;

typedef struct {
    float MC_BusVoltage;
    float MC_BusCurrent;
} mc_busmeasurement_t;

typedef struct {
    float MC_MotorVelocity;
    float MC_VehicleVelocity;
} mc_velocitymeasurement_t;

typedef struct {
    float MC_PhaseCurrentB;
    float MC_PhaseCurrentC;
} mc_phasecurrentmeasurement_t;

typedef struct {
    float MC_Vq;
    float MC_Vd;
} mc_motorvoltagevectormeasurement_t;

typedef struct {
    float MC_Iq;
    float MC_Id;
} mc_motorcurrentvectormeasurement_t;

typedef struct {
    float MC_BEMFq;
    float MC_BEMFd;
} mc_backemfmeasurementprediction_t;

typedef struct {
    float MC_Supply15V;
} mc_15vrailmeasurement_t;

typedef struct {
    float MC_Supply1V9;
    float MC_Supply3V3;
} mc_3v319vrailmeasurement_t;

typedef struct {
    float MC_MotorTemp;
    float MC_HeatsinkTemp;
} mc_motor_tempmeasurement_t;

typedef struct {
    float MC_DspBoardTemp;
} mc_dspboardtempmeasurement_t;

typedef struct {
    float MC_TripOdometer;
    float MC_DCBusAh;
} mc_odometerbusahmeasurement_t;

typedef struct {
    float MC_SlipSpeed;
} mc_slipspeedmeasurement_t;

