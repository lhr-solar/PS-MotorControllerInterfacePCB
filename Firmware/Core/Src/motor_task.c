#include "motor_task.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

// Motor state variables
static mc_status_t motor_status;
static mc_motor_tempmeasurement_t motor_temp;
static BaseType_t temp_fault;


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
      HAL_GPIO_TogglePin(DEBUG_LED2_PORT, DEBUG_LED2_PIN);
      return pdPASS;
   }
   return pdFAIL;
}

static BaseType_t recv_motor_temp(mc_motor_tempmeasurement_t *mc_temp, TickType_t timeout)
{
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8] = {0};
   
   can_status_t status = CANbus_recv(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, &rx_header, rx_data, timeout);
    if (status == CAN_OK && can_unpack_temp(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, rx_data, mc_temp) == CAN_OK) {
       HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
       return pdPASS;
   }
   return pdFAIL;
}

static uint8_t search_lut(void) {

    float temp = motor_temp.MC_HeatsinkTemp;
    printf("temp: %.5f/n/r", temp);

    uint8_t duty = 60;

    if (temp >= 70.0f) {
        temp_fault = true;
        duty = 100;
    } else if (temp >= 40.0f) {
        temp_fault = false;
        for (int i = 0; i < ((sizeof(lut))/(sizeof(lut[0]))); i++) {
            if (temp > lut[i].temp)
                duty = lut[i].duty;
        }
    } else {
        duty = 0; // turns fans off

        return duty;
    }

    return 0;
}

// receive from moco only no send
void Motor_Task(void *pvParameters)
{
   (void)pvParameters;
  
   while (1)
   {
       BaseType_t status_ok = recv_motor_status(&motor_status, 0);
       BaseType_t temp_ok = recv_motor_temp(&motor_temp, 0);

       int fault_state = HAL_GPIO_ReadPin(HSS_FAULT_PORT, HSS_FAULT_PIN);
       bool hss_safe = (fault_state == GPIO_PIN_RESET); 

       if (hss_safe) {
           HSS_EN_SetState(HSS_ON);
       } else {
           HSS_EN_SetState(HSS_OFF);
       }

       uint8_t fan_duty = 0;

       // Temperature-based fan control                                                                                                                            a
       if (temp_ok || status_ok) { // will be adding to status
            fan_duty = search_lut();
        }
       
       PWM1_SetDuty(fan_duty);
       PWM2_SetDuty(fan_duty);
       
       // Fault handling
        if (temp_fault) {
            statusLEDs_write(FAULT_LED, ON);
        } else
            statusLEDs_write(FAULT_LED, OFF);
      
       vTaskDelay(MOTOR_TASK_PERIOD);
   }
}


