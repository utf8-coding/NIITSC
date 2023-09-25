#include "encoder.h"

u8 encoder_Count_Buff[encoder_Cnt_Num] = {0};
/*===================ENCODER Initialize====================*/

void Encoder_Init_All(void){
	Encoder1_Init();
	Encoder2_Init();
	Encoder3_Init();
	Encoder4_Init();
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
	
	TIM_EncoderInterfaceConfig(ENCODER4_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER4_TIM, ENABLE);
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
//int Read_Velocity( TIMX)
//{
//    int Encoder_TIM;    
//   switch(TIMX)
//	 {
//		case 2:  Encoder_TIM= ENCODER1_CNT; 	Encoder_TIM=0; break;
//		case 5:  Encoder_TIM= ENCODER2_CNT; 	Encoder_TIM=0; break;	
//		case 1:  Encoder_TIM= ENCODER3_CNT;  	Encoder_TIM=0; break;
//		case 4:  Encoder_TIM= ENCODER4_CNT;  	Encoder_TIM=0; break;		 
//		default: Encoder_TIM=0;
//	 }
//		return Encoder_TIM;
//}

int Read_Velocity(TIM_TypeDef* TIMX)
{
	if(TIMX == ENCODER1_TIM){
		return  ENCODER1_CNT;		
	}
	else if(TIMX == ENCODER2_TIM){
		return  ENCODER2_CNT;	
	}
	else if(TIMX == ENCODER3_TIM){
		return  ENCODER3_CNT;	
	}
	else if(TIMX == ENCODER4_TIM){
		return  ENCODER4_CNT;	
	}
	else{
		return 0;
	}
}
///**************************************************************************
//函数功能：读取位置信息
//入口参数：定时器
//返回  值：位置值
//**************************************************************************/
//int Read_Position(u8 TIMX)
//{
//    int Encoder_TIM;    
//   switch(TIMX)
//	 {
//		case 2:  Encoder_TIM= ENCODER1_CNT;  break;
//		case 3:  Encoder_TIM= ENCODER2_CNT;  break;	
//		case 4:  Encoder_TIM= ENCODER3_CNT;  break;	
//		case 5:  Encoder_TIM= ENCODER4_CNT;  break;	
//		default: Encoder_TIM=0;
//	 }
//		return Encoder_TIM;
//}



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
		encoder_Count_Buff[encoder_Cnt_1] = Read_Velocity(ENCODER1_TIM);
		encoder_Count_Buff[encoder_Cnt_2] = Read_Velocity(ENCODER2_TIM);
		encoder_Count_Buff[encoder_Cnt_3] = Read_Velocity(ENCODER3_TIM);
		encoder_Count_Buff[encoder_Cnt_4] = Read_Velocity(ENCODER4_TIM);
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//void TIM2_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
//{ 		    		  			    
// if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==SET)//溢出中断
// {
//	 
// } 
// TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除中断标志位 	  
//}

//void TIM1_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
//{ 		    		  			    
// if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==SET)//溢出中断
// {
//	 
// } 
// TIM_ClearITPendingBit(TIM1,TIM_IT_Update); //清除中断标志位 	
//}

//void TIM4_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
//{ 		    		  			    
// if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)//溢出中断
// {
//	 
// } 
// TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清除中断标志位 	  
//}

//void TIM5_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
//{ 		    		  			    
// if(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==SET)//溢出中断
// {
//	 
// } 
// TIM_ClearITPendingBit(TIM5,TIM_IT_Update); //清除中断标志位 	  
//}
