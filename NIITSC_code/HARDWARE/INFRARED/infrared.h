#ifndef __INFRARED_H
#define __INFRARED_H

#include "stm32f4xx.h"

/*---------------------- INFRARED���ú� ------------------------*/

#define INFRARED1_PIN             GPIO_Pin_0       		 // INFRARED1 ����      
#define INFRARED1_PORT            GPIOC                  // INFRARED1 GPIO�˿�     
#define INFRARED1_CLK             RCC_AHB1Periph_GPIOC	 // INFRARED1 GPIO�˿�ʱ��
	

#define INFRARED2_PIN             GPIO_Pin_1       		 // INFRARED2 ����      
#define INFRARED2_PORT            GPIOC                  // INFRARED2 GPIO�˿�     
#define INFRARED2_CLK             RCC_AHB1Periph_GPIOC	 // INFRARED2 GPIO�˿�ʱ��

#define INFRARED3_PIN             GPIO_Pin_2       		 // INFRARED3 ����      
#define INFRARED3_PORT            GPIOC                  // INFRARED3 GPIO�˿�     
#define INFRARED3_CLK             RCC_AHB1Periph_GPIOC	 // INFRARED1 GPIO�˿�ʱ��

#define INFRARED4_PIN             GPIO_Pin_3       		 // INFRARED4 ����      
#define INFRARED4_PORT            GPIOC                  // INFRARED4 GPIO�˿�     
#define INFRARED4_CLK             RCC_AHB1Periph_GPIOC	 // INFRARED4 GPIO�˿�ʱ��


#define INFRARED_ON                1
#define INFRARED_OFF               0
/*---------------------- �������� ----------------------------*/
void Laser_Init(void);
void INFRARED1_Init(void);
void INFRARED2_Init(void);
void INFRARED3_Init(void);
void INFRARED4_Init(void);
u8	Infrared_Scan(void);
#endif //__LED_H

