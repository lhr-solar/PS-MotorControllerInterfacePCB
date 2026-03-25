#pragma once
#include "stm32xx_hal.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"
#include "can_bus.h"


#define MOTOR_TASK_PRIO (tskIDLE_PRIORITY + 2)
#define MOTOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE * 2 // 128 * 2 = 256 words = 1 KB


extern StaticTask_t Motor_Task_Buffer;
extern StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];


#define MOTOR_TASK_PERIOD pdMS_TO_TICKS(10)


void Motor_Task_Init(void);

void Motor_Task(void *pvParameters);