#include "run.h"
#include "qrcode.h"
#include "openmv.h"
#include "flags.h"
#include "control.h"
#include "servo.h"
#include "ops.h"
#include "infrared.h"
#include "delay.h"
#include "serial.h"
#include "motor.h"


u8 color_Index = 0; // 车上的数量  
const float slow_move_speed = 0.3;

void qr_Run_Main(void);
void obj_Run_Main(void);
void rough_Run_Main(void);
void deposit_Run_Main(void);
void backObj_Run_Main(void);
void obj2_Run_Main(void);
void deposit2_Run_Main(void);
void home_Run_Main(void);

runState run_Mode = qrcodeMode;

void Run(void){
	Wheel_Run_Loop();
	switch (run_Mode) {
		case qrcodeMode:
			qr_Run_Main();
			break;
		case objMode:
			obj_Run_Main(); 
			break;
		case roughMode:
			rough_Run_Main();
			break;
		case depositMode:
			deposit_Run_Main();
			break;
		case backObjMode:
			backObj_Run_Main();
			break;
		case objMode2:
			obj2_Run_Main();
			break;
		case depositMode2:
			deposit2_Run_Main();
			break;
		case homeMode:
			home_Run_Main();
			break;
	}
}

/*-----------------------------qr--------------------------*/

u8 qr_Run_State = 1;
void qr_Run1(void)
{
	Set_Control_Mode(coordinateMode);
	Set_Target_Coordinate(-0.18, 0.6); //比QR位置靠前一点的坐标，先快速到位
	speed_limit = 50;
	qr_Run_State = 2;
}

void qr_Run2(void)
{
	//等待到位
	if(flag_arrive)
	{
		//到位慢慢往前
//		Set_Control_Mode(velocityMode);
//		Set_Speed(0, slow_move_speed, 0); //慢慢往前
		qr_Run_State = 3;
		flag_arrive = 0;
	}
}

void qr_Run3(void)
{
	
	if(QR_Ready() == qrWrong)
		Set_Control_Mode(stop); //错了停车再扫
	else if (QR_Ready() == qrRight)
	{
		Set_Control_Mode(stop);
		
		qr_Run_State = 4; //进下一阶段
	}
	else if (QR_Ready() == scanning && OPS_y > 1.8f)
	{
		Set_Control_Mode(stop);
		flag_start = 0; //走太远了，直接锁死，等手动重置
	}
}

void qr_Run_Main(void){
	switch(qr_Run_State)
	{
		case 1:
			qr_Run1();
			break;
		case 2:
			qr_Run2();
			break;
		case 3:
			qr_Run3();
			break;
		case 4:
			run_Mode = objMode;
			break;
	}
}
/*-----------------------------obj--------------------------*/

u8 obj_Run_State = 1;
void obj_Run1(void)
{
	//快走到圆盘前面一点点
	Set_Control_Mode(coordinateMode);
	Set_Target_Coordinate(-0.18, 1.45);
	speed_limit = 50;
	obj_Run_State++;
}

//void obj_Run2(void)
//{
//	if(flag_arrive)
//	{
//		Turn_Right90();
//		servoMvCalib();
//		//向左走，走到红外标定的位置，速度待定：
//		Set_Control_Mode(velocityMode);
//		Set_Speed(-slow_move_speed, 0, 0);
//		obj_Run_State++;
//	}
//}

//void obj_Run3(void)
//{
//	//等待红外检测到，假设是一号 C0，由亮色到暗色是变成0
//	if(~Infrared_Scan() & 0x04)
//	{
//		//向前走，去对齐另一个激光的边缘
//		Set_Speed(0, slow_move_speed, 0);
//		obj_Run_State++;
//	}	
//}

//void obj_Run2(void)
//{
//	//假设是二号 C1
//	if(Infrared_Scan() & 0x01){
//		Set_Control_Mode(stop); //停下来
//		obj_Run_State++;
//	}
//}

void obj_Run2(void)
{
	//假设是二号 C1
	if(flag_arrive){
		Turn_Right90();
		servoMvCalib();
		Set_Speed_All(20, 20, 20, 20);
		while(Infrared_Scan() & 0x04);
		Set_Control_Mode(stop);
		obj_Run_State++;
	}
}


void obj_Run3(void)
{
	if((qr_buff[color_Index]&0x0f) == (mark1&0x0f)){
		//抓，要做延时控制时机，MV检测到时色块还不一定稳定下来
		servo_Action(getObj); //这个函数会不会阻塞？不阻塞则也需要延时
		if(color_Index == 3){
			obj_Run_State++;
			flag_start = 0;
		}
	}
}

