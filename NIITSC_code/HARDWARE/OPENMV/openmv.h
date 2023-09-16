#ifndef __OPENMV_H
#define	__OPENMV_H

#include "stm32f4xx.h"

/*----------------------OPENMV_USART≈‰÷√∫Í ------------------------*/
#define OPENMV_USART                  USART2
#define OPENMV_USART_BaudRate  				115200
#define OPENMV_USART_CLK      				RCC_APB1Periph_USART2

#define OPENMV_USART_TX_PIN						GPIO_Pin_2					
#define	OPENMV_USART_TX_PORT					GPIOA					
#define	OPENMV_USART_TX_CLK						RCC_AHB1Periph_GPIOA	
#define OPENMV_USART_TX_PinSource    	GPIO_PinSource2			

#define OPENMV_USART_RX_PIN						GPIO_Pin_3            
#define	OPENMV_USART_RX_PORT					GPIOA                 
#define	OPENMV_USART_RX_CLK						RCC_AHB1Periph_GPIOA    
#define OPENMV_USART_RX_PinSource     GPIO_PinSource3 

#define GPIO_AF_OPENMV_USART          GPIO_AF_USART2
#define OPENMV_USART_IRQn             USART2_IRQn

#define OPENMV_HEADER 								0x0055
#define OPENMV_TAIL										0x66ff

extern int mv_target_cx, mv_target_cy;
extern u8 mark1;

void OPENMV_USART_Config(void);
void OpenMV_Display_Specs(void);
int OpenMV_Change_Mode(u8 mode);

#endif
