#ifndef __OPS_H
#define __OPS_H

#include "stm32f4xx.h"



/*----------------------OPS_UARTÅäÖÃºê ------------------------*/
#define OPS_UART                           	UART5
#define OPS_UART_BaudRate  					115200
#define OPS_UART_CLK      						RCC_APB1Periph_UART5

#define OPS_UART_TX_PIN						GPIO_Pin_12					
#define	OPS_UART_TX_PORT						GPIOC					
#define	OPS_UART_TX_CLK						RCC_AHB1Periph_GPIOC	
#define OPS_UART_TX_PinSource    				GPIO_PinSource12			

#define OPS_UART_RX_PIN						GPIO_Pin_2            
#define	OPS_UART_RX_PORT						GPIOD                 
#define	OPS_UART_RX_CLK						RCC_AHB1Periph_GPIOD    
#define OPS_UART_RX_PinSource     				GPIO_PinSource2 

#define GPIO_AF_OPS_UART                       GPIO_AF_UART5
#define OPS_UART_IRQn                          UART5_IRQn

extern float OPS_x, OPS_y, OPS_heading;

void OPS_Init(void);
void OPS_Display_Specs(void);

// #define pos_x -OPS.ActVal[4]
// #define pos_y -OPS.ActVal[5]
// #define zangle OPS.ActVal[1]//+360.00*ring//-cali_z
// #define xangle -OPS.ActVal[2]
// #define yangle -OPS.ActVal[3]
// #define w_z -OPS.ActVal[6]
// typedef union{
	
// 	uint8_t data[32];
// 	float ActVal[8];
	
// }Union_OPS;


// void OPS_DMA_Init(u32 bound); 
// void OPS_Inspection_TIM_init(u16 arr,u16 psc);
// void PC_SendChar(uint8_t DataToSend);
// void PC_SendString(uint8_t *str);
// void Cali_Ops(void);
// void Update_X(float posx);
// void Update_Y(float posy);
// void Update_Z(float posz);
// void Stract(char str1[],uint8_t str2[],u8 num);
// void Self_checking(void);




#endif




