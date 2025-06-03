//
// Created by aweter on 25-6-3.
//

#ifndef SOFTWAREI2C_H
#define SOFTWAREI2C_H

void i2cDelay();

void switchToInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) ;//改变引脚模式

void switchToOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) ;//改变引脚模式

void i2cStart();//PB3为SDA，PB4为SCL

void i2cStop();

int i2cTransmitSingleData(uint8_t data,uint32_t timeout);

int i2cReceive(uint16_t deviceAddress,uint16_t registerAddress,uint8_t* buffer,uint16_t size,uint32_t timeout);

int i2cTransmit(uint16_t deviceAddress,uint16_t registerAddress,uint8_t * data,uint16_t size,uint32_t timeout);//软件I2C发送地址

#endif //SOFTWAREI2C_H
