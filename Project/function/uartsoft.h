/*
 * uartsoft.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 #ifndef UART_SOFT_H
 #define UART_SOFT_H
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stdbool.h"
 #include "misc.h"
 #include "stm32f10x_exti.h"
 
 
	 void UART_Init(void);
	 void UART_Send(uint8_t u8_data);
	uint8_t UART_Read(void);
 void UART_printf(const char *str,uint8_t l);


 #endif
 
 