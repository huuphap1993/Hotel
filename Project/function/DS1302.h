
#define RCC_DS1302 RCC_AHB1Periph_GPIOD
#define DS1302_Port GPIOD

#define DS1302_DataPin GPIO_Pin_2
#define DS1302_CLKPin GPIO_Pin_3
#define DS1302_CEPin GPIO_Pin_4

#define DS1302_CE1 GPIO_SetBits(DS1302_Port,DS1302_CEPin)
#define DS1302_CE0 GPIO_ResetBits(DS1302_Port,DS1302_CEPin)
#define DS1302_CLK1 GPIO_SetBits(DS1302_Port,DS1302_CLKPin)
#define DS1302_CLK0 GPIO_ResetBits(DS1302_Port,DS1302_CLKPin)
#define DS1302_DATA1  GPIO_SetBits(DS1302_Port,DS1302_DataPin)
#define DS1302_DATA0  GPIO_ResetBits(DS1302_Port,DS1302_DataPin)

#define RTC_DATAOUT1 DS1302_CLK1;RTC_Delay;DS1302_DATA1;RTC_Delay;DS1302_CLK0;RTC_Delay
#define RTC_DATAOUT0 DS1302_CLK1;RTC_Delay;DS1302_DATA0;RTC_Delay;DS1302_CLK0;RTC_Delay
#define RTC_RESET DS1302_CLK0;RTC_Delay;DS1302_CE0;RTC_Delay;DS1302_CE1;RTC_Delay
#define RTC_ReadInit DS1302In()
#define RTC_WriteInit DS1302Out()
#define RTC_Delay DS1302_delay()

void DS1302_Init();
void DS1302In();
void DS1302Out();
static int ReadByte();
void WriteByte(int Byte);

int ReadSeconds();
int ReadMinutes();
int ReadHours();
void DS1302_delay();
