#ifndef __MPU_H
#define	__MPU_H

#include "stm32f4xx.h"
#include <stdio.h>

/*----------------------MPU_USART≈‰÷√∫Í ------------------------*/
#define MPU_USART                   USART1
#define MPU_USART_BaudRate  				115200
#define MPU_USART_CLK      					RCC_APB2Periph_USART1

#define MPU_USART_TX_PIN						GPIO_Pin_9					
#define	MPU_USART_TX_PORT						GPIOA					
#define	MPU_USART_TX_CLK						RCC_AHB1Periph_GPIOA	
#define MPU_USART_TX_PinSource    	GPIO_PinSource9			

#define MPU_USART_RX_PIN						GPIO_Pin_10            
#define	MPU_USART_RX_PORT						GPIOA                 
#define	MPU_USART_RX_CLK						RCC_AHB1Periph_GPIOA    
#define MPU_USART_RX_PinSource     	GPIO_PinSource10      

#define GPIO_AF_MPU_USART           GPIO_AF_USART1
#define MPU_USART_IRQn              USART1_IRQn


void MPU_USART_Config(void);


#endif
