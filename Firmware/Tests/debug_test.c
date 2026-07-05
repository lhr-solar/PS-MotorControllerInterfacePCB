#include "Debugging.h"
#include "FreeRTOS.h"

#define TEST_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define TEST_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
StaticTask_t TEST_TASK_TCB;
StackType_t TEST_TASK_Stack_Array[TEST_TASK_STACK_SIZE];

void DebugTask(void *argument) {
int i = 0;
	while (1) {
        printf("\033[2J");
		printf("Mika Sucks Balls: %d\n\r", i);
        vTaskDelay(pdMS_TO_TICKS(50));
        i++;

	}
}

int main(void) {
    HAL_Init();
	SystemClock_Config();
     initPrintf();


	xTaskCreateStatic(DebugTask, "Debugging sucks", TEST_TASK_STACK_SIZE, NULL,
					  TEST_TASK_PRIORITY, TEST_TASK_Stack_Array, &TEST_TASK_TCB

	);
	vTaskStartScheduler();

	while (1) {
		// printf("Mika Sucks Balls\n");
		// HAL_Delay(500);
	}
}