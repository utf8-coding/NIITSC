#ifndef __QRCODE_H
#define __QRCODE_H
#include "stm32f4xx.h"
#include <stdio.h>


/*----------------------QRCODE_UARTÅäÖÃºê ------------------------*/
#define QRCODE_UART                           	UART4
#define QRCODE_UART_BaudRate  					115200
#define QRCODE_UART_CLK      					RCC_APB1Periph_UART4

#define QRCODE_UART_TX_PIN						GPIO_Pin_10					
#define	QRCODE_UART_TX_PORT					GPIOC					
#define	QRCODE_UART_TX_CLK						RCC_AHB1Periph_GPIOC	
#define QRCODE_UART_TX_PinSource    			GPIO_PinSource10			

#define QRCODE_UART_RX_PIN						GPIO_Pin_11            
#define	QRCODE_UART_RX_PORT					GPIOC                 
#define	QRCODE_UART_RX_CLK						RCC_AHB1Periph_GPIOC    
#define QRCODE_UART_RX_PinSource     			GPIO_PinSource11 

#define GPIO_AF_QRCODE_UART                       GPIO_AF_UART4
#define QRCODE_UART_IRQn                          UART4_IRQn

#define QR_BUFF_SIZE                     		8


typedef enum {
	qrWrong,
	qrRight
}qrRecState;

enum qrColor{
	red = 0x31,
	green,
	blue,
	plus = 0x2B,
	qrEnd = 0x0D
};

void QRCODE_UART_Config(void);
qrRecState QR_Ready(void);

extern u8 qr_buff[QR_BUFF_SIZE];
extern qrRecState qr_state;

#endif
