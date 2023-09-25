#ifndef __KEY_H
#define __KEY_H

#define ROW_SET1 GPIO_SetBits(GPIOF,GPIO_Pin_15)
#define ROW_SET2 GPIO_SetBits(GPIOF,GPIO_Pin_11)
#define ROW_SET3 GPIO_SetBits(GPIOC,GPIO_Pin_4)

#define ROW_RESET1 GPIO_ResetBits(GPIOF,GPIO_Pin_15)
#define ROW_RESET2 GPIO_ResetBits(GPIOF,GPIO_Pin_11)
#define ROW_RESET3 GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define COL_SCAN1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define COL_SCAN2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)
#define COL_SCAN3 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1)

#include <stdint.h>

void Input_Disp_Loop(void);
void KEY_Init(void);//IO≥ı ºªØ
void System_Test(int d);			    

#endif


