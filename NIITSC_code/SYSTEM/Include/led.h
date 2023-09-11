#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED���ú� ------------------------*/

#define LED1_PIN             GPIO_Pin_13       		 // LED1 ����      
#define LED1_PORT            GPIOC                  // LED1 GPIO�˿�     
#define LED1_CLK             RCC_AHB1Periph_GPIOC	 // LED1 GPIO�˿�ʱ��

/*---------------------- LED���ƺ� ------------------------*/
					
#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	
#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN);			

/*---------------------- �������� ----------------------------*/

void LED_Init(void);	//LED��ʼ������
void LED_Flip(void);


#endif //__LED_H

