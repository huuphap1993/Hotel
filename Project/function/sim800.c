/*
 * sim800.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 #include "sim800.h"
 
static uint8_t simstatus = 0;
uint8_t u8_rxstatus,u8_count[1];
char messagecmd[120];
uint8_t CheckCMD(void);
extern RTC_time TimeNow;
uint8_t Numconvert(uint8_t *u8_in);

void sim800Init(void)
{
	uint32_t timeout;
	UART_Init();
	u8_rxstatus=0;
	timeout=0x1ffffff;
	simstatus=NOT_CONNECT;
	while(timeout--)
	{

	}
	timeout=0xffff;
	UART_printf("AT\n",4);
	while(timeout--)
	{
		if(simstatus==COMMAND_OK)
			break;
	}
		timeout=0xffff;
	
	UART_printf("AT+CLTS=1\n",11);
	//UART_printf("\nAT+IPR=9600\n",13);
	while(timeout--)
	{
		if(simstatus==COMMAND_OK)
			break;
	}
	if(simstatus!=COMMAND_OK)
	{
		LED_Debug1ON;
		return ;
	}
	simstatus=COMMAND_NONE;
	
	UART_printf("ATE0\n",6);
	while(timeout--)
	{
		if(simstatus==COMMAND_OK)
			break;
	}
	UART_printf("AT&W\n",6);
	while(timeout--)
	{
		if(simstatus==COMMAND_OK)
			break;
	}
	
}

uint8_t u8_SIMCheck(uint8_t u8_datain,char* u8_dataout,uint8_t u8_status,uint8_t* u8_dataoutcount)
{
	switch(u8_status)
	{
		case 0: 
			if(u8_datain=='>')
			{
				simstatus=SIM_SMSWAIT;
			}
			if(u8_datain==0x0D)
				return 1;
			return 0;
		case 1:
			if(u8_datain==0x0A)
				return 2;
			return 0;
		case 2:
			if(u8_datain==0x0D)
				return 3;
			u8_dataout[u8_dataoutcount[0]]=u8_datain;
			*u8_dataoutcount=u8_dataoutcount[0]+1;
			return 2;
		case 3:
			if(u8_datain==0x0D)
				return 3;
			if(u8_datain==0x0A)
				return 4;
			return 0;
		case 5:
			if(u8_datain==0x0D)
				return 6;
			return 0;
		case 6:
			if(u8_datain==0x0A)
				return 7;
		case 7:
			if(u8_datain==0x3E)
				return 8;
			return 0;
		case 8:
			if(u8_datain==0x20)
				return 9;
			return 0;
	}
	return 0;
}

void v_checkdata(uint8_t u8_datain)
{
	u8_rxstatus=u8_SIMCheck(u8_datain,messagecmd,u8_rxstatus,u8_count);
	if(u8_rxstatus!=2)
	{
		u8_count[0]=0;
	}
	if(u8_rxstatus==4)
	{
		simstatus=CheckCMD();
		u8_rxstatus=0;
	}
}

uint8_t CheckCMD(void)
{
	uint8_t timeget[2];
	if(messagecmd[0]=='O' && messagecmd[1]=='K')
	{
		return COMMAND_OK;
	}
	if(messagecmd[0]=='E' && messagecmd[1]=='R' && messagecmd[2]=='R' && messagecmd[3]=='O' && messagecmd[4]=='R')
	{
		return SIM_ERROR;
	}
	
	if(messagecmd[0]=='+' && messagecmd[1]=='C' && messagecmd[2]=='C' && messagecmd[3]=='L' && messagecmd[4]=='K')
	{
		timeget[0]=messagecmd[8];
		timeget[1]=messagecmd[9];
		TimeNow.u8_year=Numconvert(timeget);
		timeget[0]=messagecmd[11];
		timeget[1]=messagecmd[12];
		TimeNow.u8_month=Numconvert(timeget);
		timeget[0]=messagecmd[14];
		timeget[1]=messagecmd[15];
		TimeNow.u8_day=Numconvert(timeget);
		timeget[0]=messagecmd[17];
		timeget[1]=messagecmd[18];
		TimeNow.u8_hour=Numconvert(timeget);
		timeget[0]=messagecmd[20];
		timeget[1]=messagecmd[21];
		TimeNow.u8_minute=Numconvert(timeget);
		timeget[0]=messagecmd[23];
		timeget[1]=messagecmd[24];
		TimeNow.u8_second=Numconvert(timeget);
		return TIMEGET_OK;
	}
	if(messagecmd[0]=='>' && messagecmd[1]==' ')
	{
		return SIM_SMSWAIT;
		
	}
	
	return COMMAND_NONE;
}

void SIM_CMD1(uint8_t u8_cmd,char* mess,uint8_t leng)
{
		uint32_t timeout;
	uint8_t u8_i;
	switch(u8_cmd)
	{
		case SIM_AT:
		{
			UART_printf("AT\n",3);
		}
		break;
		
		case SIM_ATE0:
		{
			UART_printf("ATE0\n",5);
		}
		break;
		
		case SIM_ATE1:
		{
			UART_printf("ATE1\n",5);
		}
		break;
		
		case SIM_SMSTEXT:
		{
			UART_printf("AT+CMGF=1\n",11);
		}
		break;
		
		case SIM_SMSNUMBER:
		{
					UART_printf("AT+CMGF=1\n",11);
						timeout=0xffff;
						while(timeout--)
						{
						if(simstatus==COMMAND_OK)
							break;
						}
			UART_printf("AT+CMGS=",9);
			UART_Send('"');
			UART_printf("+84903146779",13);
			UART_Send('"');
			UART_Send(0x0A);
			timeout=0xffff;
			while(timeout--)
			{
					if(simstatus==SIM_SMSWAIT)
					break;
			}
			for(u8_i=0;u8_i<leng;u8_i++)
			{
				
				UART_Send(mess[u8_i]);
			}
			
			UART_Send(0x1A);
			UART_Send(0x0D);
		}
		break;
		
		case SIM_MONEY:
		{
			UART_printf("AT+CMGF=1\n",11);
		}
		break;
		
	}
}

uint8_t Numconvert(uint8_t *u8_in)
{
	return ((u8_in[0]-0x30)*10)+(u8_in[1]-0x30);
}

void Gettime(void)
{
	uint32_t timeout;
	UART_printf("AT+CCLK?\n",10);
	timeout=0xfffff;
	while(timeout--)
	{
		if(simstatus==COMMAND_OK)
			break;
	}
		timeout=0xfffff;
	while(timeout--)
	{
		if(simstatus==TIMEGET_OK)
			break;
	}
	
}

uint16_t converttime(void)
{
	return (TimeNow.u8_hour*60)+TimeNow.u8_minute;
}

void v_MessengerConvert(uint32_t *datarelay,uint8_t u8_port,char* mess,uint8_t *leng)
{
	uint8_t i1,i2;
	uint8_t l;
	uint16_t relayon,relayoff;
	l=0;
				
		for(i2=0;i2<20;i2++)
		{
			if(datarelay[i2]&0xC0000000)
			{
				if(l==0)
				{
				mess[l]='P';l++;
				mess[l]=(u8_port+1/10)+0x30;l++;
				mess[l]=(u8_port+1%10)+0x30;l++;
				mess[l]=' ';l++;
				}
				relayon=datarelay[i2]>>14;
				relayoff=datarelay[i2]&0x3FFF;
				
				mess[l]=((relayon/60)/10)+0x30;l++;
				mess[l]=((relayon/60)%10)+0x30;l++;
				mess[l]='h';l++;
				mess[l]=((relayon%60)/10)+0x30;l++;
				mess[l]=((relayon%60)%10)+0x30;l++;
				mess[l]='-';l++;
				mess[l]='>';l++;
				mess[l]=((relayoff/60)/10)+0x30;l++;
				mess[l]=((relayoff/60)%10)+0x30;l++;
				mess[l]='h';l++;
				mess[l]=((relayoff%60)/10)+0x30;l++;
				mess[l]=((relayoff%60)%10)+0x30;l++;
				mess[l]='.';l++;
			}
			else
				if(datarelay[i2]&0x80000000)
			{
				if(l==0)
				{
				mess[l]='P';l++;
				mess[l]=(u8_port+1/10)+0x30;l++;
				mess[l]=(u8_port+1%10)+0x30;l++;
				mess[l]=' ';l++;
				}
				relayon=datarelay[i2]>>14;
				mess[l]='O';l++;
				mess[l]='N';l++;
				mess[l]=((relayon/60)/10)+0x30;l++;
				mess[l]=((relayon/60)%10)+0x30;l++;
				mess[l]=':';l++;
				mess[l]=((relayon%60)/10)+0x30;l++;
				mess[l]=((relayon%60)%10)+0x30;l++;
				mess[l]='.';l++;
			}
			else
				
			if(datarelay[i2]&0x40000000)
			{
				if(l==0)
				{
				mess[l]='P';l++;
				mess[l]=(u8_port+1/10)+0x30;l++;
				mess[l]=(u8_port+1%10)+0x30;l++;
				mess[l]=' ';l++;
				}
				relayoff=datarelay[i2]&0x3FFF;
				mess[l]='O';l++;
				mess[l]='F';l++;
				mess[l]='F';l++;
				mess[l]=((relayoff/60)/10)+0x30;l++;
				mess[l]=((relayoff/60)%10)+0x30;l++;
				mess[l]='.';l++;
				mess[l]=((relayon%60)/10)+0x30;l++;
				mess[l]=((relayon%60)%10)+0x30;l++;
				mess[l]='.';l++;
			}
			else
			{
				break;
			}
			
		}
		*leng=l;
}

void v_matchmessage(char* mess,char* sendmess,uint8_t leng,uint8_t *lengs)
{
	uint8_t i,i1;
	uint32_t tout;
	if((leng+*lengs)>225)
	{
		for(i=leng;i<225;i++)
		{
			sendmess[*lengs+i]=mess[i-leng];
		}

		SIM_CMD1(SIM_SMSNUMBER,sendmess,*lengs);
		*lengs=(leng+*lengs)-225;
		for(i=0;i<(leng-*lengs);i++)
		{
			sendmess[i]=mess[leng-i];
		}
	}
	else
	{
		*lengs=leng+*lengs;
		for(i=0;i<leng;i++)
		{
			sendmess[i]=mess[i];
		}
	}
}

uint8_t u8_GetSimStatus(void)
{
	return simstatus;
}

