/***
	***************************************************************************
	*	@file  	led.c
	*	@version V1.0.0
	*	@brief   LED接口相关函数
   ***************************************************************************
   *  @description
	*
	*  初始化LED的IO口，配置为推挽输出、上拉、速度等级2M。
	* 	
	***************************************************************************
***/

#include "infrared.h"  
#include "delay.h"  


void Laser_Init(void)
{
	INFRARED1_Init();
	INFRARED2_Init();
	INFRARED3_Init();
	INFRARED4_Init();
}
void INFRARED1_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED1_CLK, ENABLE); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	

	GPIO_InitStructure.GPIO_Pin = INFRARED1_PIN;	 
	GPIO_Init(INFRARED1_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(INFRARED1_PORT,INFRARED1_PIN); 
}

void INFRARED2_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED2_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
	
	GPIO_InitStructure.GPIO_Pin = INFRARED2_PIN;	 
	GPIO_Init(INFRARED2_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(INFRARED2_PORT,INFRARED2_PIN);  
}
void INFRARED3_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED3_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
	
	GPIO_InitStructure.GPIO_Pin = INFRARED3_PIN;	 
	GPIO_Init(INFRARED3_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(INFRARED3_PORT,INFRARED3_PIN);  
}
void INFRARED4_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED4_CLK, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = INFRARED4_PIN;	 
	GPIO_Init(INFRARED4_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(INFRARED4_PORT,INFRARED4_PIN);  
}

u8	Infrared_Scan(GPIO_TypeDef* INFRARED_PORT, uint16_t INFRARED_PIN)
{
	if( GPIO_ReadInputDataBit ( INFRARED_PORT,INFRARED_PIN) == 1 )	//检测按键是否被按下
	{	
		return INFRARED_ON;	
	}
	return INFRARED_OFF;	
}
