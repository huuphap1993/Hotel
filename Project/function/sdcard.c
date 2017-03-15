/*
 * sdcard.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 #include "sdcard.h"
 #include "diskio.h"
 #include "ff.h"
 
 #define BOOL bool
 #define TRUE true
 #define FALSE false
	 #define PATH_BUF_SIZE   80

	 static FATFS g_sFatFs;
static DIR g_sDirObject;
static FILINFO g_sFileInfo;
static FIL g_sFileObject;
 static char g_pcTmpBuf[PATH_BUF_SIZE];

 void delayclk(uint32_t u32_timeout);

#define delayCLK				delayclk(5)		
#define CSDeSelect      GPIO_SetBits(GPIOA,GPIO_Pin_4);delayCLK
#define CSSelect        GPIO_ResetBits(GPIOA,GPIO_Pin_4);delayCLK
#define CLKON           GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define CLKOFF          GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define MOSI1           GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define MOSI0           GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define MISORead        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)

static volatile
DSTATUS Stat = STA_NOINIT;    /* Disk status */

static volatile
UINT Timer1, Timer2;    /* 100Hz decrement timer */

static
BYTE CardType;            /* b0:MMC, b1:SDC, b2:Block addressing */


void delayclk(uint32_t u32_timeout)
{
	while(u32_timeout--);
}

typedef struct {
    uint16_t    year;   /* 1970..2106 */
    uint8_t     month;  /* 1..12 */
    uint8_t     mday;   /* 1..31 */
    uint8_t     hour;   /* 0..23 */
    uint8_t     min;    /* 0..59 */
    uint8_t     sec;    /* 0..59 */
    uint8_t     wday;   /* 0..6 (Sun..Sat) */
} RTCTIME;

DWORD get_fattime (void)
{
    RTCTIME rtc;

    /* Get local time */

    /* Pack date and time into a DWORD variable */
    return    ((DWORD)(rtc.year - 1980) << 25)
            | ((DWORD)rtc.month << 21)
            | ((DWORD)rtc.mday << 16)
            | ((DWORD)rtc.hour << 11)
            | ((DWORD)rtc.min << 5)
            | ((DWORD)rtc.sec >> 1);
}

void softspiinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruture.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruture);
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruture.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruture);
	CSDeSelect;
	
}

uint8_t SPI_SoftWrite(uint8_t u8_MOSI)
{
    uint8_t u8_MISO,u8_i;
    u8_MISO=0;
    delayCLK;
    CLKON;

    for(u8_i=0;u8_i<8;u8_i++)
    {
        if(u8_MOSI & 0x80)
            MOSI1;
        else
            MOSI0;
        u8_MOSI<<=1;
        delayCLK;
        CLKOFF;
        delayCLK;
        u8_MISO<<=1;
        if(MISORead)
            u8_MISO++;
        CLKON;
        delayCLK;

    }
    return u8_MISO;
}


BYTE receiveabyte(void)
{
    return SPI_SoftWrite(0xFF);
}

BYTE wait_ready(UINT wt)
{
    BYTE res;
		uint32_t TIMEA;
    TIMEA=wt*10000;
    receiveabyte();
    do
        res = receiveabyte();
    while((res!=0xff)&& TIMEA--);
    return (res== 0xFF)?1:0;

}

void transmitbytes(const BYTE *buff,UINT btx)
{
    CSSelect;
    do
    {
        SPI_SoftWrite(buff[0]);
        buff++;
    }while(btx-=1);
    SPI_SoftWrite(buff[0]);
    CSDeSelect;
}


int receivedatablock(BYTE *buff,UINT btr)
{
    BYTE token;
		uint32_t TIMEA;
    CSSelect;
    TIMEA = 200*100000;
    do
    {
        token = receiveabyte();
    }while((token==0xFF)&&TIMEA--);

    if(token!= 0xFE)  return 0;
    do
    {
        *buff++=receiveabyte();
        *buff++=receiveabyte();
    }while(btr-=2);
    receiveabyte();
    receiveabyte();
    CSDeSelect;
    return 1;
}

