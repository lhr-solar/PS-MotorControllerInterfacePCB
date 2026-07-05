#include "motor_task.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

#define MOTOR_TASK_PRINTF_PERIOD_MS 5000 // 5 seconds
#define MOTOR_TASK_PRINTF_DEBUG_COUNTER (MOTOR_TASK_PRINTF_PERIOD_MS / pdTICKS_TO_MS(MOTOR_TASK_PERIOD)) // Number of task periods before printing debug info

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

   return INIT_OK;
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
   
}

static BaseType_t recv_motor_temp(mc_motor_tempmeasurement_t *mc_temp, TickType_t timeout)
{
   CAN_RxHeaderTypeDef rx_header = {0};
   uint8_t rx_data[8] = {0};
   
   can_status_t status = CANbus_recv(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, &rx_header, rx_data, timeout);
    if (status == CAN_OK && can_unpack_temp(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, rx_data, mc_temp) == CAN_OK) {
        HAL_GPIO_TogglePin(DEBUG_LED1_PORT, DEBUG_LED1_PIN);

       return pdPASS;
   }

   return pdFAIL;
   
}

static uint8_t find_duty(const mc_motor_tempmeasurement_t *motor_temp, BaseType_t *temp_fault) {

    float temp = motor_temp->MC_HeatsinkTemp;

    uint8_t duty = 100; // 24V fan undervoltage: 14V. Minimum duty% other than 0 must be 60%.

    if (temp >= 60.0f) {
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

    TickType_t last_wake = xTaskGetTickCount();

    BaseType_t read_motor_status = pdTRUE;
    BaseType_t read_motor_temp = pdTRUE;

    uint8_t failed_motor_reads = 0;

    uint8_t printf_debug_counter = 0;

    while (1)
    {    
        statusLEDs_toggle(HEARTBEAT_LED);

        hss_status_t hss_fault_status = HSS_GetFaultStatus();
        HSS_EN_SetState(hss_fault_status == HSS_FAULT ? HSS_OFF : HSS_ON, 0);

        read_motor_status = recv_motor_status(&motor_status, 0);
        read_motor_temp = recv_motor_temp(&motor_temp, 0);

        if(read_motor_status == pdPASS && printf_debug_counter >= MOTOR_TASK_PRINTF_DEBUG_COUNTER) {
            printf("Read motor Status: \n\r");
        } else {
            printf("Failed to read motor status\n\r");
        }

        if(read_motor_temp == pdPASS && printf_debug_counter >= MOTOR_TASK_PRINTF_DEBUG_COUNTER) {
            printf("Motor Temperature: %.5f\n\r", motor_temp.MC_HeatsinkTemp);
        } else {
            printf("Failed to read motor temperature\n\r");
        }

        // control the speed of the fans based on the recieved motor temperature
        // if we did not succesfully read the motor temperature, we will use the old temperature
        // if we fail to read enough times then the fans will go to 100%
        uint8_t fan_duty = find_duty(&motor_temp, &temp_fault);

        if (printf_debug_counter >= MOTOR_TASK_PRINTF_DEBUG_COUNTER) {
            printf("Fan Duty: %d\n\r", fan_duty);
        }

        if(printf_debug_counter >= MOTOR_TASK_PRINTF_DEBUG_COUNTER) {
            printf_debug_counter = 0;
        } 
        
        printf_debug_counter++;

        if(read_motor_status == pdFAIL || read_motor_temp == pdFAIL) {
            failed_motor_reads++;
            if (failed_motor_reads >= 3) { // If we fail to read motor status 3 times in a row, turn on fault LED
                statusLEDs_write(FAULT_LED, ON);

                // set fans to max if fail to read motor messages enough
                fan_duty = 100;
            }
        } 
        else {
            failed_motor_reads = 0; // reset counter on successful read
            statusLEDs_write(FAULT_LED, OFF);
        }


    
        PWM1_SetDuty(fan_duty);
        PWM2_SetDuty(fan_duty);
        
        statusLEDs_write(FAULT_LED, temp_fault == pdTRUE ? ON : OFF);
        
        vTaskDelayUntil(&last_wake, MOTOR_TASK_PERIOD);
    }
}


