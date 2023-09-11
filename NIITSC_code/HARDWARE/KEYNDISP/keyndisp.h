#ifndef __KEY_H
#define __KEY_H	 

#define ROW1  GPIO_Pin_15
#define ROW2  GPIO_Pin_14
#define ROW3  GPIO_Pin_13
#define COL1  GPIO_Pin_12
#define COL2  GPIO_Pin_11
#define COL3  GPIO_Pin_10

#define ROW_SET1 GPIO_SetBits(GPIOE,ROW1)
#define ROW_SET2 GPIO_SetBits(GPIOE,ROW2)
#define ROW_SET3 GPIO_SetBits(GPIOE,ROW3)

#define ROW_RESET1 GPIO_ResetBits(GPIOE,ROW1)
#define ROW_RESET2 GPIO_ResetBits(GPIOE,ROW2)
#define ROW_RESET3 GPIO_ResetBits(GPIOE,ROW3)			

#define COL_SCAN1 GPIO_ReadInputDataBit(GPIOE,COL1)
#define COL_SCAN2 GPIO_ReadInputDataBit(GPIOE,COL2)
#define COL_SCAN3 GPIO_ReadInputDataBit(GPIOE,COL3)

#include <stdint.h>

void Input_Disp_Loop(void);
void KEY_Init(void);//IO≥ı ºªØ
void System_Test(int d);			    

#endif


