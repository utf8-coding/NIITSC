/***
	***************************************************************************
	*	@file  	led.c
	*	@version V1.0.0
	*	@brief   LED�ӿ���غ���
   ***************************************************************************
   *  @description
	*
	*  ��ʼ��LED��IO�ڣ�����Ϊ����������������ٶȵȼ�2M��
	* 	
	***************************************************************************
***/

#include "led.h"  

// ������LED IO�ڳ�ʼ��
//

int LED_State = 0;
void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	//��ʼ��GPIOGʱ��	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; //�ٶ�ѡ��
	
	//��ʼ�� LED1 ����
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_SetBits(LED1_PORT,LED1_PIN);  
}

void LED_Flip(void)
{
	if(LED_State>150)
	{
		LED_State++;
		LED1_OFF;
	}
	else{
		LED_State++;
		LED1_ON;
	}
	if(LED_State>=300)
		LED_State = 0;
}

