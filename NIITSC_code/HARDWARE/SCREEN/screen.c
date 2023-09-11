#include "screen.h"


void SCREEN_USART_Config(void) 
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
    
    USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
    NVIC_Init(&NVIC_InitStructure);
}
