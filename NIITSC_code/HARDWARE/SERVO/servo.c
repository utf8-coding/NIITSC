#include "stm32f4xx.h"
#include "servo.h"  
#include "string.h"
#include "bool.h"
#include "LobotServoController.h"
#include "run.h"
#include "delay.h"
//plate 1  screw 2  hand 3
u8 UART_RX_BUF[16];
bool isUartRxCompleted = false;

int fputc(int ch, FILE *f)
{
        if(ch =='\n')  
        {  
            USART_SendData(SERVO_USART, 0x0D);  
            while (!(SERVO_USART->SR & USART_FLAG_TXE)); 
            USART_SendData(SERVO_USART, 0x0A);  
            while (!(SERVO_USART->SR & USART_FLAG_TXE));  
        }  
        else  
        {  
            USART_SendData(SERVO_USART, (unsigned char)ch);  
            while (!(SERVO_USART->SR & USART_FLAG_TXE));   
        }  
	return ch;
}

void SERVO_USART_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(SERVO_USART_TX_CLK
                        | SERVO_USART_RX_CLK, ENABLE); //GPIO_CLK enable
    RCC_APB1PeriphClockCmd(SERVO_USART_CLK, ENABLE);   //USART_CLK enable
    
    GPIO_InitStructure.GPIO_Pin = SERVO_USART_TX_PIN 
                                | SERVO_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SERVO_USART_TX_PORT, &GPIO_InitStructure);       
    GPIO_Init(SERVO_USART_RX_PORT, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(SERVO_USART_TX_PORT, SERVO_USART_TX_PinSource, GPIO_AF_SERVO_USART);
    GPIO_PinAFConfig(SERVO_USART_RX_PORT, SERVO_USART_RX_PinSource, GPIO_AF_SERVO_USART);

    USART_InitStructure.USART_BaudRate = SERVO_USART_BaudRate;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(SERVO_USART,&USART_InitStructure); 
    USART_Cmd(SERVO_USART,ENABLE);
    
    USART_ITConfig(SERVO_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = SERVO_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}

void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) {
		while ((SERVO_USART->SR & 0x40) == 0);
		USART_SendData(SERVO_USART,*buf++);
	}
}

extern uint8_t LobotRxBuf[16];

void USART3_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	if (USART_GetITStatus(SERVO_USART, USART_IT_RXNE) != RESET) { //??????
		Res = USART_ReceiveData(SERVO_USART);//(SERVO_USART->DR);	//?????????
		if (!isGotFrameHeader) {  //????
			if (Res == FRAME_HEADER) {
				frameHeaderCount++;
				if (frameHeaderCount == 2) {
					frameHeaderCount = 0;
					isGotFrameHeader = true;
					dataCount = 1;
				}
			} else {
				isGotFrameHeader = false;
				dataCount = 0;
				frameHeaderCount = 0;
			}
		}
		if (isGotFrameHeader) { //????????
			UART_RX_BUF[dataCount] = Res;
			if (dataCount == 2) {
				dataLength = UART_RX_BUF[dataCount];
				if (dataLength < 2 || dataLength > 8) {
					dataLength = 2;
					isGotFrameHeader = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 2) {
				if (isUartRxCompleted == false) {
					isUartRxCompleted = true;
					memcpy(LobotRxBuf, UART_RX_BUF, dataCount);
				}
				isGotFrameHeader = false;
			}
		}
	}
}

void servoDefault(void){
//	moto_Action(up, 9, 500);
	runActionGroup(defa, 1);  //张开 向内
}

void servoMvCalib(void){
	runActionGroup(screwFront, 1); 
	delay_ms(200);
}


void plateRotate(u8 color_Index, u16 nms){
	switch(color_Index){
		case 0:
		case 4:
			runActionGroup(plateRotate1, 1);
			delay_ms(nms);
			break;
		case 1:
		case 5:
			runActionGroup(plateRotate2, 1);
			delay_ms(nms);
			break;
		case 2:
		case 6:
			runActionGroup(plateRotate3, 1);
			delay_ms(nms);
			break;
	}
}
/*=============================obj============================*/
void get_Obj(void){
	
//	runActionGroup(screwFront, 1);		//2700
	runActionGroup(handRelease, 1);		//2700
	moto_Action(down, 9, 4000);   
	
	runActionGroup(handGet, 1);
	delay_ms(700);	
	
	runActionGroup(screwBack, 1);		//2700
	delay_ms(100);
	plateRotate(color_Index, 10);
	moto_Action(up, 9, 4050);     
	
	//放盘
	moto_Action(down, 9, 1200);
	runActionGroup(handRelease ,1);
	delay_ms(300);
	moto_Action(up, 9, 1200);
	
	runActionGroup(screwFront, 1);		//2700
}

/*===============================rough=============================*/
void put_Rough(void){
	runActionGroup(handRelease, 1);	
	delay_ms(10);
	//回转
	runActionGroup(screwBack, 1);		//2700
	delay_ms(2700);

	//收盘
	plateRotate(color_Index, 10);
	moto_Action(down, 9, 1200);
	runActionGroup(handGet ,1);
	delay_ms(300);
	moto_Action(up, 9, 1200);
	
	runActionGroup(screwFront, 1); //向外
	delay_ms(10);
	moto_Action(down, 9, 8900);
	runActionGroup(handRelease,1);
	delay_ms(300);
	moto_Action(up, 9, 8900);
}

