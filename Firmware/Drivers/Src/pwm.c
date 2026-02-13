#include "pwm.h"
#include "pinDefs.h"

static TIM_HandleTypeDef* pPwm1TimHandle = NULL;
static TIM_HandleTypeDef* pPwm2TimHandle = NULL;

void PWM1_Init(TIM_HandleTypeDef* timHandle) // PWMA Initialization
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    pPwm1TimHandle = timHandle;

    // timHandle configurations
    timHandle->Instance = PWM1_TIMER_INSTANCE;
    timHandle->Init.Prescaler = PWM_PRESCALER;
    timHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
    timHandle->Init.Period = PWM_PERIOD;
    timHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(timHandle);

    // Clock Enables
    PWM1_TIMER_CLK_ENABLE();
    PWM1_GPIO_CLK_ENABLE();

    // GPIO Congifurations
    GPIO_InitStruct.Pin = PWM1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = PWM1_GPIO_AF;
    HAL_GPIO_Init(PWM1_GPIO_PORT, &GPIO_InitStruct);

    // PWM and Channel Configurations
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = PWM_PULSE;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, PWM1_TIMER_CHANNEL);

    HAL_TIM_PWM_Start(timHandle, PWM1_TIMER_CHANNEL);
}

void PWM1_SetDuty(uint8_t duty) // PWMA Duty Function
{
    if (duty > 100) duty = 100; // Caps duty at 100%
    uint32_t compare = (PWM_MAX_COUNT * duty) / 100; // Calculates compare value based on duty cycle %
    __HAL_TIM_SET_COMPARE(pPwm1TimHandle, PWM1_TIMER_CHANNEL, compare); // Sets compare registers
}

void PWM2_Init(TIM_HandleTypeDef* timHandle) // PWMB Initialization
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Store handle for use in PWM2_SetDuty
    pPwm2TimHandle = timHandle;

    // Configure the timer handle
    timHandle->Instance = PWM2_TIMER_INSTANCE;
    timHandle->Init.Prescaler = PWM_PRESCALER;
    timHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
    timHandle->Init.Period = PWM_PERIOD;
    timHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

 
    HAL_TIM_PWM_Init(timHandle);

    PWM2_TIMER_CLK_ENABLE();
    PWM2_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = PWM2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = PWM2_GPIO_AF;
    HAL_GPIO_Init(PWM2_GPIO_PORT, &GPIO_InitStruct);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = PWM_PULSE;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, PWM2_TIMER_CHANNEL);

    HAL_TIM_PWM_Start(timHandle, PWM2_TIMER_CHANNEL);
}

void PWM2_SetDuty(uint8_t duty) // PWMB Duty Function
{
    if (duty > 100) duty = 100;
    uint32_t compare = (PWM_MAX_COUNT * duty) / 100;
    __HAL_TIM_SET_COMPARE(pPwm2TimHandle, PWM2_TIMER_CHANNEL, compare);
}

