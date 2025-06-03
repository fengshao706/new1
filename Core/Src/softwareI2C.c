//
// Created by jiarui on 25-6-2.
//
#include "main.h"
#include "stm32f1xx_hal_gpio.h"

void i2cDelay()
{
    for (int i=1;i<=10;i++)
    {

    }
}

void switchToInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {//改变引脚模式
    HAL_GPIO_DeInit(GPIOx, GPIO_Pin); //清除旧配置
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;     // 输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;         // 启用内部上拉
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void switchToOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {//改变引脚模式
    HAL_GPIO_DeInit(GPIOx, GPIO_Pin); //清除旧配置
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;     // 输入出模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;         // 启用内部上拉
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void i2cStart()//PB3为SDA，PB4为SCL
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    i2cDelay();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
    i2cDelay();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
}

void i2cStop()
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    i2cDelay();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
}

int i2cTransmitSingleData(uint8_t data,uint32_t timeout)
{
    for (uint8_t i=0;i<8;i++){  //发送设备地址
        if (data & 0x80)//取最高位发送
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
        }else
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
        }
        i2cDelay();
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
        i2cDelay();
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
        i2cDelay();
        data <<= 1;
    }
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
    i2cDelay();
    switchToInput(GPIOB,GPIO_PIN_3);//改为输入模式，为ACK位做准备
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    i2cDelay();
    int32_t time=HAL_GetTick();
    while (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)!=GPIO_PIN_RESET)//处理ack位
    {
        if (HAL_GetTick()-time>timeout)
        {
            i2cStop();
            return -1;//未应答时退出
        }
    }
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
    switchToOutput(GPIOB,GPIO_PIN_3);//改为输出模式
    return 0;
}

int i2cReceive(uint8_t deviceAddress,uint8_t registerAddress,uint8_t* buffer,uint8_t size,uint32_t timeout)
{
    i2cStart();
    if (i2cTransmitSingleData(deviceAddress & 0xfe,timeout)==-1)//发送设备地址
    {
        return -1;
    }
    if (i2cTransmitSingleData(registerAddress,timeout)==-1)//发送寄存器地址
    {
        return -1;
    }
    i2cStart();
    if (i2cTransmitSingleData(deviceAddress | 0x01,timeout)==-1)//发送读地址
    {
        return -1;
    }

    switchToInput(GPIOB,GPIO_PIN_3);
    for (int j=0;j<size;j++)
    {
        uint8_t temp=0;
        for (int i=0;i<8;i++)
        {
            temp <<=1;
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
            i2cDelay();
            if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
            {
                temp |=0x01;
            }
            i2cDelay();
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
        }
        buffer[j]=temp;
        if (j==size-1)
        {
            switchToOutput(GPIOB,GPIO_PIN_3);//改为输出模式，为ACK位做准备
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);//发送NAK
            i2cDelay();
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
            i2cDelay();
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
            i2cDelay();
            i2cStop();
            return 0;
        }
        switchToOutput(GPIOB,GPIO_PIN_3);//改为输出模式，为ACK位做准备
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);//发送ACK
        i2cDelay();
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
        i2cDelay();
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
        i2cDelay();
    }
    return 0;
}



int i2cTransmit(uint8_t deviceAddress,uint8_t registerAddress,uint8_t * data,uint8_t size,uint32_t timeout)//软件I2C发送地址
{
    i2cStart();
    if (i2cTransmitSingleData(deviceAddress,timeout)==-1)//发送设备地址，若ACK未应答则退出发送
    {
        return -1;
    }

    if (i2cTransmitSingleData(registerAddress,timeout)==-1)//发送寄存器地址，若ACK未应答则退出发送
    {
        return -1;
    }
    for (int j=0;j<size;j++){
        uint8_t temp=data[j];//定义临时变量以存储数据
        if (i2cTransmitSingleData(temp,timeout)==-1)//发送具体数据
        {
            return -1;
        }
    }
    i2cStop();
    return 0;
}










