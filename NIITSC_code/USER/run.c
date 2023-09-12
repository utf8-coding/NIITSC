#include "run.h"
#include "qrcode.h"
#include "openmv.h"
#include "flags.h"
#include "control.h"

void qr_Run(void);
void obj_Run1(void);

/* 
	0 qrcode
	1 material
*/

runState run_Mode = qrcode;
u8 mode_change_sign = 1;

void Run(void){
	Wheel_Run_Loop();
	switch (run_Mode) {
		case qrcode:
			qr_Run();
			break;
		case objmode:
			obj_Run1();
			break;
	}
}

void qr_Init()
{
	
}

void qr_Run(void){
	//ops run
	if(mode_change_sign)
	{
		flag_run = 1;
		Set_Target_Coordinate(-0.1, 0.6, 0);
		mode_change_sign = 0;
	}
	
	if(flag_arrive)
	{
		Set_Target_Coordinate(-0.1, 0.7, 0);
		speed_limit = 30;
		while(!QR_Ready()){
			if(flag_stable) //End of the scanning, if failed, should stop.
				break;
		}
		
		//下一轮初始化：
		if(!OpenMV_Change_Mode(4))
		{
			flag_start = 0;
		}
		flag_run = 1;
		Set_Target_Coordinate(-0.1, 0.8, 0);
		mode_change_sign = 0;
		run_Mode = objmode;
	}
}

void obj_Run1(void){
	u8 color_Index = 0;
	//ops run
	static u8 position_ok = 0;
	if(flag_arrive && !position_ok)
	{
		flag_run = 0;
		position_ok = 1;
	}
	
	//停车	
	if(position_ok && (qr_buff[color_Index]&0x0f) == (mark1 & 0x0f)){
		//抓
		color_Index++;
		if(color_Index == 3){
			run_Mode = objmode;
		}
	}
}
