/*
 * uartsoft.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 
 #include "uartsoft.h"
 
  void UARTDelay(__IO uint32_t u32_timedelay)
 {
	 while(u32_timedelay--);
 }
 

 void UART_Init(void)
 {
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 EXTI_InitTypeDef  EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	 EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	 
	 UARTDelay(7500);

 }
 
 #define UARTON	GPIO_SetBits(GPIOA,GPIO_Pin_9);UARTDELAY
 #define UARTOFF GPIO_ResetBits(GPIOA,GPIO_Pin_9);UARTDELAY
 #define UARTDELAY UARTDelay(800)
 #define UARTDELAYRX UARTDelay(800)

 #define UARTREAD GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
 
 void UART_Send(uint8_t u8_data)
 {
	 UARTON;
	 UARTDelay(20000);
	 UARTOFF;
	 if(u8_data&0x01)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x02)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x04)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x08)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x10)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x20)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x40)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	 if(u8_data&0x80)
	 {
		 UARTON;
	 }
	 else
	 {
		 UARTOFF;
	 }
	  UARTON;
 }
 
 uint8_t UART_Read(void)
 {
	 uint8_t u8_temp,i;
	 UARTDelay(800);
	 for(i=0;i<8;i++)
	 {
		 UARTDELAYRX;
		 if(UARTREAD)
		 {
			 u8_temp|=1<<i;
		 }
		 else
		 {
			 u8_temp&=~(1<<i);
		 }
	 }

	 return u8_temp;
	 
 }
 
 void UART_printf(const char *str,uint8_t l)
 {
	 uint8_t i,s;
	 s=0;
	 for(i=0;i<l-1;i++)
	 {
		 switch(s)
		 {
			 case 0:
			 {
				 if(str[i]==0x5c)
				 {
					 s=1;
					 break;
				 }
				 else
				 {
					 UART_Send(str[i]);
				 }
			 }
			 break;
			 case 1:
			 {
				 if(str[i]=='n')
				 {
					 UART_Send(0x0A);
					 s=0;
					 break;
				 }
				 else
				 {
					 UART_Send(0x5c);
					 UART_Send(str[i]);
					 s=0;
				 }
			 }
			 break;
		 }
	 }
 }