int trasmitdatablock(const BYTE *buff,UINT token)
{
    BYTE resp,wc;
    CSSelect;
    if(!wait_ready(5))
    {
        CSDeSelect;
        return 0;
    }
    SPI_SoftWrite(token);
    if(token!=0xFD)
    {
        wc=0;
        do
        {
            SPI_SoftWrite(*buff++);
            SPI_SoftWrite(*buff++);
        }while(--wc);
        receiveabyte();
        receiveabyte();
        resp=SPI_SoftWrite(0xFF);
        if((resp&0x1F)!=0x05)
            return 0;
    }
    return 1;

}

BYTE SD_Cmd(unsigned char cmd, unsigned long arg)
{
    BYTE i, buffer[11];


    CSSelect;
    if(!wait_ready(5))
        {
            CSDeSelect;
            return 0xFF;
        }

    SPI_SoftWrite(0x40|cmd);
    SPI_SoftWrite(arg>>24);
    SPI_SoftWrite(arg>>16);
    SPI_SoftWrite(arg>>8);
    SPI_SoftWrite(arg);
    i=0xff;
    if(cmd==CMD0)   i=0x95;
    if(cmd==CMD8)   i=0x87;
    SPI_SoftWrite(i);
    if(cmd==CMD12)  receiveabyte();
    i=10;
    do
        buffer[i] = receiveabyte();
    while((buffer[i]&0x80) && --i);
    CSDeSelect;
    return buffer[i];
}

DSTATUS disk_initialize (BYTE drv)
{
    BYTE n,cmd,ty,ocr[4];
    if(drv) return STA_NOINIT;
    softspiinit();
    if(Stat & STA_NODISK)   return Stat;
    CSDeSelect;
    for(n=0;n<11;n++)           SPI_SoftWrite(0xFF);

    ty=0;
    if(SD_Cmd(CMD0,0)==1)
    {
        Timer1=0x03E8;
        if(SD_Cmd(CMD8, 0x1AA)==1)
        {
            CSSelect;
            for(n=0;n<4;n++)    ocr[n]=receiveabyte();
            CSDeSelect;
            if(ocr[2]==0x01 && ocr[3]==0xAA)
            {
                while(SD_Cmd(CMD55,0)<=1 && SD_Cmd(ACMD41,1UL<<30));
                if(Timer1 && SD_Cmd(CMD58,0)==0)
                {
                    for(n=0;n<4;n++)    ocr[n]= receiveabyte();
                    ty=(ocr[0] && 0x40) ? CT_SD2|CT_BLOCK: CT_SD2;
                }
            }
        }

        else
        {
            if(SD_Cmd(ACMD41, 0)<=1)
            {
                ty=CT_SD1;cmd=ACMD41;
            }
            else
            {
                ty=CT_MMC;cmd=CMD1;
            }
            while(Timer1&& SD_Cmd(cmd, 0));
            if(!Timer1 || SD_Cmd(CMD16,512)!=0)
            ty=0;
        }
    }
    CardType=ty;
    CSDeSelect;
    if(ty)
    {
        Stat&=~STA_NOINIT;
    }
    else
    {
        Stat=STA_NOINIT;
    }
    return Stat;

}

DSTATUS disk_status(BYTE drv)
{
    if(drv) return STA_NOINIT;
    return Stat;
}


