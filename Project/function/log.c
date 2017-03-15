/*
 * log.c
 *
 *  Created on: Mar 10, 2017
 *      Author: Phap.Ha
 */
 
 #include "log.h"
 
 extern RTC_time TimeNow;
 char link[8];
 char numconver[10]={'0','1','2','3','4','5','6','7','8','9'};
 
 void GetDIR(void)
 {
	 SD_cardInit();

	
	 i16_sd_open_file("log.txt",0);
 }
 
 uint16_t writelog(uint8_t u8_port, bool port_status)
 {
	 FRESULT iFResult;
	 GetDIR();
	 link[0]=numconver[TimeNow.u8_day/10];
	 link[1]=numconver[TimeNow.u8_day%10];
	 link[2]='/';
	 link[3]=numconver[TimeNow.u8_month/10];
	 link[4]=numconver[TimeNow.u8_month%10];
	 link[5]='/';
	 link[6]=numconver[TimeNow.u8_year/10];
	 link[7]=numconver[TimeNow.u8_year%10];
	 i16_sd_write_bytes(link, 8);
	 link[0]=' ';
	 link[1]=numconver[TimeNow.u8_hour/10];
	 link[2]=numconver[TimeNow.u8_hour%10];
	 link[3]=':';
	 link[4]=numconver[TimeNow.u8_minute/10];
	 link[5]=numconver[TimeNow.u8_minute%10];
	 link[6]=':';
	 i16_sd_write_bytes(link, 6);
	 link[0]='P';
	 link[1]='O';
	 link[2]='R';
	 link[3]='T';
	 link[4]=' ';
	 link[5]=numconver[u8_port/10];
	 link[6]=numconver[u8_port%10];
	 link[7]=' ';
	 iFResult=i16_sd_write_bytes(link, 8);
	 if(port_status)
	 {
		 link[0]='O';
		 link[1]='N';
		 link[2]=0x0D;
		 link[3]=0x0A;
		 i16_sd_write_bytes(link, 4);
	 }
	 else
	 {
		 link[0]='O';
		 link[1]='F';
		 link[2]='F';
		 link[3]=0x0D;
		 link[4]=0x0A;
		 i16_sd_write_bytes(link, 5);
	 }
	 i16_sd_close_file();
	 return iFResult;
 }
 
 void u8_savedata(uint8_t u8_number,bool b_status,uint16_t u16_timenow,uint32_t *outdata)
 {
	 if(b_status)
	 {
		 outdata[u8_number]=0x80000000 ;
		 u16_timenow&=~0x8000;
		 outdata[u8_number]|=u16_timenow<<14;
	 }
	 else
	 {
		 outdata[u8_number]|=0x40000000 ;
		 u16_timenow&=~0x8000;
		 outdata[u8_number]|=u16_timenow;
	 }
 }
 