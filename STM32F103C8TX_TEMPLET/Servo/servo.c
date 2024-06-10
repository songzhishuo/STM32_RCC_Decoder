#include "servo.h"


void servo_start(uint8_t id)
{
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}


void servo_ctrl(int16_t angle)
{
    float temp = 0;

    if((angle < 0) || (angle > 180))
    {
        return ;
    }

    if(angle != 0)
        temp = (200 * angle / 180) + 50;
    else
        temp = 50;

//    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, temp);
}