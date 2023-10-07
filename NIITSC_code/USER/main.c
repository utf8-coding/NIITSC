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
	Laser_Init();
	
	QRCODE_UART_Config();
	SERVO_USART_Config();
	
//	OPENMV_USART_Config();

	servoDefault();
	delay_ms(1000);
	
	int period_temp = 0;
	while (1)
	{
		OPS_Data_Process();
		if(!flag_ops_ready){
			OLED_ShowString(1, 1, "OPS prep.");
			continue;
		}
		
		Control_Loop();
		OpenMV_Data_Process();
		Input_Disp_Loop();
		
		if (flag_start)
		{
			Target_Run(0, 0.65, 0);
			if(flag_arrive)
			{
				LED_Flip();
//				Stop_Run();
//				break;
			}
//			Run();
//			speed_limit = 80
//			Set_Control_Mode(coordinateMode);
//			Go_Position_Test(-0.15, 1.5, 90);
			
//			Set_Control_Mode(velocityMode);
//			Wheel_Run_Loop();
			
		} else
		{
			Set_Pwm_All(0, 0, 0, 0);
		}
	}
}

//			if(period_temp == 0)
//			{
//				Set_Discounter(20);
//				period_temp++;
//			}
//			
//			if(period_temp ==1)
//			{
//				Rel_Speed_Run(0, 0.25, 0);
//				if(Discounter_Arrive())
//				{
//					LED_Flip();
//					Set_Discounter(50);
//					period_temp ++;
//				}
//			}
//			
//			if(period_temp ==2)
//			{
//				Rel_Speed_Run(0, -0.08, 0);
//				if(Discounter_Arrive())
//				{
//					LED_Flip();
//					period_temp ++;
//				}
//			}
//			
//			if(period_temp ==3)
//			{
//				Rel_Speed_Run(0, 0, 0);
//			}
