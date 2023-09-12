#ifndef __RUN_H
#define __RUN_H

#include "stm32f4xx.h"                  // Device header

extern u8 color_Index;

typedef enum{
	qrcode = 0,
	objmode,
	roughmode
}runState;

void Run(void);

#endif
