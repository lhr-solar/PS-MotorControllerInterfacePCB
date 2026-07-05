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

BaseType_t HSS_EN_SetState(hss_status_t state, TickType_t timeout)
{
    if(xSemaphoreTake(hss_mutex, timeout) == pdTRUE) {
        hss_current_state = state;
        HAL_GPIO_WritePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN, state);
        xSemaphoreGive(hss_mutex);
        return pdTRUE;
    }
    return pdFALSE;
}

BaseType_t HSS_EN_Toggle(TickType_t timeout)
{
    // Acquire mutex before modifying state
    if(xSemaphoreTake(hss_mutex, timeout) == pdTRUE) {
        HAL_GPIO_TogglePin(HSS_ENABLE_PORT, HSS_ENABLE_PIN);
        
        // Release mutex after done
        xSemaphoreGive(hss_mutex);
        return pdTRUE;
    }
    return pdFALSE;
}

hss_status_t HSS_GetFaultStatus(void)
{
    return (HAL_GPIO_ReadPin(HSS_FAULT_PORT, HSS_FAULT_PIN) == GPIO_PIN_SET) ? HSS_FAULT : HSS_OK;
}


