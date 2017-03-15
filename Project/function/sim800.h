/*
 * sim800.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 
 #ifndef SIM800_H
 #define SIM800_H
 
 #include "stm32f10x.h"
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_usart.h"
 #include "misc.h"
 #include "uartsoft.h"
 #include "led_debug.h"
 
 typedef enum
 {
	 NOT_CONNECT,
	 CONNECTED,
	 SIM_ERROR,
	 COMMAND_OK,
	 COMMAND_NONE,
	 SIM_SMSWAIT,
	 TIMEGET_OK,
 }SimStatus;

typedef struct
{
	uint8_t u8_year;
	uint8_t u8_month;
	uint8_t u8_day;
	uint8_t u8_hour;
	uint8_t u8_minute;
	uint8_t u8_second;
}RTC_time;
 
 typedef enum
 {
	 SIM_AT,
	 SIM_ATE0,
	 SIM_ATE1,
	 SIM_SMSTEXT,
	 SIM_SMSNUMBER,
	 SIM_MONEY
 }Simcmd;
  void sim800Init(void);
	void SIM_CMD(uint8_t u8_cmd,const char* mess,uint8_t length);
void v_checkdata(uint8_t u8_datain);
void Gettime(void);
uint16_t converttime(void);
 void v_MessengerConvert(uint32_t *datarelay,uint8_t u8_port,char* mess,uint8_t *leng);
void v_matchmessage(char* mess,char* sendmess,uint8_t leng,uint8_t *lengs);
 uint8_t u8_GetSimStatus(void);
void SIM_CMD1(uint8_t u8_cmd,char* mess,uint8_t leng);
uint32_t SetSIMstatus(uint8_t u8_status);

 
 
 #endif
 
 