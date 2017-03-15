/*
 * 74165.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Phap.Ha
 */
 
 
 #include "74165.h"
  
	
	
	void GPIO74165Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStruture;
		
		RCC_APB2PeriphClockCmd(CLK165Per|LOAD165Per|DATA165Per|RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStruture.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStruture.GPIO_Pin = CLK165Pin | LOAD165Pin;
		GPIO_Init(CLK165Port,&GPIO_InitStruture);
		
		GPIO_InitStruture.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
		GPIO_Init(GPIOC,&GPIO_InitStruture);
		
		GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		GPIO_InitStruture.GPIO_Pin = DATA165Pin;
		GPIO_Init(DATA165Port,&GPIO_InitStruture);
		LOAD165ON;
	}
	
	void ReadInput(uint8_t *u8_out,uint8_t u8_length)
	{
		uint8_t i1,i2;
		LOAD165DATAIN;
		for(i1=0;i1<u8_length;i1++)
		{
			for(i2=0;i2<8;i2++)
			{
				CLK165ON;
				if(DATA165IN)
				{
					u8_out[i1]|=1<<i2;
				}
				else
				{
					u8_out[i1]&=~(1<<i2);
				}
				
				CLK165OFF;
			}
		}
	}

	void Delay74165(uint32_t u32_timeout)
	{
		while(u32_timeout--);
	}
	
	void comparedata(uint8_t *u8_n,uint8_t *u8_p,uint8_t *u8_compare)
	{
		uint8_t i1,i2;
		for(i1=0;i1<5;i1++)
		{
			u8_compare[i1]=0;
			for(i2=0;i2<8;i2++)
			{
				if(((u8_n[i1]>>i2)&0x01) == ((u8_p[i1]>>i2)&0x01))
				{
					u8_compare[i1]&=~(1<<i2);
				}
				else
				{
					u8_compare[i1]|=1<<i2;
				}
			}
		}
	}
	