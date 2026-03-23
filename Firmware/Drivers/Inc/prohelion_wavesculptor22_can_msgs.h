#pragma once

#include <stdint.h>

/* ================= CAN ID Macros ================= */

#define CAN_ID_IDINFO 0x240
#define CAN_ID_STATUS 0x241
#define CAN_ID_BUSMEASUREMENT 0x242
#define CAN_ID_VELOCITYMEASUREMENT 0x243
#define CAN_ID_PHASECURRENTMEASUREMENT 0x244
#define CAN_ID_MOTORVOLTAGEVECTORMEASUREMENT 0x245
#define CAN_ID_MOTORCURRENTVECTORMEASUREMENT 0x246
#define CAN_ID_BACKEMFMEASUREMENTPREDICTION 0x247
#define CAN_ID_VOLTAGERAIL15VMEASUREMENT 0x248
#define CAN_ID_VOLTAGERAIL3V31V9MEASUREMENT 0x249
#define CAN_ID_RESERVED0A 0x24A
#define CAN_ID_HEATSINKMOTORTEMPMEASUREMENT 0x24B
#define CAN_ID_DSPBOARDTEMPMEASUREMENT 0x24C
#define CAN_ID_RESERVED0D 0x24D
#define CAN_ID_ODOMETERBUSAHMEASUREMENT 0x24E
#define CAN_ID_SLIPSPEEDMEASUREMENT 0x257

/* ================= CAN Length Macros ================= */

#define CAN_DLC_IDINFO 8
#define CAN_DLC_STATUS 8
#define CAN_DLC_BUSMEASUREMENT 8
#define CAN_DLC_VELOCITYMEASUREMENT 8
#define CAN_DLC_PHASECURRENTMEASUREMENT 8
#define CAN_DLC_MOTORVOLTAGEVECTORMEASUREMENT 8
#define CAN_DLC_MOTORCURRENTVECTORMEASUREMENT 8
#define CAN_DLC_BACKEMFMEASUREMENTPREDICTION 8
#define CAN_DLC_VOLTAGERAIL15VMEASUREMENT 8
#define CAN_DLC_VOLTAGERAIL3V31V9MEASUREMENT 8
#define CAN_DLC_RESERVED0A 8
#define CAN_DLC_HEATSINKMOTORTEMPMEASUREMENT 8
#define CAN_DLC_DSPBOARDTEMPMEASUREMENT 8
#define CAN_DLC_RESERVED0D 8
#define CAN_DLC_ODOMETERBUSAHMEASUREMENT 8
#define CAN_DLC_SLIPSPEEDMEASUREMENT 8


/* ================= Message Structs ================= */

typedef struct {
    uint32_t TritiumID;
    uint32_t SerialNumber;
} idinfo_t;

typedef struct {
    uint8_t LimitOutputVoltagePWM;
    uint8_t LimitMotorCurrent;
    uint8_t LimitVelocity;
    uint8_t LimitBusCurrent;
    uint8_t LimitBusVoltageUpper;
    uint8_t LimitBusVoltageLower;
    uint8_t LimitIpmOrMotorTemp;
    uint16_t LimitReserved;
    uint8_t ErrorHardwareOverCurrent;
    uint8_t ErrorSoftwareOverCurrent;
    uint8_t ErrorDcBusOverVoltage;
    uint8_t ErrorBadMotorPositionHallSeq;
    uint8_t ErrorWatchdogCausedLastReset;
    uint8_t ErrorConfigRead;
    uint8_t Error15vRailUnderVoltage;
    uint8_t ErrorDesaturationFault;
    uint8_t ErrorMotorOverSpeed;
    uint8_t ErrorReserved;
    uint16_t ActiveMotor;
    uint8_t TxErrorCount;
    uint8_t RxErrorCount;
} status_t;

typedef struct {
    float BusVoltage;
    float BusCurrent;
} busmeasurement_t;

typedef struct {
    float MotorVelocity;
    float VehicleVelocity;
} velocitymeasurement_t;

typedef struct {
    float PhaseCurrentB;
    float PhaseCurrentC;
} phasecurrentmeasurement_t;

typedef struct {
    float Vq;
    float Vd;
} motorvoltagevectormeasurement_t;

typedef struct {
    float Iq;
    float Id;
} motorcurrentvectormeasurement_t;

typedef struct {
    float BEMFq;
    float BEMFd;
} backemfmeasurementprediction_t;

typedef struct {
    int32_t ReservedSupply15V;
    float Supply15V;
} voltagerail15vmeasurement_t;

typedef struct {
    float Supply1V9;
    float Supply3V3;
} voltagerail3v31v9measurement_t;

typedef struct {
    int32_t Reserved0A0;
    int32_t Reserved0A1;
} reserved0a_t;

typedef struct {
    float MotorTemp;
    float HeatsinkTemp;
} heatsinkmotortempmeasurement_t;

typedef struct {
    float DspBoardTemp;
    int32_t ReservedDspBoardTemp;
} dspboardtempmeasurement_t;

typedef struct {
    int32_t Reserved0D0;
    int32_t Reserved0D1;
} reserved0d_t;

typedef struct {
    float Odometer;
    float DCBusAh;
} odometerbusahmeasurement_t;

typedef struct {
    int32_t SlipSpeed;
    int32_t ReservedSlipSpeed;
} slipspeedmeasurement_t;

