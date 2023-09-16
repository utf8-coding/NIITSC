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
	RCC_AHB1PeriphClockCmd (INFRARED1_CLK, ENABLE); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	

	GPIO_InitStructure.GPIO_Pin = INFRARED1_PIN;	 
	GPIO_Init(INFRARED1_PORT, &GPIO_InitStructure);	
}

void INFRARED2_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED2_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
	
	GPIO_InitStructure.GPIO_Pin = INFRARED2_PIN;	 
	GPIO_Init(INFRARED2_PORT, &GPIO_InitStructure);	
}
void INFRARED3_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED3_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
	
	GPIO_InitStructure.GPIO_Pin = INFRARED3_PIN;	 
	GPIO_Init(INFRARED3_PORT, &GPIO_InitStructure);	
}
void INFRARED4_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( INFRARED4_CLK, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = INFRARED4_PIN;	 
	GPIO_Init(INFRARED4_PORT, &GPIO_InitStructure);	 
}

u8 Infrared_Scan(void)
{
	u8 ourput = GPIO_ReadInputDataBit(INFRARED1_PORT,INFRARED1_PIN) | GPIO_ReadInputDataBit(INFRARED2_PORT,INFRARED2_PIN) <<1 | GPIO_ReadInputDataBit(INFRARED3_PORT,INFRARED3_PIN) <<2 | GPIO_ReadInputDataBit(INFRARED4_PORT,INFRARED4_PIN) <<3;
	return ourput;
}
