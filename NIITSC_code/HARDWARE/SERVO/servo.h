#ifndef __ZIGBEE_H
#define	__ZIGBEE_H
#include "stm32f4xx.h"
#include "stdio.h"
 

//Òý½Å¶¨Òå
/*----------------------ZIGBEE_USARTÅäÖÃºê ------------------------*/
#define ZIGBEE_USART                           	USART3
#define ZIGBEE_USART_BaudRate  					9600
#define ZIGBEE_USART_CLK      					RCC_APB1Periph_USART3

#define ZIGBEE_USART_TX_PIN						GPIO_Pin_10					
#define	ZIGBEE_USART_TX_PORT					GPIOB					
#define	ZIGBEE_USART_TX_CLK						RCC_AHB1Periph_GPIOB	
#define ZIGBEE_USART_TX_PinSource    			GPIO_PinSource10			

#define ZIGBEE_USART_RX_PIN						GPIO_Pin_11            
#define	ZIGBEE_USART_RX_PORT					GPIOB                 
#define	ZIGBEE_USART_RX_CLK						RCC_AHB1Periph_GPIOB    
#define ZIGBEE_USART_RX_PinSource     			GPIO_PinSource11 

#define GPIO_AF_ZIGBEE_USART                       GPIO_AF_USART3
#define ZIGBEE_USART_IRQn                          USART3_IRQn
/************************************************************/
typedef enum{
	getObj,
	getRough,
	mid_flag,
	putRough,
	putUpDep,
	putDownDep
}servoAction;

extern u8 USART_RX_BUF[64];     //½ÓÊÕ»º³å,×î´ó63¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u8 USART_RX_STA;         //½ÓÊÕ×´Ì¬±ê¼Ç	

void ZIGBEE_USART_Config(void);
void uartWriteBuf(uint8_t *buf, uint8_t len);
void servo_Action(servoAction actionNum, u16 Times);

#endif 
