/*
 * log.h
 *
 *  Created on: Mar 10, 2017
 *      Author: Phap.Ha
 */
 #ifndef LOG_H
 #define LOG_H
 #include "sdcard.h"
 #include "sim800.h"
 
 uint16_t writelog(uint8_t u8_port, bool port_status);
 void u8_savedata(uint8_t u8_number,bool b_status,uint16_t u16_timenow,uint32_t *outdata);

 #endif
 