void get_Rough(void){
	//拿地
	runActionGroup(handRelease, 1);		//2700
	moto_Action(down, 9, 8500);
	runActionGroup(handGet, 1);
	delay_ms(300);
	//回转 
	runActionGroup(screwBack, 1);		//2700
	delay_ms(10);
	//转盘
	plateRotate(color_Index, 10);
	//同时上升
	moto_Action(up, 9, 8700);
	
	runActionGroup(handGet, 1);
	delay_ms(300);

	//放盘
	moto_Action(down, 9, 1200);
	runActionGroup(handRelease,1);
	delay_ms(300);
	moto_Action(up, 9, 1200);
	
}

void put_Down_Dep1(void){
	runActionGroup(handRelease, 1);	
	delay_ms(10);
	//回转
	runActionGroup(screwBack, 1);		//2700
	delay_ms(2700);

	//收盘
	plateRotate(color_Index, 10);
	moto_Action(down, 9, 1200);
	runActionGroup(handGet ,1);
	delay_ms(300);
	moto_Action(up, 9, 1200);
	
	runActionGroup(screwFront, 1); //向外
	delay_ms(10);
	moto_Action(down, 9, 8900);
	runActionGroup(handRelease,1);
	delay_ms(300);
	moto_Action(up, 9, 8900);
}

void put_Down_Dep2(void){
	runActionGroup(handRelease, 1);	
	delay_ms(10);
	//回转
	runActionGroup(screwBack, 1);		//2700
	delay_ms(2700);

	//收盘
	plateRotate(color_Index, 10);
	moto_Action(down, 9, 1200);
	runActionGroup(handGet ,1);
	delay_ms(300);
	moto_Action(up, 9, 1200);
	
	runActionGroup(screwFront, 1); //向外
	delay_ms(10);
	moto_Action(down, 9, 8900);
	runActionGroup(handRelease,1);
	delay_ms(300);
	moto_Action(up, 9, 8900);
}
//plate relate to color_Index
void servo_Action(servoAction actionNum)
{
	switch(actionNum){
		case defaut:
			servoDefault();
			break;
		case calib:
			servoMvCalib();
			break;
		case getObj:
			get_Obj();
			color_Index++;
			break;			
		case putRough:
			put_Rough();
			color_Index--;
			break;
		case getRough:
			get_Rough();
			color_Index++;
			break;
		case putDownDep1:
			put_Down_Dep1();
			color_Index--;
			break;
		case putDownDep2:
			put_Down_Dep2();
			color_Index--;
			break;		
	}
}
//			//放盘
//			moto_Action(down, 9, 1200);
//			runActionGroup(4,1);
//			delay_ms(300);
//			moto_Action(up, 9, 1200);
//			//收盘
//			moto_Action(up, 9, 1200);
//			runActionGroup(4,1);
//			delay_ms(300);
//			moto_Action(down, 9, 1200);


//		//放地
//		moto_Action(down, 9, 8900);
//		runActionGroup(4,1);
//		delay_ms(300);
//		moto_Action(up, 9, 8900);
//		//拿地
//		moto_Action(down, 9, 8500);
//		runActionGroup(3,1);
//		delay_ms(300);
//		moto_Action(up, 9, 8700);
/*===================步进电机dir初始化==========================*/
void Moto_Init(u32 arr, u32 psc){
	Moto_Dir_Init();
	MOTO_PWM_Init(arr, psc);
}
void Moto_Dir_Init(void)          //初始化控制电机所需的IO
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;         //推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
/*-------------------------AIN_GPIO_CONFIG--------------------------*/
	RCC_AHB1PeriphClockCmd(MOTO_GPIO_CLK, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin = MOTO_GPIO_PIN;
	GPIO_Init(MOTO_GPIO_PORT, &GPIO_InitStruct);
	
}


/**************************************************************************
	电机PWM初始化
**************************************************************************/
//84000000/arr 840 psc 100        1000
//200 Pulse/round       
void MOTO_PWM_Init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	RCC_APB2PeriphClockCmd(MOTO_PWM_TIM_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTO_PWM_PIN_CLK, ENABLE);
	GPIO_PinAFConfig(MOTO_PWM_PORT, MOTO_PWM_SOURCE, MOTO_PWM_AF);
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_InitStruct.GPIO_Pin = MOTO_PWM_PIN;
	GPIO_Init(MOTO_PWM_PORT, &GPIO_InitStruct);
	
	TIM_DeInit(MOTO_PWM_TIM);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //输入捕获的分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复次数(高级定时器)
	TIM_TimeBaseInit(MOTO_PWM_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(MOTO_PWM_TIM, &TIM_OCInitStructure); 
 
	TIM_OC1PreloadConfig(MOTO_PWM_TIM, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(MOTO_PWM_TIM, ENABLE);

	TIM_Cmd(MOTO_PWM_TIM, ENABLE);

}

//200pulse/round   
void moto_Action(motoDirEnum dir, int8_t speed, u16 nms){
	if(dir == down)  		MOTODIR = 0;
	else 					MOTODIR = 1;
	MOTOPWM = speed;
	delay_ms(nms);
	MOTOPWM = 0;
}