void obj_Run_Main(void)
{
	switch(obj_Run_State)
	{
		case 1:
			obj_Run1();
			break;
		case 2:
			obj_Run2();
			break;
		case 3:
			obj_Run3();
			break;
		case 4:
//			obj_Run4();
//			break;
//		case 5:
//			obj_Run5();
//			break;
//		case 6:
			run_Mode = roughMode;
			break;
	}
}

/*-----------------------------rough--------------------------*/


u8 rough_Run_State = 1;
//粗加工区放置
void rough_Put(void){
	static u8 buff_Index = 0;
	if( color_Index < 4 ){
		buff_Index = 3 - color_Index;
	}
	else if( color_Index > 4 && color_Index <= 7 ){
		buff_Index = 11 - color_Index;
	}

	u8 flag_running = 0, flag_on_location = 0;
	switch(qr_buff[buff_Index]){        //0->123 or 4->567
		case red:
			if(!flag_running)
			{
				Set_Control_Mode(coordinateMode);
				Set_Target_Coordinate(-0.5, 1.5);//待定
				flag_running = 1;
			}
			if (flag_running && flag_on_location )
			{
				if(flag_arrive  && flag_stable)
				{
					flag_on_location = 1;
				}
			}
			
			if(flag_on_location){
				if(Openmv_Correction_Ready())
				{
					Set_Control_Mode(stop);
					//停车
					servo_Action(putRough);
					flag_running = 0; flag_on_location = 0;
				}
			}
			break;
		case green:
			if(!flag_running)
			{
				Set_Control_Mode(coordinateMode);
				Set_Target_Coordinate(-0.65, 1.5);//待定 绿色位置
				flag_running = 1;
			}
			if (flag_running && flag_on_location )
			{
				if(flag_arrive  && flag_stable)
				{
					flag_on_location = 1;
				}
			}
			
			if(flag_on_location){
				if(Openmv_Correction_Ready())
				{
					Set_Control_Mode(stop);
					//停车
					servo_Action(putRough);
					flag_running = 0; flag_on_location = 0;
				}
			}
			break;
		case blue:
			if(!flag_running)
			{
				Set_Control_Mode(coordinateMode);
				Set_Target_Coordinate(-0.80, 1.5);//待定 蓝色位置
				flag_running = 1;
			}
			if (flag_running && flag_on_location )
			{
				if(flag_arrive  && flag_stable)
				{
					flag_on_location = 1;
				}
			}
			
			if(flag_on_location){
				if(Openmv_Correction_Ready())
				{
					Set_Control_Mode(stop);
					//停车
					servo_Action(putRough);
					flag_running = 0; flag_on_location = 0;
				}
			}
			break;
	}
}

void rough_Get(void){
	static u8 buff_Index = 0;
	if( color_Index < 4 ){
		buff_Index = 3 - color_Index;
	}
	else if( color_Index > 4 && color_Index <= 7 ){
		buff_Index = 11 - color_Index;
	}
	
	switch(qr_buff[buff_Index]){
		case red:
			//ops
			if((mark1 & 0x0f) == red){
				//if openmv 找到中心
				//停车
				servo_Action(getRough);
			}
			break;
		case green:
			//ops
			if((mark1 & 0x0f) == green){
				//if openmv 找到中心
				//停车
				servo_Action(getRough);
			}
			break;
		case blue:
			//ops
			if((mark1 & 0x0f) == blue){
				//if openmv 找到中心
				//停车
				servo_Action(getRough);
			}
			break;
	}
}

