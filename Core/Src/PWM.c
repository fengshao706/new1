//
// Created by aweter on 25-6-2.
//
#include "main.h"
#include "PWM.h"
extern TIM_HandleTypeDef htim1;

void setLeftWheelPWM(int leftWheelPWM)
{
    if (leftWheelPWM>=0)//正转
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
    }
    if (leftWheelPWM<0)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);//反转
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
        leftWheelPWM=-leftWheelPWM;//强制为正数
    }
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,leftWheelPWM);
}

void setRightWheelPWM(int rightWheelPWM)
{
    if (rightWheelPWM>=0)//正转
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
    }
    if (rightWheelPWM<0)//反转
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
        rightWheelPWM=-rightWheelPWM;//强制为正数
    }
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,rightWheelPWM);
}