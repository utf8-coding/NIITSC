#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h" 

#define AIN1   PDout(0)  
#define AIN2   PDout(1)  

#define BIN1   PDout(5)  
#define BIN2   PDout(4)  

#define CIN1   PAout(11) 
#define CIN2   PAout(12) 

#define DIN1   PGout(8)  
#define DIN2   PGout(7)  

#define PWMA   TIM3->CCR1
#define PWMB   TIM3->CCR2
#define PWMC   TIM3->CCR3
#define PWMD   TIM3->CCR4

#define MOTOT_12_ENA 			    PGout(11) 
#define MOTOT_12_ENA_PIN			GPIO_Pin_11
#define MOTOT_12_ENA_PORT			GPIOG
#define MOTOT_12_ENA_CLK			RCC_AHB1Periph_GPIOG

#define MOTOT_34_ENA 				PAout(8)
#define MOTOT_34_ENA_PIN			GPIO_Pin_8
#define MOTOT_34_ENA_PORT           GPIOA
#define MOTOT_34_ENA_CLK            RCC_AHB1Periph_GPIOA


#define AIN1_GPIO_PIN       GPIO_Pin_0
#define AIN2_GPIO_PIN   		GPIO_Pin_1
#define AIN_GPIO_PORT  			GPIOD
#define AIN_GPIO_CLK 				RCC_AHB1Periph_GPIOD


#define BIN1_GPIO_PIN       GPIO_Pin_5
#define BIN2_GPIO_PIN   		GPIO_Pin_4
#define BIN_GPIO_PORT  			GPIOD
#define BIN_GPIO_CLK 				RCC_AHB1Periph_GPIOD


#define CIN1_GPIO_PIN       GPIO_Pin_11
#define CIN2_GPIO_PIN   		GPIO_Pin_12
#define CIN_GPIO_PORT  			GPIOA
#define CIN_GPIO_CLK 				RCC_AHB1Periph_GPIOA

#define DIN1_GPIO_PIN       GPIO_Pin_8
#define DIN2_GPIO_PIN   		GPIO_Pin_7
#define DIN_GPIO_PORT  			GPIOG
#define DIN_GPIO_CLK 				RCC_AHB1Periph_GPIOG

/*---------------------- MOTORA_PWM------------------------*/

#define MOTORA_PWM_PIN           						   GPIO_Pin_4       		 // MOTOR1_PWM 引脚      
#define MOTORA_PWM_PORT          						   GPIOB                 // MOTOR1_PWM GPIO端口  
#define MOTORA_PWM_SOURCE        						   GPIO_PinSource5       // MOTOR1_PWM GPIO端口  
#define MOTORA_PWM_PIN_CLK       						   RCC_AHB1Periph_GPIOB	 // MOTOR1_PWM GPIO端口时钟
						 
#define MOTORA_PWM_TIM           						   TIM3                  // MOTOR1_PWM GPIO端口  
#define MOTORA_PWM_AF            						   GPIO_AF_TIM3
#define MOTORA_PWM_TIM_CLK       						   RCC_APB1Periph_TIM3	 // MOTOR1_PWM GPIO端口时钟
#define MOTORA_PWM_OC_INIT(initStructure)			 TIM_OC1Init(MOTORA_PWM_TIM, (TIM_OCInitTypeDef*)(initStructure))
#define MOTORA_PWM_OC_PRELOAD_CFIG() 					 TIM_OC1PreloadConfig(MOTORA_PWM_TIM, TIM_OCPreload_Enable)

/*---------------------- MOTORB_PWM------------------------*/

#define MOTORB_PWM_PIN       						       GPIO_Pin_5       		 // MOTOR2_PWM 引脚      
#define MOTORB_PWM_PORT      						       GPIOB                  // MOTOR2_PWM GPIO端口    
#define MOTORB_PWM_SOURCE    						       GPIO_PinSource4                  // MOTOR2_PWM GPIO端口  
#define MOTORB_PWM_PIN_CLK   						       RCC_AHB1Periph_GPIOB	 // MOTOR2_PWM GPIO端口时钟
																				 
