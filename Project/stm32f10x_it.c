/**
  ******************************************************************************
  * @file    RTC/Calendar/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "diskio.h"
#include "sdcard.h"
#include "stm32f10x_exti.h"
#include "uartsoft.h"
#include "sim800.h"
#include "project.h"
#include "74165.h"
#include "diskio.h"
#include "ff.h"
#include "sdcard.h"
#include "log.h"

extern 	uint8_t datainp[5];
extern 	uint8_t datainn[5];
				uint8_t datac[5];

uint32_t timeout1;

extern RTC_time TimeNow;
RTC_time TimePart;
uint32_t u32_timecount;
uint8_t k[120],c;
char numcon[10]={'0','1','2','3','4','5','6','7','8','9'};
uint16_t Now;
uint32_t count[40]={0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,};
uint32_t countrx[40]={0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
uint32_t powertime;
uint32_t alarmtout;
char c_mess[290],s_mess[290];


	
	char FlagError[6];

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TimeDisplay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	int i,i1;
	u32_timecount++;
	if(!(u32_timecount%200))
	{
		if(!FlagError[SD_NotExist]==FR_OK)
		{
			FlagError[SD_NotExist]=SD_cardInit();
		}
		if(u8_GetSimStatus()==SIM_ERROR)
		{
			FlagError[SIM_ErrorCommand]=1;
		}

		
	}
	if(!(u32_timecount%100))
	{
		if(LED_DebugRead)
		{
			LED_Debug1OFF;
		}
		else
		{			
			LED_Debug1ON;
		}
		Gettime();
		
		if(TimeNow.u8_year<17)
		{
			FlagError[TIME_ERROR]=1;
		}
		else
		{
			FlagError[TIME_ERROR]=1;
		}
		
		
		Now=converttime();
		ReadInput(datainn,5);
		comparedata(datainn,datainp,datac);
		*datainp=*datainn;
		
		if(!(datainn[4]>>7)&0x01)
		{
			powertime++;
			if(powertime>3000)
			{
				writelog(40,1);
				s_mess[0]='M';s_mess[1]='A';s_mess[2]='T';s_mess[3]='D';s_mess[4]='I';s_mess[5]='E';s_mess[6]='N';s_mess[7]=':';
				s_mess[8]=numcon[(i*8+i1+1)/10];
				s_mess[9]=numcon[(i*8+i1+1)%10];
				s_mess[10]=':';
				s_mess[11]=numcon[TimeNow.u8_hour/10];
				s_mess[12]=numcon[TimeNow.u8_hour%10];
				s_mess[13]='h';
				s_mess[14]=numcon[TimeNow.u8_minute/10];
				s_mess[15]=numcon[TimeNow.u8_minute%10];
				s_mess[16]='m';
				s_mess[17]=numcon[TimeNow.u8_day/10];
				s_mess[18]=numcon[TimeNow.u8_day%10];
				s_mess[19]='/';
				s_mess[20]=numcon[TimeNow.u8_month/10];
				s_mess[21]=numcon[TimeNow.u8_month%10];
				s_mess[22]='/';
				s_mess[23]=numcon[TimeNow.u8_year/10];
				s_mess[24]=numcon[TimeNow.u8_year%10];
				SIM_CMD1(SIM_SMSNUMBER,s_mess,25);
			}
		}
		else
		{
			powertime=0;
		}
		
		if((datainn[4]>>6)&0x01)
		{
				timeout1=600;
		}
		else
		{
				timeout1=300;
		}
		
		if((datainn[4]>>5)&0x01)
		{
			alarmtout++;
			if(alarmtout>30)
			{
				alarmtout=0;
				writelog(37,1);
				s_mess[0]='C';s_mess[1]='H';s_mess[2]='U';s_mess[3]='O';s_mess[4]='N';s_mess[5]='G';s_mess[6]='1';s_mess[7]=':';
				s_mess[8]=numcon[(i*8+i1+1)/10];
				s_mess[9]=numcon[(i*8+i1+1)%10];
				s_mess[10]=':';
				s_mess[11]=numcon[TimeNow.u8_hour/10];
				s_mess[12]=numcon[TimeNow.u8_hour%10];
				s_mess[13]='h';
				s_mess[14]=numcon[TimeNow.u8_minute/10];
				s_mess[15]=numcon[TimeNow.u8_minute%10];
				s_mess[16]='m';
				s_mess[17]=numcon[TimeNow.u8_day/10];
				s_mess[18]=numcon[TimeNow.u8_day%10];
					
				s_mess[19]='-';
				s_mess[20]=numcon[TimeNow.u8_month/10];
				s_mess[21]=numcon[TimeNow.u8_month%10];
				s_mess[22]='/';
				s_mess[23]=numcon[TimeNow.u8_year/10];
				s_mess[24]=numcon[TimeNow.u8_year%10];
				
				SIM_CMD1(SIM_SMSNUMBER,s_mess,25);
			}
										
		}
		else
		{
			alarmtout=0;
		}

		
		for(i=0; i<4 ; i++)
		{
				for(i1=0;i1<8;i1++)
					{
						if(datac[i]&(1<<i1))
							{
									if((!(datainn[i]>>i1)&0x01))
									{
										
										writelog(i*8+i1,1);
										count[i*8+i1]=0;
									}
									else
									{
										writelog(i*8+i1,0);
										countrx[i*8+i1]=0x00;
										
									}
							}
							else
							{
								if((!(datainn[i]>>i1)&0x01))
								{
									countrx[i*8+i1]=0x30;
										if(count[i*8+i1]<timeout1)
											{
												count[i*8+i1]++;
											}
											else
											{
													if(count[i*8+i1]<(timeout1+0x40))
													{
														s_mess[0]='P';s_mess[1]='H';s_mess[2]='O';s_mess[3]='N';s_mess[4]='G';s_mess[5]=':';
														if(i==1)
														{
														s_mess[6]=numcon[(i*8+i1+1)/10];
														s_mess[7]=numcon[(i*8+i1+1)%10];
														}
														else
														{
														s_mess[6]=numcon[(i*8+i1)/10];
														s_mess[7]=numcon[(i*8+i1)%10];
															
														}
														s_mess[8]=':';
														s_mess[9]=numcon[TimeNow.u8_hour/10];
														s_mess[10]=numcon[TimeNow.u8_hour%10];
														s_mess[11]='h';
														s_mess[12]=numcon[TimeNow.u8_minute/10];
														s_mess[13]=numcon[TimeNow.u8_minute%10];
														s_mess[14]=' ';
														s_mess[15]=numcon[TimeNow.u8_day/10];
														s_mess[16]=numcon[TimeNow.u8_day%10];
														s_mess[17]='/';
														s_mess[18]=numcon[TimeNow.u8_month/10];
														s_mess[19]=numcon[TimeNow.u8_month%10];
														s_mess[20]='/';
														s_mess[21]=numcon[TimeNow.u8_year/10];
														s_mess[22]=numcon[TimeNow.u8_year%10];
														s_mess[23]=':';

										
														s_mess[24]='V';
														s_mess[25]='A';
														s_mess[26]='O';
														SIM_CMD1(SIM_SMSNUMBER,s_mess,27);
														count[i*8+i1]=(timeout1+0x50);
													}
											}
									}
								else
								{
									count[i*8+i1]=0;
									if(countrx[i*8+i1]<10)
									{
										countrx[i*8+i1]++;
									}
									else
									{
										if(countrx[i*8+i1]<20)
										{
										s_mess[0]='P';s_mess[1]='H';s_mess[2]='O';s_mess[3]='N';s_mess[4]='G';s_mess[5]=':';
										if(i==0)
										{
										s_mess[6]=numcon[(i*8+i1+1)/10];
										s_mess[7]=numcon[(i*8+i1+1)%10];
										}
										else
										{
										s_mess[6]=numcon[(i*8+i1)/10];
										s_mess[7]=numcon[(i*8+i1)%10];
										}

										s_mess[8]=':';
										s_mess[9]=numcon[TimeNow.u8_hour/10];
										s_mess[10]=numcon[TimeNow.u8_hour%10];
										s_mess[11]='h';
										s_mess[12]=numcon[TimeNow.u8_minute/10];
										s_mess[13]=numcon[TimeNow.u8_minute%10];
										s_mess[14]=' ';
										s_mess[15]=numcon[TimeNow.u8_day/10];
										s_mess[16]=numcon[TimeNow.u8_day%10];
										s_mess[17]='/';
										s_mess[18]=numcon[TimeNow.u8_month/10];
										s_mess[19]=numcon[TimeNow.u8_month%10];
										s_mess[20]='/';
										s_mess[21]=numcon[TimeNow.u8_year/10];
										s_mess[22]=numcon[TimeNow.u8_year%10];
										s_mess[23]=':';
										
										s_mess[24]='R';
										s_mess[25]='A';
										countrx[i*8+i1]=0x40;
										SIM_CMD1(SIM_SMSNUMBER,s_mess,26);
										}
										
									}
								}
							}
					}
		}
		
	}
	
	
//	if(TimeNow.u8_day!=TimePart.u8_day)
//	{
//		flag[send_data]=1;
//		TimePart.u8_day=TimeNow.u8_day;
//	}
	//SDTimer_Handler();
	
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{

    
}

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
   {
		 k[c]=UART_Read();
		 v_checkdata(k[c]);
		 
		 c++;
		 if(c>=119)
		 {
			 c=0;
		 }
			EXTI->PR = EXTI_Line10;
   }
 }

 
 void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
   {
		 if(alarmtout>30000)
		 {
			 //flag[alarmtimeout]=1;
			 alarmtout=0;
		 }
			EXTI->PR = EXTI_Line1;
   }
 }
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
