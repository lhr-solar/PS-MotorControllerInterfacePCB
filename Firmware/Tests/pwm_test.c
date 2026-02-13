// Tests Fan functionality
// Runs loop of duty cycles starting from 0%, 25%, 50%, 75%, 100%. LED1 turns on if code is active

// test validated! ✅
#include "stm32xx_hal.h"
#include "pwm.h"
#include "led.h"

int main(void) {
    HAL_Init();
    
    PWM1_Init(&pwm1TimHandle);  
    PWM2_Init(&pwm2TimHandle);  
    
    LED_Init();
    
    // Test variables
    uint8_t duty_cycles[] = {0, 25, 50, 75, 100};
    uint8_t num_tests = sizeof(duty_cycles) / sizeof(duty_cycles[0]);
    
    while (1) 
    {
        for (uint8_t i = 0; i < num_tests; i++) 
        {
            PWM1_SetDuty(duty_cycles[i]);  // PB10
            PWM2_SetDuty(duty_cycles[i]);  // PA5 
            Debug_LED_Toggle(); // Toggles LED1 if code is active
        }
    }
    
    return 0;
}

