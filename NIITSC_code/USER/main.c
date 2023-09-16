/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
#include "mpu.h"
#include "openmv.h"
#include "servo.h"
#include "qrcode.h"
#include "ops.h"
#include "screen.h"
#include "infrared.h"
#include "motor.h"
#include "control.h"
#include "led.h"
#include "keyndisp.h"
#include "flags.h"
#include "run.h"

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
	Control_Init();
	
//	MPU_USART_Config();
//	OPENMV_USART_Config();
//	ZIGBEE_USART_Config();
	QRCODE_UART_Config();
//	SCREEN_USART_Config();
	
//	Laser_Init();
	
	while (1)
	{
		if(!flag_ops_ready){
			OLED_ShowString(1, 1, "OPS prep.");
			continue;
		}
		Input_Disp_Loop();
		if (flag_start)
		{
//			speed_limit = 80;
//			Go_Position_Test(-0.15, 1.8, 0);
			speed_limit = 50;
			Run();
		} else
		{
			Set_Speed_All(0, 0, 0, 0);
		}
	}
}
