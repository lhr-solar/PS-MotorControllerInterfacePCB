#include "motor_task.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

// Motor state variables - now local to Motor_Task


init_status_t Motor_Task_Init(void)
{
   // Initialize HSS (creates mutex)
   if (HSS_Init() != pdTRUE) {
       return INIT_ERR;
   }
  
   // Initialize both PWM outputs
   PWM1_Init(pwm1TimHandle);
   PWM2_Init(pwm2TimHandle); 
  
   // Initialize LEDs
   LEDs_Init();
  
   // Initialize CAN bus
   if (CANbus_init() != CAN_OK) {
       return INIT_ERR;
   }

   // Create the Motor Task
   xTaskCreateStatic(
       Motor_Task,              // Task function
       "Motor Task",            // Task name
       MOTOR_TASK_STACK_SIZE,   // Stack size
       NULL,                    // Parameters
       MOTOR_TASK_PRIO,         // Priority
       Motor_Task_Stack,        // Stack buffer
       &Motor_Task_Buffer       // TCB buffer
   );

   return INIT_ERR;
}

static BaseType_t recv_motor_status(mc_status_t *mc_status, TickType_t timeout)
{
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8] = {0};
   
   can_status_t status = CANbus_recv(CAN_ID_MC_STATUS, &rx_header, rx_data, timeout);
   if (status == CAN_OK && can_unpack_status(CAN_ID_MC_STATUS, rx_data, mc_status) == CAN_OK) {
      return pdPASS;
   }
   return pdFAIL;
   
   HAL_GPIO_TogglePin(DEBUG_LED2_PORT, DEBUG_LED2_PIN);
}

static BaseType_t recv_motor_temp(mc_motor_tempmeasurement_t *mc_temp, TickType_t timeout)
{
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8] = {0};
   
   can_status_t status = CANbus_recv(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, &rx_header, rx_data, timeout);
    if (status == CAN_OK && can_unpack_temp(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, rx_data, mc_temp) == CAN_OK) {
       return pdPASS;
   }
   return pdFAIL;
   
   HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
}

static uint8_t find_duty(const mc_motor_tempmeasurement_t *motor_temp, BaseType_t *temp_fault) {

    float temp = motor_temp->MC_HeatsinkTemp;
    printf("temp: %.5f\n/r", temp);

    uint8_t duty = 60; // 24V fan undervoltage: 14V. Minimum duty% other than 0 must be 60%.

    if (temp >= 70.0f) {
        *temp_fault = pdTRUE;
        duty = 100;
    } else if (temp >= 40.0f) {
        *temp_fault = pdFALSE;
        const size_t lut_size = sizeof(lut) / sizeof(lut[0]);
        for (size_t i = 0; i < lut_size; i++) {
            if (temp >= lut[i].temp)
                duty = lut[i].duty;
        }
    } else {
        duty = 0; // turns fans off
    }

    return duty;
}

// receive from moco only no send
void Motor_Task(void *pvParameters)
{
   (void)pvParameters;
   
   mc_status_t motor_status = {0};
   mc_motor_tempmeasurement_t motor_temp = {0};
   BaseType_t temp_fault = pdFALSE;
  HAL_GPIO_WritePin(DEBUG_LED1_PORT, DEBUG_LED1_PIN, 1);
   while (1)
   {
       recv_motor_status(&motor_status, 0);
       recv_motor_temp(&motor_temp, 0);

       int hss_fault_state = HAL_GPIO_ReadPin(HSS_FAULT_PORT, HSS_FAULT_PIN);
       bool hss_safe = (hss_fault_state == GPIO_PIN_RESET); 
       HSS_EN_SetState(hss_safe ? HSS_ON : HSS_OFF);

       // Temperature-based fan control
       uint8_t fan_duty = find_duty(&motor_temp, &temp_fault);

       PWM1_SetDuty(fan_duty);
       PWM2_SetDuty(fan_duty);
       
       // Fault handling
        statusLEDs_write(FAULT_LED, temp_fault ? ON : OFF);
      
       static TickType_t last_wake = 0;
       vTaskDelayUntil(&last_wake, MOTOR_TASK_PERIOD);
   }
}


