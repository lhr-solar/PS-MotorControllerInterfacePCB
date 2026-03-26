#include "motor_task.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"
#include "can_bus.h"
#include "common.h"
#include "CAN.h"
#include "MotorCAN_can_msgs.h"
#include "can1_recv_entries.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

// Motor state variables
static mc_status_t motor_status;
static mc_motor_tempmeasurement_t motor_temp;
// static bool motor_temp_valid = false;

void Motor_Task_Init(void)
{
   // Initialize HSS (creates mutex)
   if (HSS_Init() != pdTRUE) {
       Error_Handler();
   }
  
   // Initialize both PWM outputs
   PWM1_Init(pwm1TimHandle);
   PWM2_Init(pwm2TimHandle); 
  
   // Initialize LEDs
   LEDs_Init();
  
   // Initialize CAN bus
   if (CAN_Init(hcan1) != CAN_OK) {
       Error_Handler();
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
}

static bool recv_motor_status(void)
{
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8] = {0};
   TickType_t timeout = 0; // non-blocking
   
   can_status_t status = CANbus_recv(CAN_ID_MC_STATUS, &rx_header, rx_data, timeout);
   if(status == CAN_OK) {
       return (can_unpack(CAN_ID_MC_STATUS, rx_data, &motor_status) == CAN_OK);
   }
   return false;
}

static bool recv_motor_temp(void)
{
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8] = {0};
   TickType_t timeout = 0;
   
   can_status_t status = CANbus_recv(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, &rx_header, rx_data, timeout);
   if(status == CAN_OK) {
       bool unpacked = (can_unpack(CAN_ID_MC_MOTOR_TEMPMEASUREMENT, rx_data, &motor_temp) == CAN_OK);
       //motor_temp_valid = unpacked;
       statusLEDs_toggle(HEARTBEAT_LED);
       return unpacked;
   }
   return false;
}

// receive from moco only no send
void Motor_Task(void *pvParameters)
{
   (void)pvParameters;
  
   while (1)
   {
       bool status_ok = recv_motor_status();
       bool temp_ok = recv_motor_temp();
      
       // HSS Enable logic (fixed)
       //GPIO_PinState fault_state = HAL_GPIO_ReadPin(HSS_FAULT_PORT, HSS_FAULT_PIN);
       //bool hss_safe = (fault_state == GPIO_PIN_SET); // SET = no fault (active-low)
       bool hss_safe = true;
       if (hss_safe) {
           HSS_EN_SetState(HSS_ON, portMAX_DELAY);
           statusLEDs_write(HSS_FAULT_LED, OFF);
       } else {
           HSS_EN_SetState(HSS_OFF, portMAX_DELAY);
           statusLEDs_write(HSS_FAULT_LED, ON);
       }
      
       // Temperature-based fan contro
       uint8_t fan_duty = 0;
       bool temp_fault = false;

       typedef struct {
            float temp;
            uint8_t duty;
       } temp_lut_entry_t;
       
       if (temp_ok && status_ok ) {
           float temp = motor_temp.MC_HeatsinkTemp;

            static const temp_lut_entry_t lut[] = {
                {40.0f, 0},
                {45.0f, 17},
                {50.0f, 33},
                {55.0f, 50},
                {60.0f, 67},
                {65.0f, 83},
                {70.0f, 100}
            };
        
           if (temp > 70.0f) {
               temp_fault = true;
               fan_duty = 100;
           } else if (temp > 40.0f) {
               fan_duty = 50;
               for (int i = 0; i < ((sizeof(lut))/(sizeof(lut[0]))); i++) {
                if (temp > lut[i].temp)
                    fan_duty = lut[i].duty;
               }
           } else {
               fan_duty = 0;
           }
       }
       
       PWM1_SetDuty(fan_duty);
       PWM2_SetDuty(fan_duty);
       
       // Fault handling
       if (temp_fault) {
           statusLEDs_write(HSS_FAULT_LED, ON);
       }
      
       vTaskDelay(MOTOR_TASK_PERIOD);
   }
}

