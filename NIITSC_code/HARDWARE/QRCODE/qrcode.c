#include "qrcode.h"

void QRCODE_UART_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHB1PeriphClockCmd(QRCODE_UART_TX_CLK
            | QRCODE_UART_RX_CLK, ENABLE); //GPIO_CLK enable
  RCC_APB1PeriphClockCmd(QRCODE_UART_CLK, ENABLE);  //UART_CLK enable
  
  GPIO_InitStructure.GPIO_Pin = QRCODE_UART_TX_PIN 
                | QRCODE_UART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(QRCODE_UART_TX_PORT, &GPIO_InitStructure);    
  GPIO_Init(QRCODE_UART_RX_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(QRCODE_UART_TX_PORT, QRCODE_UART_TX_PinSource, GPIO_AF_QRCODE_UART);
  GPIO_PinAFConfig(QRCODE_UART_RX_PORT, QRCODE_UART_RX_PinSource, GPIO_AF_QRCODE_UART);

  USART_InitStructure.USART_BaudRate = QRCODE_UART_BaudRate;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
  USART_Init(QRCODE_UART,&USART_InitStructure); 
  USART_Cmd(QRCODE_UART,ENABLE);
  
  USART_ITConfig(QRCODE_UART, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = QRCODE_UART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
  NVIC_Init(&NVIC_InitStructure);
}