DRESULT disk_read (BYTE pdrv, BYTE*buff, DWORD sector, BYTE count)
{
    if(pdrv|| !count)    return RES_PARERR;
    if(Stat & STA_NOINIT)   return RES_NOTRDY;

    if(!(CardType&CT_BLOCK))    sector*=512;

    if(count==1)
    {
        if((SD_Cmd(CMD17,sector)==0) && receivedatablock(buff, 512))
        {
            count=0;
        }
    }
    else
    {
        if(SD_Cmd(CMD18,sector)==0)
        {
            do
            {
                if(!receivedatablock(buff, 512))break;
                buff+=512;
            }while(--count);
            SD_Cmd(CMD12, 0);
        }
    }
    CSDeSelect;
    return count? RES_ERROR: RES_OK;
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, BYTE count)
{
    if(pdrv|| !count) return RES_PARERR;
    if(Stat & STA_NOINIT) return RES_NOTRDY;
    if(Stat & STA_PROTECT) return RES_WRPRT;

    if(!(CardType & CT_BLOCK))   sector*=512;
    if(count==1)
    {
        if((SD_Cmd(CMD24,sector)==0) && trasmitdatablock(buff, 0xFE))
        {
            count=0;
        }
    }
    else
    {
        if(CardType & CT_SDC)   SD_Cmd(ACMD23, count);
        if(SD_Cmd(CMD25, sector)==0)
        {
            do
            {
                if(!trasmitdatablock(buff, 0xFC))    break;
                buff+=512;
            }while(--count);
            if(!trasmitdatablock(0, 0xFD))  count=1;
        }
    }
    CSDeSelect;
    return count? RES_ERROR:RES_OK;
}

DRESULT disk_ioctl (BYTE drv,BYTE cmd, void *buff)
{
    DRESULT res;
    BYTE n,csd[16];
    DWORD *dp,st,ed,csize;
    if(drv) return RES_PARERR;
    if(Stat & STA_NOINIT)   return RES_NOTRDY;
    res = RES_ERROR;
    switch(cmd)
    {
    case CTRL_SYNC:
        CSSelect;
        if(wait_ready(5))
            {
                CSDeSelect;
                res=RES_OK;
            }
        break;
    case GET_SECTOR_COUNT:
    {
        if((SD_Cmd(CMD9, 0)==0) && receivedatablock(csd,16))
        {
            if((csd[0]>>6)==1)
            {
                csize=csd[9]+((WORD)csd[8]<<8)+((DWORD)(csd[7]&63)<<16)+1;
                *(DWORD*)buff=csize<<10;
            }
            else
            {
                n=(csd[5]&15)+((csd[10]&128)>>7) +((csd[9]&3)<<1)+2;
                csize=(csd[8]>>6)+((WORD)csd[7]<<2)+((WORD)(csd[6]&3)<<10)+1;
                *(DWORD*)buff=csize<<(n-9);
            }
            res=RES_OK;
        }
    }
    break;
    case GET_BLOCK_SIZE:
    {
        if(CardType&CT_SD2)
        {
            if(SD_Cmd(ACMD13, 0)==0)
            {
                SPI_SoftWrite(0xFF);
                if(receivedatablock(csd, 16))
                {
                    for(n=64-16;n;n--)  SPI_SoftWrite(0xff);
                    *(DWORD*)buff =16UL <<(csd[10]>>4);
                    res=RES_OK;
                }
            }
        }
        else
        {
            if((SD_Cmd(CMD9,0)==0) && receivedatablock(csd,0x10))
            {
                if(CardType &CT_SD1)
                {
                    *(DWORD*)buff =(((csd[10] & 63)<<1)+((WORD)(csd[11]&128)>>7)+1)<<((csd[13]>>6)-1);
                }
                else
                {
                    *(DWORD*)buff=((WORD)((csd[10] &124)>>2)+1)*(((csd[11]&3)<<3)+((csd[11]&224)>>5)+1);
                }
                res=RES_OK;
            }
        }
    }
    break;
    case CTRL_ERASE_SECTOR:
    {
        if(!(CardType&CT_SDC))  break;
        if(disk_ioctl(drv, MMC_GET_CSD, csd))   break;
        if(!(csd[0]>>6)&& !(csd[10]&0x40))      break;
        dp=buff;st=dp[0];ed=dp[1];
        if(!(CardType & CT_BLOCK))
        {
            st*=512;ed*=512;
        }
        if(SD_Cmd(CMD32,st)==0 &&SD_Cmd(CMD33,ed) ==0 && SD_Cmd(CMD38,0)==0 && wait_ready(30))
        {
            res=RES_OK;
        }
    }
    break;
    default:
        res = RES_PARERR;
    }
    CSDeSelect;
    return res;

}

