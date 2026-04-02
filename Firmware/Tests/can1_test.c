 #include "led.h"
#include "can_bus.h"
#include "pinDefs.h"
#include "common.h"
#include <stm32l4xx_hal_can.h>
// can send and receive works

/* 
Debugging: main is fine, inits passed and task created
*/
#define CAN1_TEST_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define CAN1_TEST_TASK_STACK configMINIMAL_STACK_SIZE * 2

StaticTask_t CAN1_test_task_buffer;
StackType_t CAN1_test_task_stack[CAN1_TEST_TASK_STACK];

void CAN1_test_task(void *pvParameters)
{
  (void)pvParameters;

  // HAL_GPIO_TogglePin(DEBUG_LED2_PORT, DEBUG_LED2_PIN); // on = task started ✅

  CAN_TxHeaderTypeDef tx_header = {0};
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.IDE = CAN_ID_STD;
  tx_header.DLC = 2;
  tx_header.TransmitGlobalTime = DISABLE;

  uint8_t tx_data[8] = {0};
  tx_data[1] = 0x00;

  while(1)
  {
    // --- Send two payloads to ID 0x1 ---
    tx_header.StdId = 0x1;

    tx_data[0] = 0x01;
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) == CAN_OK) {
      HAL_GPIO_TogglePin(DEBUG_LED1_PORT, DEBUG_LED1_PIN);
    } else 
      Error_Handler();

    // RX Test: PCAN → STM ID 0x1
    CAN_RxHeaderTypeDef rx_header = {0};
    uint8_t rx_data[8] = {0};
    if (can_recv(hcan1, 0x002, &rx_header, rx_data, pdMS_TO_TICKS(500)) == CAN_OK)  
      HAL_GPIO_TogglePin(DEBUG_LED2_PORT, DEBUG_LED2_PIN);
   
    HAL_GPIO_TogglePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN);
    // 3. Pause before sending the batch again
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  Error_Handler();
  // Task complete - delete itself
  vTaskDelete(NULL);
}

int main(void)
{
    
    LEDs_Init();
    if (HAL_Init() != HAL_OK) Error_Handler(); 
    SystemClock_Config();
    if (CANbus_init() != CAN_OK) Error_Handler();

    // HAL_GPIO_TogglePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN); // on = inits passed ✅

    xTaskCreateStatic (
        CAN1_test_task,
        "CAN1_test_task",
        CAN1_TEST_TASK_STACK,
        NULL,
        CAN1_TEST_TASK_PRIO + 2,
        CAN1_test_task_stack,
        &CAN1_test_task_buffer);
    vTaskStartScheduler();

    Error_Handler();

    return 0;
}