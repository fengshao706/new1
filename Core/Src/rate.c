//
// Created by jiarui on 25-6-4.
//

#include "rate.h"
#include "main.h"

int leftPreCount;
int rightPreCount;
int leftError;
int rightError;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
#define ARR 1000
#define numberOfPulses 11.0 //定义单圈脉冲数
#define reductionOfRatio 20.4  //定义减速比

void rateUpdate()
{
    int leftCurCount=__HAL_TIM_GET_COUNTER(&htim2);
    int rightCurCount=__HAL_TIM_GET_COUNTER(&htim4);

    leftError=leftCurCount-leftPreCount;
    rightError=rightCurCount-rightPreCount;

    if (leftError<-ARR/2) //处理左轮正向回绕
    {
        leftError=ARR-(-leftError);
    }
    if (leftError>ARR/2)//处理左轮负向回绕
    {
        leftError=-ARR+leftError;
    }
    if (rightError<-ARR/2) //处理右轮正向回绕
    {
        rightError=ARR-(-rightError);
    }
    if (rightError>ARR/2)//处理右轮负向回绕
    {
        rightError=-ARR+rightError;
    }
    leftPreCount=leftCurCount;
    rightPreCount=rightCurCount;
}

double getLeftWheelRate()//获取左轮在中断区间转过的圈数
{
    double leftWheelRate=leftError/(numberOfPulses*reductionOfRatio);
    return leftWheelRate;
}

double getRightWheelRate()//获取右轮在中断区间转过的圈数
{
    double rightWheelRate=rightError/(numberOfPulses*reductionOfRatio);
    return rightWheelRate;
}