//到粗加工区
void rough_Run(void){
	static u8 position_run1 = 0, position_ok1 = 0, position_run2 = 0, position_ok2 = 0, position_run3 = 0, position_ok3 = 0, ops_calib_ok = 0;
	//static u8 ops_calib_ok = 0; //先不校准
//	if(!position_ok && !ops_calib_ok ){
//		//慢走（直到激光扫到点）
//		//if 激光扫描到角
//		OPS_Calibrate(-0.1, 1, 90); // 待定
//		ops_calib_ok = 1;
//	}
	
//	if(!position_ok && ops_calib_ok){
//		//ops run 
//		if(flag_arrive){
//			//停车
//			position_ok = 1;
//			ops_calib_ok = 0;
//		}
//	}
	
	if (!position_run1) //到转角处
	{
		Set_Control_Mode(coordinateMode);
		Set_Target_Coordinate(-0.1, 1.5);
		position_run1 = 1;
	}
	
	if(position_run1 && !position_ok1)
	{
		if (flag_arrive)
				position_ok1 = 1;
		return;
	}
	
	if (position_ok1 && !position_run2) //转向
	{
		Set_Control_Mode(coordinateMode);
		Set_Target_Heading(0);
		position_run2 = 1;
	}
	
	if(position_run2 && !position_ok2)
	{
		if (flag_arrive)
				position_ok2 = 1;
		return;
	}
	
	if(position_ok2 && !position_run3) //到红色标靶位置
	{
		Set_Control_Mode(coordinateMode);
		Set_Target_Coordinate(-0.5, 1.5);
		position_run2 = 1;
	}
	
	if(position_run3 && !position_ok3)
	{
		if (flag_arrive)
				position_ok3 = 1;
		return;
	}
	
	if(position_ok3 && !ops_calib_ok){
		Set_Control_Mode(stop);
		if ((mark1 & 0x0f) == (red & 0x01)){
			OpenMV_Change_Mode(1);
			//openmv找红色中心
			if(Openmv_Correction_Ready())
			{
				Set_Control_Mode(stop);
			}
			//ops校准
			OPS_Calibrate(-0.5, 1.5 ,0); //待定
			ops_calib_ok = 1;
		}
		
		if(ops_calib_ok){
			if(color_Index != 0 || color_Index != 4){   //车上有东西
				rough_Put();
			}
			else{
				rough_Get();
				if(color_Index == 3)
					run_Mode = depositMode;
				else if(color_Index == 7)
					run_Mode = depositMode2;
	
			}
		}

	}
}

void rough_Run_Main(void)
{
	switch(rough_Run_State)
	{
		case 1:
			rough_Run();
			break;
	}
}

/*-----------------------------deposit--------------------------*/

u8 deposit_Run_State = 1;

//粗加工到拐角 + 旋转
void deposit_Run1(void){
	//ops run 到拐角
	if(flag_arrive){
		//ops run 旋转
		deposit_Run_State++;
	}
}

//开到靠近暂存区上层并校准
void deposit_Run2(void){
	static u8 position_ok = 0;
	static u8 ops_calib_ok = 0;
	if(flag_arrive && !position_ok){
		//ops run 到红盘
		position_ok = 1;
	}
	
	if(flag_arrive && flag_stable && position_ok && !ops_calib_ok){
		//pid清零
		//ops 慢慢向前
		position_ok++;
	}
	
	
	if(flag_arrive && flag_stable && position_ok == 2){
		//openmv找红色中心（上层）
		//ops 校准
		ops_calib_ok = 1;
	}
	
	
	if(ops_calib_ok)
		deposit_Run_State++;
}

//放置
void deposit_Put(void){
	if( color_Index == 0 ){
		deposit_Run_State++;
	}
	
	else{
		switch(qr_buff[3 - color_Index]){      // 3->210
			case red:
				//ops
				if((mark1 & 0x0f) == red){
					//if openmv 找到中心
					//停车
					servo_Action(putDownDep1);
				}
				break;
			case green:
				//ops
				if((mark1 & 0x0f) == green){
					//if openmv 找到中心
					//停车
					servo_Action(putDownDep1);
				}
				break;
			case blue:
				//ops
				if((mark1 & 0x0f) == blue){
					//if openmv 找到中心
					//停车
					servo_Action(putDownDep1);
				}
				break;
		}
	}
}


void deposit_Run_Main(void)
{
	switch(deposit_Run_State)
	{
		case 1:
			deposit_Run1();
			break;
		case 2:
			deposit_Run2();
			break;
		case 3:
			deposit_Put();
			break;
		case 4:
			run_Mode = backObjMode;
			break;
	}
}

/*-----------------------------backObj--------------------------*/
u8 backObj_Run_State = 1;

void backObj_Run1(void){
	static positionDes position_Des = back;
	static u8 ops_calib_ok = 0;
	//ops run 后退
	if(flag_arrive && position_Des == back){
		position_Des = corner2_Pos;
		//ops run 回到二号拐角
	}
	
	if(flag_arrive && position_Des == corner2_Pos){
		position_Des = corner1_Pos;
		//ops run 回到一号拐角 （直走）
	}
	
	if(flag_arrive && position_Des == corner1_Pos){
		//慢走
		//校准 or 激光对其
		//if校准 ops_calib_ok = 1 
		if(ops_calib_ok)
			//ops run 回到原料区（向右走）
			position_Des = object_Pos;
	}
	
	if(flag_arrive && position_Des == object_Pos){
		deposit_Run_State++;
	}
	
}

