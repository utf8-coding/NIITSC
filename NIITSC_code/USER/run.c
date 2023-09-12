#include "run.h"
#include "qrcode.h"
#include "openmv.h"
#include "flags.h"
#include "control.h"
#include "servo.h"


u8 color_Index = 0; // 车上的数量

void qr_Run(void);
void obj_Run1(void);
void rough_Run(void);
void rough_Put(void);
void rough_Get(void);
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
	//ops run
	static u8 position_ok = 0;
	if(flag_arrive && !position_ok)
	{
		flag_run = 0;
		position_ok = 1;
	}
	
	//停车	
	if(position_ok && (qr_buff[color_Index]&0x01) == (mark1 & 0x0f)){
		//抓
		servo_Action(getObj, 1);
		if(color_Index == 3){
			run_Mode = roughmode;
		}
	}
}
//粗加工区放置
void rough_Put(void){
	switch(qr_buff[3 - color_Index]){
		case red:
			//ops
			if((mark1 & 0x0f) == red){
				//if openmv 找到中心
				//停车
				servo_Action(putRough, 1);
			}
			break;
		case green:
			//ops
			if((mark1 & 0x0f) == green){
				//if openmv 找到中心
				//停车
				servo_Action(putRough, 1);
			}
			break;
		case blue:
			//ops
			if((mark1 & 0x0f) == blue){
				//if openmv 找到中心
				//停车
				servo_Action(putRough, 1);
			}
			break;
	}
}

void rough_Get(void){
	switch(qr_buff[color_Index]){
		case red:
			//ops
			if((mark1 & 0x0f) == red){
				//if openmv 找到中心
				//停车
				servo_Action(getRough, 1);
			}
			break;
		case green:
			//ops
			if((mark1 & 0x0f) == green){
				//if openmv 找到中心
				//停车
				servo_Action(getRough, 1);
			}
			break;
		case blue:
			//ops
			if((mark1 & 0x0f) == blue){
				//if openmv 找到中心
				//停车
				servo_Action(getRough, 1);
			}
			break;
	}
}
//到粗加工区�
void rough_Run(void){
	static u8 position_ok = 0;
	static u8 ops_calib_ok = 0;
	if(!position_ok && !ops_calib_ok ){
		//慢走（直到激光扫到点）
		//激光扫描
		//ops校准
		ops_calib_ok = 1;
	}
	
	
	if(!position_ok && ops_calib_ok){
		//ops run 
		if(flag_arrive){
			//停车
			flag_run = 0;
			position_ok = 1;
			ops_calib_ok = 0;
		}
	}

	
	if(position_ok){
		if ((mark1 & 0x0f) == (red & 0x01) && !ops_calib_ok){
			if(!ops_calib_ok){
				//openmv找红色中心
				//ops校准
				ops_calib_ok = 1;
			}				
		}
		
		if(ops_calib_ok){
			if(color_Index != 0){   //车上有东西
				rough_Put();
			}
			else{
				rough_Get();
			}
		}

	}
}
