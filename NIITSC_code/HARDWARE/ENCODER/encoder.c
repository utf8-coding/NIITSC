#include "encoder.h"
#include "oled.h"

//NOTE: Max enc speed is around 500

void Encoder1_Init(void);
void Encoder2_Init(void);
void Encoder3_Init(void);
void Encoder4_Init(void);
void Encoder_Tim_Config(u16 arr, u16 psc);


int encoder_Count_Buff[encoder_Cnt_Num] = {0};
/*===================ENCODER Initialize====================*/

void Encoder_Init_All(void){
	Encoder1_Init();
	Encoder2_Init();
	Encoder3_Init();
	Encoder4_Init();
	Encoder_Tim_Config(1000, 8400); // 84MHz/8400/1000 = 10Hz
}

void Encoder1_Init(void)
{
	RCC_APB1PeriphClockCmd(ENCODER1_TIM_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER1_PIN_CLK_1 | ENCODER1_PIN_CLK_2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = ENCODER1_PIN_1 | ENCODER1_PIN_2;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        
//	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; 
	GPIO_Init(ENCODER1_PORT_1, &GPIO_InitStructure);
	GPIO_Init(ENCODER1_PORT_2, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(ENCODER1_PORT_1, ENCODER1_SOURCE_1, ENCODER1_AF);    
	GPIO_PinAFConfig(ENCODER1_PORT_2, ENCODER1_SOURCE_2, ENCODER1_AF);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENCODER1_TIM, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);   //每一项按照缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x10;
	TIM_ICInit(ENCODER1_TIM, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER1_TIM, ENABLE);
}

void Encoder2_Init(void)
{
	RCC_APB1PeriphClockCmd(ENCODER2_TIM_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER2_PIN_CLK_1 | ENCODER2_PIN_CLK_2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = ENCODER2_PIN_1 | ENCODER2_PIN_2;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        
//	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; 
	GPIO_Init(ENCODER2_PORT_1, &GPIO_InitStructure);
	GPIO_Init(ENCODER2_PORT_2, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(ENCODER2_PORT_1, ENCODER2_SOURCE_1, ENCODER2_AF);    
	GPIO_PinAFConfig(ENCODER2_PORT_2, ENCODER2_SOURCE_2, ENCODER2_AF);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENCODER2_TIM, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);   //每一项按照缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x10;
	TIM_ICInit(ENCODER2_TIM, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER2_TIM, ENABLE);
}

void Encoder3_Init(void)
{
	RCC_APB2PeriphClockCmd(ENCODER3_TIM_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER3_PIN_CLK_1 | ENCODER3_PIN_CLK_2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = ENCODER3_PIN_1 | ENCODER3_PIN_2;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        
//	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; 
	GPIO_Init(ENCODER3_PORT_1, &GPIO_InitStructure);
	GPIO_Init(ENCODER3_PORT_2, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(ENCODER3_PORT_1, ENCODER3_SOURCE_1, ENCODER3_AF);    
	GPIO_PinAFConfig(ENCODER3_PORT_2, ENCODER3_SOURCE_2, ENCODER3_AF);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENCODER3_TIM, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);   //每一项按照缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x10;
	TIM_ICInit(ENCODER3_TIM, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER3_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER3_TIM, ENABLE);
}

void Encoder4_Init(void)
{
	RCC_APB1PeriphClockCmd(ENCODER4_TIM_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER4_PIN_CLK_1 | ENCODER4_PIN_CLK_2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = ENCODER4_PIN_1 | ENCODER4_PIN_2;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        
//	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; 
	GPIO_Init(ENCODER4_PORT_1, &GPIO_InitStructure);
	GPIO_Init(ENCODER4_PORT_2, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(ENCODER4_PORT_1, ENCODER4_SOURCE_1, ENCODER4_AF);    
	GPIO_PinAFConfig(ENCODER4_PORT_2, ENCODER4_SOURCE_2, ENCODER4_AF);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENCODER4_TIM, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);   //每一项按照缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x10;
	TIM_ICInit(ENCODER4_TIM, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER4_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER4_TIM, ENABLE);
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int read_Velocity(TIM_TypeDef* TIMX)
{
	int Encoder_TIM; 
	if(TIMX == ENCODER1_TIM){
		Encoder_TIM = ENCODER1_CNT;
		TIM5->CNT = 0;
		return Encoder_TIM;		
	}
	else if(TIMX == ENCODER2_TIM){
		Encoder_TIM = ENCODER2_CNT;
		TIM2->CNT = 0;
		return  Encoder_TIM;	
	}
	else if(TIMX == ENCODER3_TIM){
		Encoder_TIM = ENCODER3_CNT;
		TIM1->CNT = 0;
		return  Encoder_TIM;	
	}
	else if(TIMX == ENCODER4_TIM){
		Encoder_TIM = ENCODER4_CNT;
		TIM4->CNT = 0;
		return  Encoder_TIM;	
	}
	else{
		return 0;
	}
}


/*===================ENCODER+_TIM Initialize====================*/
void Encoder_Tim_Config(u16 arr, u16 psc){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
//	TIM_InternalClockConfig(TIM7);//the internal clock is default so do not need to used this conf

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //输入捕获的分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复次数(高级定时器)
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update); //to prevent enter the interupt immediatelly after config
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM7_IRQHandler(void){
	if(TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == SET){
		encoder_Count_Buff[encoder_Cnt_1] = read_Velocity(ENCODER1_TIM);
		encoder_Count_Buff[encoder_Cnt_2] = read_Velocity(ENCODER2_TIM);
		encoder_Count_Buff[encoder_Cnt_3] = read_Velocity(ENCODER3_TIM);
		encoder_Count_Buff[encoder_Cnt_4] = read_Velocity(ENCODER4_TIM);
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}

void Encoder_Display_Spects(void)
{
	OLED_ShowString(1, 1, "Enc:");
	OLED_ShowSignedNum(2, 1, encoder_Count_Buff[encoder_Cnt_2], 3);
	OLED_ShowSignedNum(2, 6, encoder_Count_Buff[encoder_Cnt_1], 3);
	OLED_ShowSignedNum(3, 1, encoder_Count_Buff[encoder_Cnt_3], 3);
	OLED_ShowSignedNum(3, 6, encoder_Count_Buff[encoder_Cnt_4], 3);
}