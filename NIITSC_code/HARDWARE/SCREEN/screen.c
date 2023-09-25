#include "screen.h"
#include "serial.h"
#include "delay.h"

void Screen_USART_Config(void);

//Note these are blocking func. 以下都是是阻塞函数
void Screen_Init(){
	Screen_USART_Config();
	
	unsigned char starter[9] = "CLR(0);";
	starter[7] = 0x0d;
	starter[8] = 0x0a;
	Serial_SendArray(USART6, &starter[0], 9);
	delay_ms(100);
	
	unsigned char starter2[9] = "DIR(1);";
	starter2[7] = 0x0d;
	starter2[8] = 0x0a;
	Serial_SendArray(USART6, &starter2[0], 9);
	delay_ms(100);
}

void Screen_USART_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(SCREEN_USART_TX_CLK
                        | SCREEN_USART_RX_CLK, ENABLE); //GPIO_CLK enable
    RCC_APB2PeriphClockCmd(SCREEN_USART_CLK, ENABLE);   //USART_CLK enable
    
    
    GPIO_InitStructure.GPIO_Pin = SCREEN_USART_TX_PIN 
                                | SCREEN_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCREEN_USART_TX_PORT, &GPIO_InitStructure);       
    GPIO_Init(SCREEN_USART_RX_PORT, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(SCREEN_USART_TX_PORT, SCREEN_USART_TX_PinSource, GPIO_AF_SCREEN_USART);
    GPIO_PinAFConfig(SCREEN_USART_RX_PORT, SCREEN_USART_RX_PinSource, GPIO_AF_SCREEN_USART);

    USART_InitStructure.USART_BaudRate = SCREEN_USART_BaudRate;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(SCREEN_USART,&USART_InitStructure); //初始化串口1
    USART_Cmd(SCREEN_USART,ENABLE); //使能串口1
    
    //USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
    NVIC_Init(&NVIC_InitStructure);
}

void Screen_Disp_Mission(u8 *qr_buff){
	unsigned char starter[26] = "DC32(10,10,'321+123',1);";
	starter[12] = qr_buff[0];
	starter[13] = qr_buff[1];
	starter[14] = qr_buff[2];
	starter[15] = qr_buff[3];
	starter[16] = qr_buff[4];
	starter[17] = qr_buff[5];
	starter[18] = qr_buff[6];
	starter[24] = 0x0d;
	starter[25] = 0x0a;
	Serial_SendArray(USART6, &starter[0], 26);
	delay_ms(100);
}

void Screen_Clear(void){
	unsigned char starter[9] = "CLR(0);";
	starter[7] = 0x0d;
	starter[8] = 0x0a;
	Serial_SendArray(USART6, &starter[0], 9);
	delay_ms(100);
}