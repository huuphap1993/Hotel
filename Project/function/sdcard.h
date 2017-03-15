/*
 * sdcard.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 #ifndef SDCARD_H
 #define SDCARD_H
 #include "stm32f10x_rcc.h"
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_spi.h"
 #include "stdbool.h"
 #include "ff.h"
 
 #define FF_SELECT_NONE		0
#define FF_SELECT_SD_CARD	1
#define FF_SELECT_USB			2


/* Definitions for MMC/SDC command */
#define CMD0    (0)         /* GO_IDLE_STATE */
#define CMD1    (1)         /* SEND_OP_COND (MMC) */
#define ACMD41  (41)   /* SEND_OP_COND (SDC) */
#define CMD8    (8)         /* SEND_IF_COND */
#define CMD9    (9)         /* SEND_CSD */
#define CMD10   (10)        /* SEND_CID */
#define CMD12   (12)        /* STOP_TRANSMISSION */
#define ACMD13  (0x80+13)   /* SD_STATUS (SDC) */
#define CMD16   (16)        /* SET_BLOCKLEN */
#define CMD17   (17)        /* READ_SINGLE_BLOCK */
#define CMD18   (18)        /* READ_MULTIPLE_BLOCK */
#define CMD23   (23)        /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80+23)   /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (24)        /* WRITE_BLOCK */
#define CMD25   (25)        /* WRITE_MULTIPLE_BLOCK */
#define CMD32   (32)        /* ERASE_ER_BLK_START */
#define CMD33   (33)        /* ERASE_ER_BLK_END */
#define CMD38   (38)        /* ERASE */
#define CMD55   (55)        /* APP_CMD */
#define CMD58   (58)        /* READ_OCR */

void SDTimer_Handler(void);
int i16_sd_open_file(char *txtname, bool RnW);
int i16_sd_write_bytes(char *data, uint32_t NumofBytes);
int i16_sd_close_file(void);
FRESULT SD_cardInit(void);


 
 #endif
 
 
 