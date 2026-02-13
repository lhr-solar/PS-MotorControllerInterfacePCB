// PWM on PB10 and PA5
// when u run this test should see pwms loop from duty cycles 0, 25, 50, 75, 100, PB0 toggle if code has been flashed
// test validated! ✅
#include "stm32xx_hal.h"
#include "pwm.h"
#include "led.h"
#include "motor_task.h"

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
        // test the variables
        for (uint8_t i = 0; i < num_tests; i++) 
        {
            PWM1_SetDuty(duty_cycles[i]);  // PA2
            PWM2_SetDuty(duty_cycles[i]);  // PA5 
            LED_Toggle(); // PB0 should toggle if pwm_test is flashed properly
        }
    }
    
    return 0;
}

