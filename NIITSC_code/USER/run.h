#ifndef __RUN_H
#define __RUN_H

#include "stm32f4xx.h"                  // Device header

typedef enum{
	qrcode = 0,
	objmode,
}runState;

void Run(void);

#endif
