/*
 * DS1307.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 #include "DS1307.h"
 
 #define I2Cx	I2C1
 #define I2CSDA1	GPIO_SetBits(GPIOB,GPIO_Pin_7)
 #define I2CSDA0	GPIO_ResetBits(GPIOB,GPIO_Pin_7)
 #define I2CReadSDA	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
 
 #define I2CSCL1	GPIO_SetBits(GPIOB,GPIO_Pin_6)
 #define I2CSCL0	GPIO_ResetBits(GPIOB,GPIO_Pin_6)
 #define I2CDelay	DelayI2C(3000)
 
 #define I2CChange	I2CSCL1;I2CDelay;I2CSCL0
 #define I2CStart		I2CSCL1;I2CDelay;I2CSDA1;I2CDelay;I2CSDA0;I2CDelay;I2CSCL0;I2CDelay
 #define I2CRepStart	I2CSDA1;I2CSCL1;I2CDelay;I2CSDA0;I2CDelay
 #define I2CStop		I2CSCL1;I2CDelay;I2CSDA1
 
 void DelayI2C(uint32_t u32_timeout)
	{
		while(u32_timeout--);
	}
	
 void i2c_init()
{
    // Initialization struct
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		I2CSDA1;
		I2CSCL1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
		DelayI2C(30000);
}

void i2c_transmitinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void i2c_receiveinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void i2c_transmit(uint8_t addr,uint8_t *buff,uint8_t length)
{
	//start
	I2CStart;
	I2CDelay;
	if(addr&0x80)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x40)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x20)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x10)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x08)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x04)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x02)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	if(addr&0x01)
	{
		I2CSDA1;
	}
	else
	{
		I2CSDA0;
	}
	I2CChange;
	i2c_receiveinit();
	
	
}

