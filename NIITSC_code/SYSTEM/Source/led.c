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

#include "led.h"  

// 函数：LED IO口初始化
//

int LED_State = 0;
void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	//初始化GPIOG时钟	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; //速度选择
	
	//初始化 LED1 引脚
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_SetBits(LED1_PORT,LED1_PIN);  
}

void LED_Flip(void)
{
	if(LED_State>5)
	{
		LED_State++;
		LED1_OFF;
	}
	else{
		LED_State++;
		LED1_ON;
	}
	if(LED_State>=10)
		LED_State = 0;
}

