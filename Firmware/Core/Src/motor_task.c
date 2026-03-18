#include "motor_task.h"
#include "pwm.h"
#include "led.h"
#include "hss.h"
#include "can_bus.h"
#include "common.h"
#include "CAN.h"

// Static buffers for motor task
StaticTask_t Motor_Task_Buffer;
StackType_t Motor_Task_Stack[MOTOR_TASK_STACK_SIZE];

// Motor state variables
/* static int16_t current_velocity = 0;
static uint16_t bus_voltage = 0;
static int16_t motor_current = 0; */
// static bool motor_ready = false;

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
   if (CAN_Init() != CAN_OK) {
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

static ws_status_t motor_status;

static bool recv_motor_status(void) //(x241)
{
   CAN_RxHeaderTypeDef rx_header = {0};
   uint8_t rx_data[8] = {0};
   can_status_t status;

   status = can_recv(hcan1, MOTOR_STATUS, &rx_header, rx_data, 0);

   if(status == CAN_RECV)
   {
       motor_status.limit_flags =
           rx_data[0] | (rx_data[1] << 8);

       motor_status.error_flags =
           rx_data[2] | (rx_data[3] << 8);

       motor_status.active_motor =
           rx_data[4] | (rx_data[5] << 8);

       motor_status.tx_error_count = rx_data[6];
       motor_status.rx_error_count = rx_data[7];
       return true;
   }
   return false;
}


/**
 * @brief Send motor command to motor controller over CAN
 * @param torque_command Torque value to send (signed 16-bit)
 * @param angular_velocity Angular velocity to send (signed 16-bit)
 */
static void send_motor_command(int16_t torque_command, int16_t angular_velocity)
{
    uint8_t data[8] = {0};
    
    // Pack torque command (2 bytes, little-endian)
    data[0] = (uint8_t)(torque_command & 0xFF);
    data[1] = (uint8_t)((torque_command >> 8) & 0xFF);
    
    // Pack angular velocity (2 bytes, little-endian)
    data[2] = (uint8_t)(angular_velocity & 0xFF);
    data[3] = (uint8_t)((angular_velocity >> 8) & 0xFF);
    
    // Send to CONTROL_MODE ID (0x580)
    CAN_Send_Motor_Command(CONTROL_MODE, data, 4);
}


void Motor_Task(void *pvParameters)
{
   (void)pvParameters;
  
   // Default torque command (0 = no torque)
   int16_t torque_command = 0;
   int16_t angular_velocity = 0;
  
   while (1)
   {
       // Try to receive various CAN messages (non-blocking)
       /* receive_motor_status();
       receive_velocity();
       receive_bus_voltage(); */

       recv_motor_status();
      
       // Example: Send control command
       // In a real application, this would be based on input from controls
       send_motor_command(torque_command, angular_velocity);
      
       // Sets duty cycles for the PWMs
       PWM1_SetDuty(70);
       PWM2_SetDuty(70);
      
       // Enable HSS circuit
       HSS_EN_SetState(HSS_ON, portMAX_DELAY);


       vTaskDelay(MOTOR_TASK_PERIOD);
   }
}
