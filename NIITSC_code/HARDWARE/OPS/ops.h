#ifndef __OPS_H
#define __OPS_H

#include "stm32f4xx.h"

/*----------------------OPS_UARTÅäÖÃºê ------------------------*/
#define OPS_UART                    USART1
#define OPS_UART_BaudRate  					115200
#define OPS_UART_CLK      					RCC_APB2Periph_USART1
                                    
#define OPS_UART_TX_PIN							GPIO_Pin_9					
#define	OPS_UART_TX_PORT						GPIOA					
#define	OPS_UART_TX_CLK							RCC_AHB1Periph_GPIOA	
#define OPS_UART_TX_PinSource    		GPIO_PinSource9			
                                    
#define OPS_UART_RX_PIN							GPIO_Pin_10            
#define	OPS_UART_RX_PORT						GPIOA                  
#define	OPS_UART_RX_CLK							RCC_AHB1Periph_GPIOA     
#define OPS_UART_RX_PinSource     	GPIO_PinSource10      
                                    
#define GPIO_AF_OPS_UART            GPIO_AF_USART1
#define OPS_UART_IRQn               USART1_IRQn

typedef union{
	u8 data[32];
	float ActVal[8];
}OPS;


/*----------------- OPS DMA Related parameters (USART1_RX) -------------------*/
//两个指针没有放在这里，不知道怎么写。。。不要暴露给外界但是同时要是常量。
#define OPS_DMA_Stream 				 DMA2_Stream2
#define OPS_DMA_CLK 					 RCC_AHB1Periph_DMA2
#define OPS_DMA_Channel 			 DMA_Channel_4
#define OPS_DMA_Buf_Size			 28

extern float OPS_x, OPS_y, OPS_heading;
extern int32_t OPS_ring;

void OPS_Init(void);
void OPS_Display_Specs(void);
void OPS_Calibrate(float x, float y, float heading);
void OPS_Data_Process(void);



#endif




