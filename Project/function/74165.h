/*
 * 74165.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 #ifndef IC74165_H
 #define IC74165_H
 
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stdbool.h" 
 
 #define CLK165Per			RCC_APB2Periph_GPIOA	
 #define CLK165Pin			GPIO_Pin_1
 #define CLK165Port			GPIOA
 
 #define LOAD165Per			RCC_APB2Periph_GPIOA	
 #define LOAD165Pin			GPIO_Pin_2
 #define LOAD165Port		GPIOA 
 
 #define DATA165Per			RCC_APB2Periph_GPIOB	
 #define DATA165Pin			GPIO_Pin_12
 #define DATA165Port		GPIOB
 
 #define CLK165ON				GPIO_SetBits(CLK165Port,CLK165Pin);DELAY74165
 #define CLK165OFF			GPIO_ResetBits(CLK165Port,CLK165Pin);DELAY74165
 
 #define LOAD165ON			GPIO_SetBits(LOAD165Port,LOAD165Pin)
 #define LOAD165OFF			GPIO_ResetBits(LOAD165Port,LOAD165Pin)
 #define LOAD165DATAIN	CLK165ON;LOAD165OFF;DELAY74165;LOAD165ON
 
 #define DATA165IN			GPIO_ReadInputDataBit(DATA165Port,DATA165Pin)
 
 #define DELAY74165			Delay74165(500);
 
 void GPIO74165Init(void);
 void ReadInput(uint8_t *u8_out,uint8_t u8_length);
 void Delay74165(uint32_t u32_timeout);
 void comparedata(uint8_t *u8_n,uint8_t *u8_p,uint8_t *u8_compare);

 #endif
 
 