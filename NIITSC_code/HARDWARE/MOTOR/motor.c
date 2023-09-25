#include "motor.h"
#include "stdlib.h"

void Control_Motor_Init(void);          //初始化控制电机所需的IO
void TIM_PWM_Init(u32 arr, u32 psc);

void Motor_Init(void)
{
	Control_Motor_Init();
	TIM_PWM_Init(101-1, 8400-1);
}

/**************************************************************************
	电机控制初始化函数
**************************************************************************/
void Control_Motor_Init(void)          //初始化控制电机所需的IO
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;         //推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
/*-------------------------AIN_GPIO_CONFIG--------------------------*/
	RCC_AHB1PeriphClockCmd(AIN_GPIO_CLK, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin = AIN1_GPIO_PIN | AIN2_GPIO_PIN;
	GPIO_Init(AIN_GPIO_PORT, &GPIO_InitStruct);
	
/*-------------------------BIN_GPIO_CONFIG--------------------------*/
	RCC_AHB1PeriphClockCmd(BIN_GPIO_CLK, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin = BIN1_GPIO_PIN | BIN2_GPIO_PIN;
	GPIO_Init(BIN_GPIO_PORT, &GPIO_InitStruct);
	
/*-------------------------CIN_GPIO_CONFIG--------------------------*/
	RCC_AHB1PeriphClockCmd(CIN_GPIO_CLK, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin = CIN1_GPIO_PIN | CIN2_GPIO_PIN;
	GPIO_Init(CIN_GPIO_PORT, &GPIO_InitStruct);
	
/*-------------------------DIN_GPIO_CONFIG--------------------------*/
	RCC_AHB1PeriphClockCmd(DIN_GPIO_CLK, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin = DIN1_GPIO_PIN | DIN2_GPIO_PIN;
	GPIO_Init(DIN_GPIO_PORT, &GPIO_InitStruct);
}


/**************************************************************************
	电机PWM初始化
**************************************************************************/
void TIM_PWM_Init(u32 arr , u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(MOTORA_PWM_TIM_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(MOTORB_PWM_TIM_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(MOTORC_PWM_TIM_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(MOTORD_PWM_TIM_CLK, ENABLE);
	
	RCC_AHB1PeriphClockCmd(MOTORA_PWM_PIN_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTORB_PWM_PIN_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTORC_PWM_PIN_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTORD_PWM_PIN_CLK, ENABLE);
	
	GPIO_PinAFConfig(MOTORA_PWM_PORT, MOTORA_PWM_SOURCE, MOTORA_PWM_AF);
	GPIO_PinAFConfig(MOTORB_PWM_PORT, MOTORB_PWM_SOURCE, MOTORB_PWM_AF);
	GPIO_PinAFConfig(MOTORC_PWM_PORT, MOTORC_PWM_SOURCE, MOTORC_PWM_AF);
	GPIO_PinAFConfig(MOTORD_PWM_PORT, MOTORD_PWM_SOURCE, MOTORD_PWM_AF);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_InitStruct.GPIO_Pin = MOTORA_PWM_PIN;
	GPIO_Init(MOTORA_PWM_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = MOTORB_PWM_PIN;
	GPIO_Init(MOTORB_PWM_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = MOTORC_PWM_PIN;
	GPIO_Init(MOTORC_PWM_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = MOTORD_PWM_PIN;
	GPIO_Init(MOTORD_PWM_PORT, &GPIO_InitStruct);
	
	TIM_DeInit(MOTORA_PWM_TIM);
	TIM_DeInit(MOTORB_PWM_TIM);
	TIM_DeInit(MOTORC_PWM_TIM);
	TIM_DeInit(MOTORD_PWM_TIM);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //输入捕获的分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复次数(高级定时器)
	TIM_TimeBaseInit(MOTORA_PWM_TIM, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(MOTORB_PWM_TIM, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(MOTORC_PWM_TIM, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(MOTORD_PWM_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	MOTORA_PWM_OC_INIT(&TIM_OCInitStructure); 
	MOTORB_PWM_OC_INIT(&TIM_OCInitStructure); 
	MOTORC_PWM_OC_INIT(&TIM_OCInitStructure); 
	MOTORD_PWM_OC_INIT(&TIM_OCInitStructure);
	
	MOTORA_PWM_OC_PRELOAD_CFIG();
	MOTORB_PWM_OC_PRELOAD_CFIG();
	MOTORC_PWM_OC_PRELOAD_CFIG();
	MOTORD_PWM_OC_PRELOAD_CFIG();
	
	TIM_ARRPreloadConfig(MOTORA_PWM_TIM, ENABLE);
	TIM_ARRPreloadConfig(MOTORB_PWM_TIM, ENABLE);
	TIM_ARRPreloadConfig(MOTORC_PWM_TIM, ENABLE);
	TIM_ARRPreloadConfig(MOTORD_PWM_TIM, ENABLE);

	TIM_Cmd(MOTORA_PWM_TIM, ENABLE);
	TIM_Cmd(MOTORB_PWM_TIM, ENABLE);
	TIM_Cmd(MOTORC_PWM_TIM, ENABLE);
	TIM_Cmd(MOTORD_PWM_TIM, ENABLE);

	RCC_AHB1PeriphClockCmd(MOTOT_12_ENA_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTOT_34_ENA_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStruct.GPIO_Pin = MOTOT_12_ENA_PIN;
	GPIO_Init(MOTOT_12_ENA_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = MOTOT_34_ENA_PIN;
	GPIO_Init(MOTOT_34_ENA_PORT, &GPIO_InitStruct);
	MOTOT_12_ENA = ENABLE;
	MOTOT_34_ENA = ENABLE;
}

void Set_Speed_All(int8_t speedA,int8_t speedB,int8_t speedC,int8_t speedD)
{
	MotorA_SetSpeed(speedA);
	MotorB_SetSpeed(speedB);	
	MotorC_SetSpeed(speedC);
	MotorD_SetSpeed(speedD);
}

void MotorA_SetSpeed(int8_t speed)
{
	if(speed>0)	    DIN2=1,			DIN1=0; //前进 
	else           	DIN2=0,			DIN1=1; //后退
	PWMA=abs((int)(speed*0.80));
}

void MotorB_SetSpeed(int8_t speed)
{
	if(speed>0)	    CIN2=1,			CIN1=0; //前进 
	else           	CIN2=0,			CIN1=1; //后退
	PWMB=abs(speed);
}

void MotorC_SetSpeed(int8_t speed)
{
	if(speed>0)	    AIN2=1,			AIN1=0; //前进 
	else           	AIN2=0,			AIN1=1; //后退
	PWMC=abs((int)(speed*0.80));
}

void MotorD_SetSpeed(int8_t speed)
{
	if(speed>0)	    BIN2=1,			BIN1=0; //前进 
	else           	BIN2=0,			BIN1=1; //后退
	PWMD=abs((int)(speed*0.80));
}
