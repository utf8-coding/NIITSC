#include "stm32f4xx.h"
#include "servo.h"  
#include "string.h"
#include "bool.h"
#include "LobotServoController.h"
#include "run.h"
u8 UART_RX_BUF[16];
bool isUartRxCompleted = false;

int fputc(int ch, FILE *f)
{
        if(ch =='\n')  
        {  
            USART_SendData(ZIGBEE_USART, 0x0D);  
            while (!(ZIGBEE_USART->SR & USART_FLAG_TXE)); 
            USART_SendData(ZIGBEE_USART, 0x0A);  
            while (!(ZIGBEE_USART->SR & USART_FLAG_TXE));  
        }  
        else  
        {  
            USART_SendData(ZIGBEE_USART, (unsigned char)ch);  
            while (!(ZIGBEE_USART->SR & USART_FLAG_TXE));   
        }  
	return ch;
}

void ZIGBEE_USART_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(ZIGBEE_USART_TX_CLK
                        | ZIGBEE_USART_RX_CLK, ENABLE); //GPIO_CLK enable
    RCC_APB1PeriphClockCmd(ZIGBEE_USART_CLK, ENABLE);   //USART_CLK enable
    
    GPIO_InitStructure.GPIO_Pin = ZIGBEE_USART_TX_PIN 
                                | ZIGBEE_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ZIGBEE_USART_TX_PORT, &GPIO_InitStructure);       
    GPIO_Init(ZIGBEE_USART_RX_PORT, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(ZIGBEE_USART_TX_PORT, ZIGBEE_USART_TX_PinSource, GPIO_AF_ZIGBEE_USART);
    GPIO_PinAFConfig(ZIGBEE_USART_RX_PORT, ZIGBEE_USART_RX_PinSource, GPIO_AF_ZIGBEE_USART);

    USART_InitStructure.USART_BaudRate = ZIGBEE_USART_BaudRate;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(ZIGBEE_USART,&USART_InitStructure); 
    USART_Cmd(ZIGBEE_USART,ENABLE);
    
    USART_ITConfig(ZIGBEE_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = ZIGBEE_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
    NVIC_Init(&NVIC_InitStructure);
}

void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) {
		while ((ZIGBEE_USART->SR & 0x40) == 0);
		USART_SendData(ZIGBEE_USART,*buf++);
	}
}

extern uint8_t LobotRxBuf[16];

void ZIGBEE_USART_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	if (USART_GetITStatus(ZIGBEE_USART, USART_IT_RXNE) != RESET) { //??????
		Res = USART_ReceiveData(ZIGBEE_USART);//(ZIGBEE_USART->DR);	//?????????
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



void servo_Action(servoAction actionNum, u16 Times){
	runActionGroup(actionNum, Times);
	if( actionNum < mid_flag ){
		color_Index++;
	}
	else{
		color_Index--;
	}
}
	