void backObj_Run_Main(){
	switch(backObj_Run_State)
	{
		case 1:
			backObj_Run1();
			break;
		case 2:
			color_Index += 4;       //Index 4 = 车上0 
			run_Mode = objMode2;
		
//		case 2:
//			backObj_Run2();
//			break;
//		case 3:
//			backObj_Put();
//			break;
//		case 4:
//			run_Mode = backObjMode;
//			break;
	}
}

/*-----------------------------objMode2--------------------------*/

u8 obj2_Run_State = 1;


void obj2_Run1(void)
{
	if((qr_buff[color_Index]&0x0f) == (mark1&0x0f)){
		//抓，要做延时控制时机，MV检测到时色块还不一定稳定下来
		servo_Action(getObj); //这个函数会不会阻塞？不阻塞则也需要延时
		delay_ms(10000);          //
		if(color_Index == 7){
			obj2_Run_State++;
		}
	}
}

void obj2_Run_Main(){
	switch(obj2_Run_State)
	{
		case 1:
			obj2_Run1();
			break;
		case 2:
			run_Mode = roughMode;
			break;
//		case 3:
//			obj2_Run3();
//			break;
//		case 4:
//			obj2_Run4();
//			break;
//		case 5:
//			run_Mode = roughMode;
//			break;
	}
}


/*-----------------------------deposit2--------------------------*/

u8 deposit2_Run_State = 1;

//粗加工到拐角 + 旋转
void deposit2_Run1(void){
	//ops run 到拐角
	if(flag_arrive){
		//ops run 旋转
		deposit_Run_State++;
	}
}

//开到靠近暂存区上层并校准
void deposit2_Run2(void){
	static u8 position_ok = 0;
	static u8 ops_calib_ok = 0;
	if(flag_arrive && !position_ok){
		//ops run 到红盘
		position_ok = 1;
	}
	
	if(flag_arrive && flag_stable && position_ok && !ops_calib_ok){
		//pid清零
		position_ok++;
	}
	
	
	if(flag_arrive && flag_stable && position_ok == 2){
		//openmv找红色中心（下层）
		//ops 校准
		ops_calib_ok = 1;
	}
	
	
	if(ops_calib_ok)
		deposit_Run_State++;
}


void deposit2_Put(void){
	if(color_Index == 4){
		deposit2_Run_State++;
	}
	
	switch(qr_buff[11 - color_Index]){  //11-7 11-6 11-5   7->654
		case red:
			//ops
			if((mark1 & 0x0f) == red){
				//if openmv 找到中心
				//停车
				servo_Action(putDownDep2);
			}
			break;
		case green:
			//ops
			if((mark1 & 0x0f) == green){
				//if openmv 找到中心
				//停车
				servo_Action(putDownDep2);
			}
			break;
		case blue:
			//ops
			if((mark1 & 0x0f) == blue){
				//if openmv 找到中心
				//停车
				servo_Action(putDownDep2);
			}
			break;
	}
}

void deposit2_Run_Main(void)
{
	switch(deposit2_Run_State)
	{
		case 1:
			deposit2_Run1();
			break;
		case 2:
			deposit2_Run2();
			break;
		case 3:
			deposit2_Put();
			break;
		case 4:
			run_Mode = homeMode;
			break;
	}
}


/*-----------------------------homeMode--------------------------*/

u8 home_Run_State = 1;

void home_Run1(void){
	static positionDes position_Des = corner3_Pos;
	static u8 ops_calib_ok = 0;
	//ops run 到三号拐角 
	if(flag_arrive && position_Des == corner3_Pos){
		//ops run 慢慢左移
//		if(Infrared_Scan()){       //
			//停车
			position_Des = back;
//		}
		
	}
	
	if(flag_arrive && position_Des == back){
		//ops run 回到起停区
		position_Des = home_Pos;
	}
	
	if(flag_arrive && flag_stable && position_Des == home_Pos){
		home_Run_State++;           
	}
	
	
}

void home_Run_Main(void)
{
	switch(home_Run_State)
	{
		case 1:
			home_Run1();
			break;
		case 2:
			//流程结束 
			break;
//		case 3:
//			run_Mode = homeMode;
//			break;
//		case 4:
//			run_Mode = homeMode;
//			break;
	}
}



