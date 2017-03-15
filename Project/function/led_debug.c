/*
 * leddebug.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 #include "led_debug.h"
 
 void Led_DebugInit(void)
 {
	 GPIO_InitTypeDef GPIO_InitStruture = {(GPIO_Pin_13|GPIO_Pin_14) ,GPIO_Speed_50MHz,GPIO_Mode_Out_PP};
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	 GPIO_Init(GPIOC,&GPIO_InitStruture);
 }
 
 void AlarmInit(void)
 {
		NVIC_InitTypeDef  NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef  EXTI_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	 EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
 }