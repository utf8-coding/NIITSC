#include "zigbee.h"  

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

