#include "Debugging.h"
#include <stdio.h>

void initPrintf(void) {
	// First, initialize the UART hardware (GPIO, clocks, etc.)
	// UART init
	GPIO_InitTypeDef InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}

	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX
	*/
	InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	InitStruct.Mode = GPIO_MODE_AF_PP;
	InitStruct.Pull = GPIO_NOPULL;
	InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &InitStruct);

	// Then configure the UART handle and printf
	husart1->Init.BaudRate = 115200;
	husart1->Init.WordLength = UART_WORDLENGTH_8B;
	husart1->Init.StopBits = UART_STOPBITS_1;
	husart1->Init.Parity = UART_PARITY_NONE;
	husart1->Init.Mode = UART_MODE_TX_RX;
	husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	husart1->Init.OverSampling = UART_OVERSAMPLING_16;

	setvbuf(stdout, NULL, _IONBF, 0);

	printf_init(husart1);
}

//int _write(int file, char *ptr, int len) {
//	(void)file;
//	if (husart1 == NULL)
//		return -1;
//	if (HAL_UART_Transmit(husart1, (uint8_t *)ptr, (uint16_t)len,
//						  HAL_MAX_DELAY) != HAL_OK) {
//		return -1;
//	}
//	return len;
//}
