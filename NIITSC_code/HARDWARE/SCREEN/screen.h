#ifndef __SCREEN_H
#define	__SCREEN_H

#include "stm32f4xx.h"
#include <stdio.h>

/*----------------------SCREEN_USART≈‰÷√∫Í ------------------------*/
#define SCREEN_USART                           	USART6
#define SCREEN_USART_BaudRate  					115200
#define SCREEN_USART_CLK      					RCC_APB2Periph_USART6

#define SCREEN_USART_TX_PIN						GPIO_Pin_6					
#define	SCREEN_USART_TX_PORT					GPIOC					
#define	SCREEN_USART_TX_CLK						RCC_AHB1Periph_GPIOA	
#define SCREEN_USART_TX_PinSource    			GPIO_PinSource6			

#define SCREEN_USART_RX_PIN						GPIO_Pin_7            
#define	SCREEN_USART_RX_PORT					GPIOC                 
#define	SCREEN_USART_RX_CLK						RCC_AHB1Periph_GPIOC    
#define SCREEN_USART_RX_PinSource     			GPIO_PinSource7 

#define GPIO_AF_SCREEN_USART                    GPIO_AF_USART6
#define SCREEN_USART_IRQn                       USART6_IRQn


void Screen_Init(void);
void Screen_Clear(void);
void Screen_Disp_Mission(u8 *qr_buff);


#endif
