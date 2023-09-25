#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h"
#include "sys.h"

/*==============================ENCODER1============================*/
#define ENCODER1_CNT                                  (short)TIM2->CNT
	
#define ENCODER1_PIN_1           					   GPIO_Pin_15       		 // ENCODER1 引脚  
#define ENCODER1_PORT_1          					   GPIOA                 // ENCODER1 GPIO端口 
#define ENCODER1_SOURCE_1        					   GPIO_PinSource15       // ENCODER1 GPIO端口 
#define ENCODER1_PIN_CLK_1      					   RCC_AHB1Periph_GPIOA	 // ENCODER1 GPIO端口时钟

#define ENCODER1_PIN_2          					   GPIO_Pin_3       		 // ENCODER1 引脚  
#define ENCODER1_PORT_2          					   GPIOB                 // ENCODER1 GPIO端口  
#define ENCODER1_SOURCE_2        					   GPIO_PinSource3       // ENCODER1 GPIO端口  
#define ENCODER1_PIN_CLK_2     						   RCC_AHB1Periph_GPIOB	 // ENCODER1 GPIO端口时钟
						 
#define ENCODER1_TIM           						   TIM2                  // ENCODER1 GPIO端口  
#define ENCODER1_AF            						   GPIO_AF_TIM2
#define ENCODER1_TIM_CLK       						   RCC_APB1Periph_TIM2	 // ENCODER1 GPIO端口时钟

/*==============================ENCODER2============================*/
#define ENCODER2_CNT                                  (short)TIM5->CNT
	
#define ENCODER2_PIN_1           					   GPIO_Pin_0       		 // ENCODER2 引脚  
#define ENCODER2_PORT_1          					   GPIOA                 // ENCODER2 GPIO端口 
#define ENCODER2_SOURCE_1        					   GPIO_PinSource0      // ENCODER2 GPIO端口 
#define ENCODER2_PIN_CLK_1      					   RCC_AHB1Periph_GPIOA	 // ENCODER2 GPIO端口时钟

#define ENCODER2_PIN_2          					   GPIO_Pin_1       		 // ENCODER2 引脚  
#define ENCODER2_PORT_2          					   GPIOA                 // ENCODER2 GPIO端口  
#define ENCODER2_SOURCE_2        					   GPIO_PinSource1       // ENCODER2 GPIO端口  
#define ENCODER2_PIN_CLK_2     						   RCC_AHB1Periph_GPIOA	 // ENCODER2 GPIO端口时钟
						 
#define ENCODER2_TIM           						   TIM5                  // ENCODER2 GPIO端口  
#define ENCODER2_AF            						   GPIO_AF_TIM5
#define ENCODER2_TIM_CLK       						   RCC_APB1Periph_TIM5	 // ENCODER2 GPIO端口时钟

/*==============================ENCODER3============================*/
#define ENCODER3_CNT                                  (short)TIM1->CNT
	
#define ENCODER3_PIN_1           					   GPIO_Pin_9       		 // ENCODER3 引脚  
#define ENCODER3_PORT_1          					   GPIOE                 // ENCODER3 GPIO端口 
#define ENCODER3_SOURCE_1        					   GPIO_PinSource9       // ENCODER3 GPIO端口 
#define ENCODER3_PIN_CLK_1      					   RCC_AHB1Periph_GPIOE	 // ENCODER3 GPIO端口时钟

#define ENCODER3_PIN_2          					   GPIO_Pin_11       		 // ENCODER3 引脚  
#define ENCODER3_PORT_2          					   GPIOE                 // ENCODER3 GPIO端口  
#define ENCODER3_SOURCE_2        					   GPIO_PinSource11       // ENCODER3 GPIO端口  
#define ENCODER3_PIN_CLK_2     						   RCC_AHB1Periph_GPIOE	 // ENCODER3 GPIO端口时钟
						 
#define ENCODER3_TIM           						   TIM1                  // ENCODER3 GPIO端口  
#define ENCODER3_AF            						   GPIO_AF_TIM1
#define ENCODER3_TIM_CLK       						   RCC_APB2Periph_TIM1	 // ENCODER3 GPIO端口时钟


/*==============================ENCODER4============================*/
#define ENCODER4_CNT                                  (short)TIM4->CNT
	
#define ENCODER4_PIN_1           					   GPIO_Pin_12       		 // ENCODER4 引脚  
#define ENCODER4_PORT_1          					   GPIOD                 // ENCODER4 GPIO端口 
#define ENCODER4_SOURCE_1        					   GPIO_PinSource12       // ENCODER4 GPIO端口 
#define ENCODER4_PIN_CLK_1      					   RCC_AHB1Periph_GPIOD	 // ENCODER4 GPIO端口时钟

#define ENCODER4_PIN_2          					   GPIO_Pin_13       		 // ENCODER4 引脚  
#define ENCODER4_PORT_2          					   GPIOD                 // ENCODER4 GPIO端口  
#define ENCODER4_SOURCE_2        					   GPIO_PinSource13       // ENCODER4 GPIO端口  
#define ENCODER4_PIN_CLK_2     						   RCC_AHB1Periph_GPIOD	 // ENCODER4 GPIO端口时钟
						 
#define ENCODER4_TIM           						   TIM4                  // ENCODER4 GPIO端口  
#define ENCODER4_AF            						   GPIO_AF_TIM4
#define ENCODER4_TIM_CLK       						   RCC_APB1Periph_TIM4	 // ENCODER4 GPIO端口时钟

/*==============================EXTERNAL ACCESS============================*/
#define SPEED_ENC_RATIO 1 //编码器数值与速度比 in m/s
#define Motor_A_Speed ENCODER2_CNT*SPEED_ENC_RATIO
#define Motor_B_Speed ENCODER1_CNT*SPEED_ENC_RATIO
#define Motor_C_Speed ENCODER3_CNT*SPEED_ENC_RATIO
#define Motor_D_Speed ENCODER4_CNT*SPEED_ENC_RATIO

/*==============================FUNCTION============================*/
typedef enum{
	encoder_Cnt_1,
	encoder_Cnt_2,
	encoder_Cnt_3,
	encoder_Cnt_4,
	encoder_Cnt_Num
}Encoder_Cnt_Enum;
extern int encoder_Count_Buff[encoder_Cnt_Num];

void Encoder_Tim_Config(u16 arr, u16 psc);
void Encoder_Init_All(void);
void Encoder1_Init(void);
void Encoder2_Init(void);
void Encoder3_Init(void);	
void Encoder4_Init(void);	
int16_t Encoder_Get(void);
//int Read_Velocity(u8 TIMX);
int Read_Velocity(TIM_TypeDef* TIMX);
#endif
