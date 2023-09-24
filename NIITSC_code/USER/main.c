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
	Encoder_Init_All();

//	OPENMV_USART_Config();
	QRCODE_UART_Config();
	SCREEN_USART_Config();
	SERVO_USART_Config();
	Moto_Init(18 - 1, 8400 -1);
	Laser_Init();
	//BUZZER_Init();
	
//	OLED_ShowChar(1, 1, 'A');
//	OLED_ShowString(1, 3, "HelloWorld!");
//	OLED_ShowNum(2, 1, 12345, 5);
//	OLED_ShowSignedNum(2, 7, -66, 2);
//	OLED_ShowHexNum(3, 1, 0xAA55, 4);
//	OLED_ShowBinNum(4, 1, 0xAA55, 16);

	servoDefault();
	//BUZZER_Ms();
	delay_ms(1000);
	
	unsigned char starter[9] = "CLR(0);";
	starter[7] = 0x0d;
	starter[8] = 0x0a;
	Serial_SendArray(USART6, &starter[0], 9);
	delay_ms(100);
	
	unsigned char starter2[9] = "DIR(1);";
	starter2[7] = 0x0d;
	starter2[8] = 0x0a;
	Serial_SendArray(USART6, &starter2[0], 9);
	delay_ms(100);
	
	while (1)
	{
		OpenMV_Data_Process();
		OPS_Data_Process();
		if(!flag_ops_ready){
			OLED_ShowString(1, 1, "OPS prep.");
			continue;
		}
		Input_Disp_Loop();
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
