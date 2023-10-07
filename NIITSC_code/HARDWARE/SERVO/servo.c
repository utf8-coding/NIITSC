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

/*=============================default============================*/

void servoDefault(u16 nms){
	runActionGroup(defaut, 1);  //张开 向内
	delay_ms(nms);
}

/*=============================calib============================*/

// action can be calibObj or calibRough
void servoMvCalib(servoAction action, u16 nms){
	runActionGroup(action, 1); 
	delay_ms(nms);
}

/*=============================obj============================*/
void get_Obj(u16 nms){
	runActionGroup(getObj, 1);		
	delay_ms(nms);
}

/*===============================rough=============================*/

void put_Rough(u16 nms){
	runActionGroup(putRough, 1);	
	delay_ms(nms);
}

void get_Rough(u16 nms){
	runActionGroup(getRough, 1);
	delay_ms(nms);
}

/*===============================deposit=============================*/

//暂存区下层
void put_Down_Dep(u16 nms){
	runActionGroup(putDownDep, 1);	
	delay_ms(nms);
}

//暂存区上层
void put_Up_Dep(u16 nms){
	runActionGroup(putUpDep, 1);	
	delay_ms(nms);
}







