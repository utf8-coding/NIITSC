/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
#include "mpu.h"
#include "openmv.h"
#include "zigbee.h"
#include "qrcode.h"
#include "ops.h"
#include "screen.h"
#include "infrared.h"
#include "motor.h"
#include "control.h"
#include "led.h"
#include "keyndisp.h"
#include "flags.h"

/* Private functions ---------------------------------------------------------*/
int main(void)
{
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	KEY_Init();
	LED_Init();
	OPS_Init();
	Motor_Init();
	OLED_Init();
	
//	MPU_USART_Config();
//	OPENMV_USART_Config();
//	ZIGBEE_USART_Config();
//	QRCODE_UART_Config();
//	SCREEN_USART_Config();
	
//	Laser_Init();
	
	
//	OLED_ShowChar(1, 1, 'A');
//	OLED_ShowString(1, 3, "HelloWorld!");
//	OLED_ShowNum(2, 1, 12345, 5);
//	OLED_ShowSignedNum(2, 7, -66, 2);
//	OLED_ShowHexNum(3, 1, 0xAA55, 4);
//	OLED_ShowBinNum(4, 1, 0xAA55, 16);

	while (1)
	{
		Input_Disp_Loop();
		if (flag_start)
		{
			Set_Speed_All(50, 50, 50, 50);
		} else
		{
			Set_Speed_All(0, 0, 0, 0);
		}
	}
}
