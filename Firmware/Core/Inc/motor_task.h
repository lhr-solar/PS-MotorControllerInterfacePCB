#pragma once
#include "stm32xx_hal.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"
#include "can_bus.h"
#include "MotorCAN_can_msgs.h"
#include "Debugging.h"
#include "can_unpack.h"
typedef struct {
    float temp;
    uint8_t duty;
    } temp_lut_entry_t;

typedef enum {
    INIT_OK,
    INIT_ERR
    } init_status_t;

static const temp_lut_entry_t lut[] = {
    {40.0f, 60},    // {Celsius, %}
    {50.0f, 76},
    {60.0f, 93},
    {70.0f, 100}
};

#define MOTOR_TASK_PRIO (tskIDLE_PRIORITY + 2)
#define MOTOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE * 2 // 128 * 2 = 256 words = 1 KB

extern StaticTask_t Motor_Task_Buffer;
extern StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

#define MOTOR_TASK_PERIOD pdMS_TO_TICKS(10)

init_status_t Motor_Task_Init(void);

void Motor_Task(void *pvParameters);