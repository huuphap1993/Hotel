/*
 * project.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 #ifndef PROJECT_H
 #define PROJECT_H
 typedef enum
 {
	 input_flag,
	 send_data,
	 alarmtimeout
 }Flag;
 
 // Noi chua loi (Error Flag)
	typedef enum 
	{
		SD_NotExist,			//Khong co SD card 
		SIM_NotExist,		// Khong co SIM
		SIM_ErrorCommand,				//SIM bi loi
		SIM_AlarmMoney,			//SIM het tien
		POWER_OFF,						//Cup dien
		TIME_ERROR,				//Loi thoi gian he thong
	}ErrorFlagEnum;
	
	
 #endif
 