/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
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
#include "buzzer.h"
#include "serial.h"
#include "encoder.h"

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
	Encoder_Init_All();
	Screen_Init();
	
	QRCODE_UART_Config();
	SERVO_USART_Config();
	Moto_Init(18 - 1, 8400 -1);
	Laser_Init();
	
	
//	OPENMV_USART_Config();
//	BUZZER_Init();
//	BUZZER_Ms();

	servoDefault();
	delay_ms(1000);
	
	while (1)
	{
		OpenMV_Data_Process();
		OPS_Data_Process();
		Input_Disp_Loop();
		
		if(!flag_ops_ready){
			OLED_ShowString(1, 1, "OPS prep.");
			continue;
		}
		
		if (flag_start)
		{
			Run();
//			speed_limit = 80
//			Set_Control_Mode(coordinateMode);
//			Go_Position_Test(-0.15, 1.5, 90);
			
//			Set_Control_Mode(velocityMode);
//			Set_Speed(-0.2, 0, 0);
//			Wheel_Run_Loop();
		} else
		{
			Set_Speed_All(0, 0, 0, 0);
		}
	}
}