void SDTimer_Handler(void)
{
    WORD n;
    n = Timer1;                     /* 1kHz decrement timer stopped at 0 */
        if (n) Timer1 = --n;
    n = Timer2;
        if (n) Timer2 = --n;

}


int i16_sd_open_file(char *txtname, bool RnW)
{
	FRESULT iFResult;
	static char g_pcTmpBuf[PATH_BUF_SIZE];



	memset(g_pcTmpBuf, 0, sizeof(g_pcTmpBuf));

	//
	// Now finally, append the file name to result in a fully specified file.
	//
	strcat(g_pcTmpBuf, txtname);

	if (RnW)
	{
		//
		// Open the file for reading.
		//
		iFResult = f_open(&g_sFileObject, g_pcTmpBuf, FA_READ);
	}
	else
	{
		iFResult = f_open(&g_sFileObject, g_pcTmpBuf, FA_CREATE_NEW);
		if (iFResult == FR_EXIST)
		{
			iFResult = f_open(&g_sFileObject, g_pcTmpBuf, FA_WRITE);
		}
		else
		{
			iFResult = f_open(&g_sFileObject, g_pcTmpBuf, FA_WRITE);
		}
	}
	//
	// If there was some problem opening the file, then return an error.
	//
	if(iFResult != FR_OK)
	{
		return((int)iFResult);
	}

	return (0);
}

FRESULT SD_cardInit(void)
{
	FRESULT iFResult;
	DSTATUS iStatus;
	iStatus=disk_initialize(0);
	iFResult =f_mount(0, &g_sFatFs);
	if((!iFResult==FR_OK))
	{
		return iFResult;
	}
	f_opendir(&g_sDirObject, "./");
	return iStatus;
	
}

int i16_sd_write_bytes(char *data, uint32_t NumofBytes)
{
	volatile FRESULT iFResult;
	volatile uint16_t ui16BytesWrite = 0;


	iFResult = f_lseek(&g_sFileObject, g_sFileObject.fsize);
	if(iFResult != FR_OK)
	{
		return(iFResult);
	}
	//
	// Enter a loop to repeatedly read data from the file and display it, until
	// the end of the file is reached.
	//
	iFResult = f_write(&g_sFileObject, (void *)data, NumofBytes,
			(UINT *)&ui16BytesWrite);


	
	//
	// Return success.
	//
	return(iFResult);
}

int i16_sd_close_file(void)
{
	FRESULT iFResult;



	iFResult = f_close(&g_sFileObject);

	//
	// If there was some problem opening the file, then return an error.
	//
	if(iFResult != FR_OK)
	{
		
		return((int)iFResult);
	}


	return (0);
}

int i16_sd_read_bytes(uint32_t u32_offset, uint8_t *readData, uint32_t NumofBytes)
{
	FRESULT iFResult;
	uint16_t ui16BytesRead;

	if (u32_offset >= g_sFileObject.fsize)
		return FR_INVALID_PARAMETER;
	


	iFResult = f_lseek(&g_sFileObject, u32_offset);
	if(iFResult != FR_OK)
	{

		return(iFResult);
	}

	iFResult = f_read(&g_sFileObject, readData, NumofBytes, (UINT *)&ui16BytesRead);
	//
	// If there was an error reading, then print a newline and return the
	// error to the user.
	//
	if(iFResult != FR_OK)
	{

		return((int)iFResult);
	}


	//
	// Return success.
	//
	return(FR_OK);
}