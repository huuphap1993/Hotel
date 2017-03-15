/*
 * led_debug.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 #ifndef LED_DEBUG_H
 #define LED_DEBUG_H
 
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stdbool.h" 
 #define LED_Debug1ON		GPIO_SetBits(GPIOC,GPIO_Pin_13)
 #define LED_Debug1OFF	GPIO_ResetBits(GPIOC,GPIO_Pin_13)
 #define LED_DebugRead	GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)
 
 
 #define LED_Debug2ON	GPIO_SetBits(GPIOC,GPIO_Pin_14)
 #define LED_Debug2OFF	GPIO_ResetBits(GPIOC,GPIO_Pin_14)
 
 
 
 
  void Led_DebugInit(void);
 void AlarmInit(void);

 #endif
 