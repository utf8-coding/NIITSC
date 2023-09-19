#include "stm32f4xx.h"                  // Device header
 #include "buzzer.h"
 #include "delay.h"

void BUZZER_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOC,GPIO_Pin_3);  
}

void BUZZER_Sec(float time_sec)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
  delay_ms((int)(time_sec*1000));
	GPIO_SetBits(GPIOC,GPIO_Pin_3);	
}

void BUZZER_Ms()
{
	BUZZER_Sec(0.05);
}
