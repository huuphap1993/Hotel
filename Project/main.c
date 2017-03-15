/*
 * main.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "uartsoft.h"
#include "sim800.h"
#include "74165.h"
#include "project.h"
#include "diskio.h"
#include "ff.h"
#include "sdcard.h"
#include "log.h"

RTC_time TimeNow;


uint8_t datainp[5];

uint8_t datainn[5];



int main(void)
{ 
	sim800Init();
	Gettime();
	GPIO74165Init();
	ReadInput(datainp,5);
	if(!(SD_cardInit()==FR_OK))
	{
		
	}
	SysTick_Config(SystemCoreClock / 100);


		while(1)
		{
			
		}
}