#define MOTORB_PWM_TIM       						       TIM3                  // MOTOR2_PWM GPIO端口  
#define MOTORB_PWM_AF        						       GPIO_AF_TIM3
#define MOTORB_PWM_TIM_CLK   						       RCC_APB1Periph_TIM3	 // MOTOR2_PWM GPIO端口时钟
#define MOTORB_PWM_OC_INIT(initStructure)			 TIM_OC2Init(MOTORB_PWM_TIM, (TIM_OCInitTypeDef*)(initStructure))
#define MOTORB_PWM_OC_PRELOAD_CFIG() 					 TIM_OC2PreloadConfig(MOTORB_PWM_TIM, TIM_OCPreload_Enable)

/*---------------------- MOTORC_PWM------------------------*/

#define MOTORC_PWM_PIN           						   GPIO_Pin_8      		 // MOTOR3_PWM 引脚      
#define MOTORC_PWM_PORT          						   GPIOC                  // MOTOR3_PWM GPIO端口   
#define MOTORC_PWM_SOURCE        						   GPIO_PinSource8                  // MOTOR2_PWM GPIO端口  
#define MOTORC_PWM_PIN_CLK       						   RCC_AHB1Periph_GPIOC	 // MOTOR3_PWM GPIO端口时钟
																						 
#define MOTORC_PWM_TIM           						   TIM3                  // MOTOR3_PWM GPIO端口  
#define MOTORC_PWM_AF            						   GPIO_AF_TIM3
#define MOTORC_PWM_TIM_CLK       						   RCC_APB1Periph_TIM3	 // MOTOR3_PWM GPIO端口时钟
#define MOTORC_PWM_OC_INIT(initStructure)			 TIM_OC3Init(MOTORC_PWM_TIM, (TIM_OCInitTypeDef*)(initStructure))
#define MOTORC_PWM_OC_PRELOAD_CFIG() 					 TIM_OC3PreloadConfig(MOTORC_PWM_TIM, TIM_OCPreload_Enable)

/*---------------------- MOTORD_PWM------------------------*/

#define MOTORD_PWM_PIN          						   GPIO_Pin_9       		 // MOTOR4_PWM 引脚      
#define MOTORD_PWM_PORT         						   GPIOC                  // MOTOR4_PWM GPIO端口     
#define MOTORD_PWM_SOURCE       						   GPIO_PinSource9                  // MOTOR4_PWM GPIO端口  
#define MOTORD_PWM_PIN_CLK      						   RCC_AHB1Periph_GPIOC	 // MOTOR4_PWM GPIO端口时钟
						
#define MOTORD_PWM_TIM          						   TIM3                  // MOTOR4_PWM GPIO端口  
#define MOTORD_PWM_AF           						   GPIO_AF_TIM3
#define MOTORD_PWM_TIM_CLK      						   RCC_APB1Periph_TIM3	 // MOTOR4_PWM GPIO端口时钟
#define MOTORD_PWM_OC_INIT(initStructure)			 TIM_OC4Init(MOTORD_PWM_TIM, (TIM_OCInitTypeDef*)(initStructure))
#define MOTORD_PWM_OC_PRELOAD_CFIG()					 TIM_OC4PreloadConfig(MOTORD_PWM_TIM, TIM_OCPreload_Enable)

/*-----------------------------------------------------*/
void Motor_Init(void);

void MotorA_SetSpeed(int8_t Speed);
void MotorB_SetSpeed(int8_t Speed);
void MotorC_SetSpeed(int8_t Speed);
void MotorD_SetSpeed(int8_t Speed);

void Set_Speed_All(int8_t speedA,int8_t speedB,int8_t speedC,int8_t speedD);

#endif
