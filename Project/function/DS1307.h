/*
 * DS1307.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Phap.Ha
 */
 
 
 #ifndef DS1307_H
 #define DS1307_H
 
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_i2c.h"
 
 #include "stdbool.h" 

	 #define I2C_DS1307		I2C1
	 #define DS1307_SCL		GPIO_Pin_6
	 #define DS1307_SDA		GPIO_Pin_7
	 #define RCC_APB2Periph_DS1307_GPIO		RCC_APB2Periph_GPIOB
	 #define RCC_APB1Periph_DS1307_I2C		RCC_APB1Periph_I2C1
	 
		/* Dinh nghia cac thanh ghi cua ds1307 */
	 #define DS1307_ADDRESS_READ	0xD1
	 #define DS1307_ADDRESS_WRITE	0xD0
	 #define DS1307_SECOND_REG 		0x00
	 #define DS1307_MINUTE_REG 		0x01
	 #define DS1307_HOURS_REG 		0x02
	 #define DS1307_DAY_REG				0x03
	 #define DS1307_DATE_REG			0x04
	 #define DS1307_MONTH_REG			0x05
	 #define DS1307_YEAR_REG			0x06
	 #define DS1307_CONTROL_REG		0x07
	  void i2c_init();
		
	 uint8_t DS1307_ReadReg(uint8_t addReg);
	 uint8_t BCD2DEC(uint8_t dat);
	 uint8_t DEC2BCD(uint8_t dat);
	 void I2Cx_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 WriteAddr);
	 void I2Cx_BufferRead(u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);


#endif
