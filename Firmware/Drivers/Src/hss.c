#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "hss.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"

// Global variable to track HSS state
static hss_status_t hss_current_state = HSS_OFF;

// Mutex to protect hss_current_state
static SemaphoreHandle_t hss_mutex = NULL;
static StaticSemaphore_t hss_mutex_buffer;

BaseType_t HSS_Init(void)
{
    // Create mutex for hss_current_state
    hss_mutex = xSemaphoreCreateMutexStatic(&hss_mutex_buffer);
    if (hss_mutex == NULL) {
        return pdFALSE;
    }
    return pdTRUE;
}

void HSS_EN_SetState(hss_status_t state)
{
    // Acquire mutex before modifying state
    xSemaphoreTake(hss_mutex, portMAX_DELAY);
    
    hss_current_state = state;
    HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, state);
    
    // Release mutex after done
    xSemaphoreGive(hss_mutex);
}

void HSS_EN_Toggle(void)
{
    // Acquire mutex before modifying state
    xSemaphoreTake(hss_mutex, portMAX_DELAY);
    
    hss_current_state = (hss_current_state == HSS_ON) ? HSS_OFF : HSS_ON;
    HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, hss_current_state);
    
    // Release mutex after done
    xSemaphoreGive(hss_mutex);
}


