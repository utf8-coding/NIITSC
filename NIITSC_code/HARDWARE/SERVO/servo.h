#ifndef __SERVO_H
#define	__SERVO_H
#include "stm32f4xx.h"
#include "stdio.h"
#include "LobotServoController.h"
#include "sys.h" 


//Òý½Å¶¨Òå
/*----------------------SERVO_USARTÅäÖÃºê ------------------------*/
#define SERVO_USART                           	USART3
#define SERVO_USART_BaudRate  					9600
#define SERVO_USART_CLK      					RCC_APB1Periph_USART3

#define SERVO_USART_TX_PIN						GPIO_Pin_10					
#define	SERVO_USART_TX_PORT					GPIOB					
#define	SERVO_USART_TX_CLK						RCC_AHB1Periph_GPIOB	
#define SERVO_USART_TX_PinSource    			GPIO_PinSource10			

#define SERVO_USART_RX_PIN						GPIO_Pin_11            
#define	SERVO_USART_RX_PORT					GPIOB                 
#define	SERVO_USART_RX_CLK						RCC_AHB1Periph_GPIOB    
#define SERVO_USART_RX_PinSource     			GPIO_PinSource11 

#define GPIO_AF_SERVO_USART                       GPIO_AF_USART3
#define SERVO_USART_IRQn                          USART3_IRQn




/************************************************************/
typedef enum{
	defaut,      
	calibObj,
	calibRough,
	getObj,
	getRough,
	putRough,
	putDownDep,
	putUpDep
}servoAction;

extern u8 USART_RX_BUF[64];     //½ÓÊÕ»º³å,×î´ó63¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u8 USART_RX_STA;         //½ÓÊÕ×´Ì¬±ê¼Ç	

void SERVO_USART_Config(void);
void uartWriteBuf(uint8_t *buf, uint8_t len);
//void servo_Action(servoAction actionNum, u16 Times);

void servoDefault(u16 nms);
void servoMvCalib(servoAction action, u16 nms);
void get_Obj(u16 nms);
void put_Rough(u16 nms);
void get_Rough(u16 nms);
void put_Up_Dep(u16 nms);
void put_Down_Dep(u16 nms);

#endif 
