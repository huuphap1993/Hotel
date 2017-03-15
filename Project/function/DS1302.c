#include "DS1302.h"

GPIO_InitTypeDef  DS1302_Pin;

void DS1302_Init()
{
  RCC_AHB1PeriphClockCmd(RCC_DS1302, ENABLE);
  DS1302_Pin.GPIO_Pin = DS1302_DataPin | DS1302_CLKPin|DS1302_CEPin;
  DS1302_Pin.GPIO_Mode = GPIO_Mode_OUT;
  DS1302_Pin.GPIO_OType = GPIO_OType_PP;
  DS1302_Pin.GPIO_Speed = GPIO_Speed_100MHz;
  DS1302_Pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(DS1302_Port, &DS1302_Pin);
}

void DS1302In()
{
  RCC_AHB1PeriphClockCmd(RCC_DS1302, ENABLE);
  DS1302_Pin.GPIO_Pin = DS1302_DataPin;
  DS1302_Pin.GPIO_Mode = GPIO_Mode_IN;
  DS1302_Pin.GPIO_OType = GPIO_OType_PP;
  DS1302_Pin.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DS1302_Port,&DS1302_Pin);
}

void DS1302Out()
{
  RCC_AHB1PeriphClockCmd(RCC_DS1302, ENABLE);
  DS1302_Pin.GPIO_Pin = DS1302_DataPin;
  DS1302_Pin.GPIO_Mode = GPIO_Mode_OUT;
  DS1302_Pin.GPIO_OType = GPIO_OType_PP;
  DS1302_Pin.GPIO_Speed = GPIO_Speed_100MHz;
  DS1302_Pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(DS1302_Port, &DS1302_Pin);
}

static int ReadByte()
{
  int byte;
  int i;
  byte=0;
  RTC_ReadInit;
  for(i=0;i!=8;i++)
  {
    DS1302_CLK0;
    RTC_Delay;
    if(GPIO_ReadInputDataBit(DS1302_Port,DS1302_DataPin))
    {
      byte|=1<<i;
    }
    RTC_Delay;
    DS1302_CLK1;
    RTC_Delay;
  }
 return byte;
}
       
void WriteByte(int Byte)
{
  int i;
  RTC_WriteInit;
  RTC_Delay;
  for(i=0;i!=8;i++)
  {
    if(((Byte>>i)&&0x01))
    {
      RTC_DATAOUT1;
    }
    else
    {
      RTC_DATAOUT0;
    }
  }
  RTC_Delay;
}

int ReadSeconds()
{
  int s;
  RTC_RESET;
  WriteByte(0x81);
  s=ReadByte();
  return s;
}
       
int ReadMinutes()
{
  int m;
  RTC_RESET;
  WriteByte(0x83);
  m=ReadByte();
  return m;
}
       
int ReadHours()
{
  int h;
  RTC_RESET;
  WriteByte(0x85);
  h=ReadByte();
  return h;
}

void DS1302_delay()
{
  int i;
  i=1000;
  while(i--);
}

void WriteSeconds(int Byte)
{
  RTC_RESET;
  WriteByte(0x80);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteMinutes(int Byte)
{
  RTC_RESET;
  WriteByte(0x82);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteHours(int Byte)
{
  RTC_RESET;
  WriteByte(0x84);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteDate(int Byte)
{
  RTC_RESET;
  WriteByte(0x86);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteMonth(int Byte)
{
  RTC_RESET;
  WriteByte(0x84);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteDay(int Byte)
{
  RTC_RESET;
  WriteByte(0x84);
  WriteByte(Byte);
  RTC_Delay;
}

void WriteYears(int Byte)
{
  RTC_RESET;
  WriteByte(0x84);
  WriteByte(Byte);
  RTC_Delay;
}