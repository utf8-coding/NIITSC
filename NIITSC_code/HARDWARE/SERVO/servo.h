#ifndef __SERVO_H
#define	__SERVO_H
#include "stm32f4xx.h"
#include "stdio.h"
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



#define MOTO_GPIO_PIN       					GPIO_Pin_6
#define MOTO_GPIO_PORT  						GPIOE
#define MOTO_GPIO_CLK 							RCC_AHB1Periph_GPIOE


#define MOTODIR    										PEout(6)     
#define MOTOPWM   										TIM10->CCR1

#define MOTO_PWM_PIN           						   GPIO_Pin_6       		 // MOTOR1_PWM Òý½Å      
#define MOTO_PWM_PORT          						   GPIOF                 // MOTOR1_PWM GPIO¶Ë¿Ú  
#define MOTO_PWM_SOURCE        						   GPIO_PinSource6       // MOTOR1_PWM GPIO¶Ë¿Ú  
#define MOTO_PWM_PIN_CLK       						   RCC_AHB1Periph_GPIOF	 // MOTOR1_PWM GPIO¶Ë¿ÚÊ±ÖÓ
						 
#define MOTO_PWM_TIM           						   TIM10                 // MOTOR1_PWM GPIO¶Ë¿Ú  
#define MOTO_PWM_AF            						   GPIO_AF_TIM10
#define MOTO_PWM_TIM_CLK       						   RCC_APB2Periph_TIM10	 // MOTOR1_PWM GPIO¶Ë¿ÚÊ±ÖÓ
//#define MOTO_PWM_OC_INIT(initStructure)			 TIM_OC2Init(MOTO_PWM_TIM, (TIM_OCInitTypeDef*)(initStructure))
//#define MOTO_PWM_OC_PRELOAD_CFIG() 					 TIM_OC2PreloadConfig(MOTO_PWM_TIM, TIM_OCPreload_Enable)
/************************************************************/
//typedef enum{
//	getObj,
//	getRough,
//	mid_flag,
//	putRough,
//	putUpDep,
//	putDownDep
//}servoAction;


typedef enum{
	defaut,   
	calib,
	getObj,
	getRough,
	putRough,
	putUpDep,
	putDownDep
}servoAction;

typedef enum{	
	backOpen,           //0
	screwFront,		//1
	handGet, 		
	handRelease,
	plateRotate1,
	plateRotate2,
	plateRotate3,
	screwBack
}servoTypeEnum;

typedef enum{
	screw
}screwStateEnum;

typedef enum{
	down,
	up
}motoDirEnum;


extern u8 USART_RX_BUF[64];     //½ÓÊÕ»º³å,×î´ó63¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u8 USART_RX_STA;         //½ÓÊÕ×´Ì¬±ê¼Ç	

void SERVO_USART_Config(void);
void uartWriteBuf(uint8_t *buf, uint8_t len);
//void servo_Action(servoAction actionNum, u16 Times);
void servo_Action(servoAction actionNum, servoTypeEnum platePos);

void Moto_Init(u32 arr, u32 psc);
void Moto_Dir_Init(void);
void MOTO_PWM_Init(u32 arr, u32 psc);
void moto_Action(motoDirEnum dir, int8_t speed);

#endif 
