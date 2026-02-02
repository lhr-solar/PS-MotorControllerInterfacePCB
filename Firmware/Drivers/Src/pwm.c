#include "pwm.h"
#include "pinDefs.h"

TIM_HandleTypeDef pwmTimHandle = {0}; // Declare the timer handle struct
GPIO_InitTypeDef GPIO_InitStruct = {0}; //set struct variable to 0

void PWM_Init(void){
    // Enable clocks for timer and GPIO
    PWM_TIMER_CLK_ENABLE();
    PWM_GPIO_CLK_ENABLE();

    //PA2 Congifurations
    GPIO_InitStruct.Pin = PWM_GPIO_PIN;//set pin
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; //set alternate function, push pull
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 
    GPIO_InitStruct.Alternate = PWM_GPIO_AF; //connedcts timer 2 to PA2 internally

    HAL_GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStruct);
    //PA2 is now electrically connected to TIM2_CH3!

    //TIM2 Configurations
    pwmTimHandle.Instance = PWM_TIMER_INSTANCE;
    pwmTimHandle.Init.Prescaler = PWM_PRESCALER; //83
    pwmTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    pwmTimHandle.Init.Period = PWM_PERIOD; //999    
    pwmTimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    pwmTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&pwmTimHandle);

    TIM_OC_InitTypeDef sConfigOC = {0}; //sConfigOC sets up initial PWM configs
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = PWM_PULSE; //sets initial pulse width 
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &sConfigOC, PWM_TIMER_CHANNEL);

    HAL_TIM_GenerateEvent(&pwmTimHandle, TIM_EVENTSOURCE_UPDATE);
    HAL_TIM_PWM_Start(&pwmTimHandle, PWM_TIMER_CHANNEL);
}

void PWM_SetDuty(uint8_t duty) //change duty cycle during runtime
{
    if (duty > 100) duty = 100;
    if (duty < 0) duty = 0;
    uint32_t compare = (PWM_MAX_COUNT * duty) / 100;
    
    __HAL_TIM_SET_COMPARE(&pwmTimHandle, PWM_TIMER_CHANNEL, compare);
}