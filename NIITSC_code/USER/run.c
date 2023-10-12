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
#include "encoder.h"
#include "led.h"
#include "oled.h"


u8 color_Index = 0; // ���ϵ�����  
const float slow_move_speed = 0.3;

void qr_Run_Main(void);
void obj_Run_Main(void);
void rough_Run_Main(void);
void deposit_Run_Main(void);
void backObj_Run_Main(void);
void obj2_Run_Main(void);
void deposit2_Run_Main(void);
void home_Run_Main(void);
void stop_running(void);
u8 get_Itr(void);

runState run_Mode = qrcodeMode;

//--------------- TEST --------------//
int test1(void)
{
	Set_Discounter(20);
	return 1;
}

int test2(void)
{
	Abs_Speed_Run(0, 0.10, 0);
	if(Discounter_Arrive()) return 1;
	return 0;
}

int test3(void)
{
	return 1;
}


//------------ QR Code ------------//
int qr1_1(void)
{
	Target_Run(-0.18, 0.6, 0);
	speed_limit = 0.08;
	if (flag_arrive)return 1;
	return 0;
}

int qr1_2(void)
{
	Abs_Speed_Run(0, 0.05, 0);
	if(QR_Ready() == qrWrong)
	{
		Stop_Run(); //����ͣ����ɨ
	}
	else if (QR_Ready() == scanning && OPS_y > 1.8f)
	{
		Stop_Run();
		stop_running(); //��̫Զ�ˣ�ֱ�����������ֶ�����
	}
	else if (QR_Ready() == qrRight)
	{
		return 1;
	}
	return 0;
}

//------------ Get Object ------------//
int obj1_1(void)
{
	Target_Run(-0.20, 1.15, 0);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int obj1_2(void)
{
	Abs_Speed_Run(0.08, 0, 0); // Slow to the right
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x01) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}

int obj1_3(void)
{
	Abs_Speed_Run(0, 0.05, 0); // Slow ahead
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x08) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}

int obj1_4(void)
{
	servoMove(calibObj, 1500);
	
	return 1;
}

//----------- Put Obj in temp area 1-----------//
int obj2_1(void)
{
	Target_Run(-0.15, 1.80, -90);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int obj2_2(void)
{
	Target_Run(-0.70, 1.87, -90);
	speed_limit = 0.08;
	if (flag_arrive && flag_stable) return 1;
	return 0;
}

int obj2_3(void)
{
	Abs_Speed_Run(0, 0.05, 0); // Slow to its right
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x01) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}


int obj2_4(void)
{
	Abs_Speed_Run(-0.045, 0, 0); // Slow to its ahead
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x02) temp_itr++;
		else break;
		
		if(temp_itr >= 12)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}
//put obj
int obj2_5(void)
{
	return 1;
}

//----------- Put Obj in final area 1-----------//
int obj3_1(void)
{
	Target_Run(-1.70, 1.87, -180);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int obj3_2(void)
{
	Target_Run(-1.70, 1.50, -180);
	speed_limit = 0.08;
	if (flag_arrive) return 1;
	return 0;
}

int obj3_3(void)
{
	Abs_Speed_Run(-0.05, 0, 0); // Slow to its right
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x01) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			OLED_Clear();
			OLED_ShowNum(1, 1, (u32)get_Itr, 1);
			return 1;
		}
	}
	return 0;
}

int obj3_4(void)
{
	Abs_Speed_Run(0, -0.045, 0); // Slow to its ahead
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x03) temp_itr++;
		else break;
		
		if(temp_itr >= 12)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}

//put obj
int obj3_5(void)
{
	return 1;
}

//----------- Return to get obj -----------//
int return1_1(void)
{
	Target_Run(-1.70, 1.87, -180);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int return1_2(void)
{
	Target_Run(-0.15, 1.80, 0);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int return1_3(void)
{
	Target_Run(-0.15, 1.80, 0);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int return1_4(void)
{
	Target_Run(-0.20, 1.15, 0);
	speed_limit = 0.08;
	if (flag_vague_arrive) return 1;
	return 0;
}

int return1_5(void)
{
	Abs_Speed_Run(0.08, 0, 0); // Slow to the right
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x01) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}

int return1_6(void)
{
	Abs_Speed_Run(0, 0.05, 0); // Slow ahead
	
	u8 temp_itr = 0;
	while(1)
	{
		if(Infrared_Scan() & 0x08) temp_itr++;
		else break;
		
		if(temp_itr >= 8)
		{
			Stop_Run();
			return 1;
		}
	}
	return 0;
}

int return1_7(void)
{
	delay_ms(2000);
	//Grab thingy
	return 1;
}

//EEENNNDDD

int end_return_home(void)
{
	Target_Run(0, 0, 0);
	speed_limit = 0.08;
	if (flag_arrive && flag_stable) return 1;
	return 0;
}

//----------- Run Excutor -----------//
 int(*operation_sequence[])(void) = {qr1_1, qr1_2, obj1_1, obj1_2, 
																		obj1_3, obj1_4, obj2_1, obj2_2, obj2_3, obj2_4, obj2_5,
																		obj3_1, obj3_2, obj3_3, obj3_4, obj3_5,
																		end_return_home};
 u8 max_run_itr = 16;

//int(*operation_sequence[])(void) = {test1, test2, test3};
//u8 max_run_itr = 3;

u8 itr = 0;
	
void Run(void){
	if(operation_sequence[itr]()) itr++;
	if (itr >= max_run_itr) 
	{
		itr = 0;
		flag_start = 0;
	}
}

void stop_running(void)
{
	flag_start = 0;
	itr = 0;
}

u8 get_Itr(void)
{
	return itr;
}

/*-----------------------------obj--------------------------*/

//u8 obj_Run_State = 1;
//void obj_Run1(void)
//{
//	//���ߵ�Բ��ǰ��һ���
//	Set_Control_Mode(coordinateMode);
//	Set_Target_Coordinate(-0.18, 1.45);
//	speed_limit = 50;
//	obj_Run_State++;
//}

////void obj_Run2(void)
////{
////	if(flag_arrive)
////	{
////		Turn_Right90();
////		servoMvCalib();
////		//�����ߣ��ߵ�����궨��λ�ã��ٶȴ�����
////		Set_Control_Mode(velocityMode);
////		Set_Speed(-slow_move_speed, 0, 0);
////		obj_Run_State++;
////	}
////}

////void obj_Run3(void)
////{
////	//�ȴ������⵽��������һ�� C0������ɫ����ɫ�Ǳ��0
////	if(~Infrared_Scan() & 0x04)
////	{
////		//��ǰ�ߣ�ȥ������һ������ı�Ե
////		Set_Speed(0, slow_move_speed, 0);
////		obj_Run_State++;
////	}	
////}

////void obj_Run2(void)
////{
////	//�����Ƕ��� C1
////	if(Infrared_Scan() & 0x01){
////		Set_Control_Mode(stop); //ͣ����
////		obj_Run_State++;
////	}
////}

//void obj_Run2(void)
//{
//	//�����Ƕ��� C1
//	if(flag_arrive){
//		Turn_Right90();
//		servoMvCalib();
//		Set_Speed_All(20, 20, 20, 20);
//		while(Infrared_Scan() & 0x04);
//		Set_Control_Mode(stop);
//		obj_Run_State++;
//	}
//}


//void obj_Run3(void)
//{
//	if((qr_buff[color_Index]&0x0f) == (mark1&0x0f)){
//		//ץ��Ҫ����ʱ����ʱ����MV��⵽ʱɫ�黹��һ���ȶ�����
//		servo_Action(getObj); //��������᲻����������������Ҳ��Ҫ��ʱ
//		if(color_Index == 3){
//			obj_Run_State++;
//			flag_start = 0;
//		}
//	}
//}

//void obj_Run_Main(void)
//{
//	switch(obj_Run_State)
//	{
//		case 1:
//			obj_Run1();
//			break;
//		case 2:
//			obj_Run2();
//			break;
//		case 3:
//			obj_Run3();
//			break;
//		case 4:
////			obj_Run4();
////			break;
////		case 5:
////			obj_Run5();
////			break;
////		case 6:
//			run_Mode = roughMode;
//			break;
//	}
//}

///*-----------------------------rough--------------------------*/


//u8 rough_Run_State = 1;
////�ּӹ�������
//void rough_Put(void){
//	static u8 buff_Index = 0;
//	if( color_Index < 4 ){
//		buff_Index = 3 - color_Index;
//	}
//	else if( color_Index > 4 && color_Index <= 7 ){
//		buff_Index = 11 - color_Index;
//	}

//	u8 flag_running = 0, flag_on_location = 0;
//	switch(qr_buff[buff_Index]){        //0->123 or 4->567
//		case red:
//			if(!flag_running)
//			{
//				Set_Control_Mode(coordinateMode);
//				Set_Target_Coordinate(-0.5, 1.5);//����
//				flag_running = 1;
//			}
//			if (flag_running && flag_on_location )
//			{
//				if(flag_arrive  && flag_stable)
//				{
//					flag_on_location = 1;
//				}
//			}
//			
//			if(flag_on_location){
//				if(Openmv_Correction_Ready())
//				{
//					Set_Control_Mode(stop);
//					//ͣ��
//					servo_Action(putRough);
//					flag_running = 0; flag_on_location = 0;
//				}
//			}
//			break;
//		case green:
//			if(!flag_running)
//			{
//				Set_Control_Mode(coordinateMode);
//				Set_Target_Coordinate(-0.65, 1.5);//���� ��ɫλ��
//				flag_running = 1;
//			}
//			if (flag_running && flag_on_location )
//			{
//				if(flag_arrive  && flag_stable)
//				{
//					flag_on_location = 1;
//				}
//			}
//			
//			if(flag_on_location){
//				if(Openmv_Correction_Ready())
//				{
//					Set_Control_Mode(stop);
//					//ͣ��
//					servo_Action(putRough);
//					flag_running = 0; flag_on_location = 0;
//				}
//			}
//			break;
//		case blue:
//			if(!flag_running)
//			{
//				Set_Control_Mode(coordinateMode);
//				Set_Target_Coordinate(-0.80, 1.5);//���� ��ɫλ��
//				flag_running = 1;
//			}
//			if (flag_running && flag_on_location )
//			{
//				if(flag_arrive  && flag_stable)
//				{
//					flag_on_location = 1;
//				}
//			}
//			
//			if(flag_on_location){
//				if(Openmv_Correction_Ready())
//				{
//					Set_Control_Mode(stop);
//					//ͣ��
//					servo_Action(putRough);
//					flag_running = 0; flag_on_location = 0;
//				}
//			}
//			break;
//	}
//}

//void rough_Get(void){
//	static u8 buff_Index = 0;
//	if( color_Index < 4 ){
//		buff_Index = 3 - color_Index;
//	}
//	else if( color_Index > 4 && color_Index <= 7 ){
//		buff_Index = 11 - color_Index;
//	}
//	
//	switch(qr_buff[buff_Index]){
//		case red:
//			//ops
//			if((mark1 & 0x0f) == red){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(getRough);
//			}
//			break;
//		case green:
//			//ops
//			if((mark1 & 0x0f) == green){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(getRough);
//			}
//			break;
//		case blue:
//			//ops
//			if((mark1 & 0x0f) == blue){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(getRough);
//			}
//			break;
//	}
//}

////���ּӹ���
//void rough_Run(void){
//	static u8 position_run1 = 0, position_ok1 = 0, position_run2 = 0, position_ok2 = 0, position_run3 = 0, position_ok3 = 0, ops_calib_ok = 0;
//	//static u8 ops_calib_ok = 0; //�Ȳ�У׼
////	if(!position_ok && !ops_calib_ok ){
////		//���ߣ�ֱ������ɨ���㣩
////		//if ����ɨ�赽��
////		OPS_Calibrate(-0.1, 1, 90); // ����
////		ops_calib_ok = 1;
////	}
//	
////	if(!position_ok && ops_calib_ok){
////		//ops run 
////		if(flag_arrive){
////			//ͣ��
////			position_ok = 1;
////			ops_calib_ok = 0;
////		}
////	}
//	
//	if (!position_run1) //��ת�Ǵ�
//	{
//		Set_Control_Mode(coordinateMode);
//		Set_Target_Coordinate(-0.1, 1.5);
//		position_run1 = 1;
//	}
//	
//	if(position_run1 && !position_ok1)
//	{
//		if (flag_arrive)
//				position_ok1 = 1;
//		return;
//	}
//	
//	if (position_ok1 && !position_run2) //ת��
//	{
//		Set_Control_Mode(coordinateMode);
//		Set_Target_Heading(0);
//		position_run2 = 1;
//	}
//	
//	if(position_run2 && !position_ok2)
//	{
//		if (flag_arrive)
//				position_ok2 = 1;
//		return;
//	}
//	
//	if(position_ok2 && !position_run3) //����ɫ���λ��
//	{
//		Set_Control_Mode(coordinateMode);
//		Set_Target_Coordinate(-0.5, 1.5);
//		position_run2 = 1;
//	}
//	
//	if(position_run3 && !position_ok3)
//	{
//		if (flag_arrive)
//				position_ok3 = 1;
//		return;
//	}
//	
//	if(position_ok3 && !ops_calib_ok){
//		Set_Control_Mode(stop);
//		if ((mark1 & 0x0f) == (red & 0x01)){
//			OpenMV_Change_Mode(1);
//			//openmv�Һ�ɫ����
//			if(Openmv_Correction_Ready())
//			{
//				Set_Control_Mode(stop);
//			}
//			//opsУ׼
//			OPS_Calibrate(-0.5, 1.5 ,0); //����
//			ops_calib_ok = 1;
//		}
//		
//		if(ops_calib_ok){
//			if(color_Index != 0 || color_Index != 4){   //�����ж���
//				rough_Put();
//			}
//			else{
//				rough_Get();
//				if(color_Index == 3)
//					run_Mode = depositMode;
//				else if(color_Index == 7)
//					run_Mode = depositMode2;
//	
//			}
//		}

//	}
//}

//void rough_Run_Main(void)
//{
//	switch(rough_Run_State)
//	{
//		case 1:
//			rough_Run();
//			break;
//	}
//}

///*-----------------------------deposit--------------------------*/

//u8 deposit_Run_State = 1;

////�ּӹ����ս� + ��ת
//void deposit_Run1(void){
//	//ops run ���ս�
//	if(flag_arrive){
//		//ops run ��ת
//		deposit_Run_State++;
//	}
//}

////���������ݴ����ϲ㲢У׼
//void deposit_Run2(void){
//	static u8 position_ok = 0;
//	static u8 ops_calib_ok = 0;
//	if(flag_arrive && !position_ok){
//		//ops run ������
//		position_ok = 1;
//	}
//	
//	if(flag_arrive && flag_stable && position_ok && !ops_calib_ok){
//		//pid����
//		//ops ������ǰ
//		position_ok++;
//	}
//	
//	
//	if(flag_arrive && flag_stable && position_ok == 2){
//		//openmv�Һ�ɫ���ģ��ϲ㣩
//		//ops У׼
//		ops_calib_ok = 1;
//	}
//	
//	
//	if(ops_calib_ok)
//		deposit_Run_State++;
//}

////����
//void deposit_Put(void){
//	if( color_Index == 0 ){
//		deposit_Run_State++;
//	}
//	
//	else{
//		switch(qr_buff[3 - color_Index]){      // 3->210
//			case red:
//				//ops
//				if((mark1 & 0x0f) == red){
//					//if openmv �ҵ�����
//					//ͣ��
//					servo_Action(putDownDep1);
//				}
//				break;
//			case green:
//				//ops
//				if((mark1 & 0x0f) == green){
//					//if openmv �ҵ�����
//					//ͣ��
//					servo_Action(putDownDep1);
//				}
//				break;
//			case blue:
//				//ops
//				if((mark1 & 0x0f) == blue){
//					//if openmv �ҵ�����
//					//ͣ��
//					servo_Action(putDownDep1);
//				}
//				break;
//		}
//	}
//}


//void deposit_Run_Main(void)
//{
//	switch(deposit_Run_State)
//	{
//		case 1:
//			deposit_Run1();
//			break;
//		case 2:
//			deposit_Run2();
//			break;
//		case 3:
//			deposit_Put();
//			break;
//		case 4:
//			run_Mode = backObjMode;
//			break;
//	}
//}

///*-----------------------------backObj--------------------------*/
//u8 backObj_Run_State = 1;

//void backObj_Run1(void){
//	static positionDes position_Des = back;
//	static u8 ops_calib_ok = 0;
//	//ops run ����
//	if(flag_arrive && position_Des == back){
//		position_Des = corner2_Pos;
//		//ops run �ص����Źս�
//	}
//	
//	if(flag_arrive && position_Des == corner2_Pos){
//		position_Des = corner1_Pos;
//		//ops run �ص�һ�Źս� ��ֱ�ߣ�
//	}
//	
//	if(flag_arrive && position_Des == corner1_Pos){
//		//����
//		//У׼ or �������
//		//ifУ׼ ops_calib_ok = 1 
//		if(ops_calib_ok)
//			//ops run �ص�ԭ�����������ߣ�
//			position_Des = object_Pos;
//	}
//	
//	if(flag_arrive && position_Des == object_Pos){
//		deposit_Run_State++;
//	}
//	
//}

//void backObj_Run_Main(){
//	switch(backObj_Run_State)
//	{
//		case 1:
//			backObj_Run1();
//			break;
//		case 2:
//			color_Index += 4;       //Index 4 = ����0 
//			run_Mode = objMode2;
//		
////		case 2:
////			backObj_Run2();
////			break;
////		case 3:
////			backObj_Put();
////			break;
////		case 4:
////			run_Mode = backObjMode;
////			break;
//	}
//}

///*-----------------------------objMode2--------------------------*/

//u8 obj2_Run_State = 1;


//void obj2_Run1(void)
//{
//	if((qr_buff[color_Index]&0x0f) == (mark1&0x0f)){
//		//ץ��Ҫ����ʱ����ʱ����MV��⵽ʱɫ�黹��һ���ȶ�����
//		servo_Action(getObj); //��������᲻����������������Ҳ��Ҫ��ʱ
//		delay_ms(10000);          //
//		if(color_Index == 7){
//			obj2_Run_State++;
//		}
//	}
//}

//void obj2_Run_Main(){
//	switch(obj2_Run_State)
//	{
//		case 1:
//			obj2_Run1();
//			break;
//		case 2:
//			run_Mode = roughMode;
//			break;
////		case 3:
////			obj2_Run3();
////			break;
////		case 4:
////			obj2_Run4();
////			break;
////		case 5:
////			run_Mode = roughMode;
////			break;
//	}
//}


///*-----------------------------deposit2--------------------------*/

//u8 deposit2_Run_State = 1;

////�ּӹ����ս� + ��ת
//void deposit2_Run1(void){
//	//ops run ���ս�
//	if(flag_arrive){
//		//ops run ��ת
//		deposit_Run_State++;
//	}
//}

////���������ݴ����ϲ㲢У׼
//void deposit2_Run2(void){
//	static u8 position_ok = 0;
//	static u8 ops_calib_ok = 0;
//	if(flag_arrive && !position_ok){
//		//ops run ������
//		position_ok = 1;
//	}
//	
//	if(flag_arrive && flag_stable && position_ok && !ops_calib_ok){
//		//pid����
//		position_ok++;
//	}
//	
//	
//	if(flag_arrive && flag_stable && position_ok == 2){
//		//openmv�Һ�ɫ���ģ��²㣩
//		//ops У׼
//		ops_calib_ok = 1;
//	}
//	
//	
//	if(ops_calib_ok)
//		deposit_Run_State++;
//}


//void deposit2_Put(void){
//	if(color_Index == 4){
//		deposit2_Run_State++;
//	}
//	
//	switch(qr_buff[11 - color_Index]){  //11-7 11-6 11-5   7->654
//		case red:
//			//ops
//			if((mark1 & 0x0f) == red){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(putDownDep2);
//			}
//			break;
//		case green:
//			//ops
//			if((mark1 & 0x0f) == green){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(putDownDep2);
//			}
//			break;
//		case blue:
//			//ops
//			if((mark1 & 0x0f) == blue){
//				//if openmv �ҵ�����
//				//ͣ��
//				servo_Action(putDownDep2);
//			}
//			break;
//	}
//}

//void deposit2_Run_Main(void)
//{
//	switch(deposit2_Run_State)
//	{
//		case 1:
//			deposit2_Run1();
//			break;
//		case 2:
//			deposit2_Run2();
//			break;
//		case 3:
//			deposit2_Put();
//			break;
//		case 4:
//			run_Mode = homeMode;
//			break;
//	}
//}


///*-----------------------------homeMode--------------------------*/

//u8 home_Run_State = 1;

//void home_Run1(void){
//	static positionDes position_Des = corner3_Pos;
//	static u8 ops_calib_ok = 0;
//	//ops run �����Źս� 
//	if(flag_arrive && position_Des == corner3_Pos){
//		//ops run ��������
////		if(Infrared_Scan()){       //
//			//ͣ��
//			position_Des = back;
////		}
//		
//	}
//	
//	if(flag_arrive && position_Des == back){
//		//ops run �ص���ͣ��
//		position_Des = home_Pos;
//	}
//	
//	if(flag_arrive && flag_stable && position_Des == home_Pos){
//		home_Run_State++;           
//	}
//	
//	
//}

//void home_Run_Main(void)
//{
//	switch(home_Run_State)
//	{
//		case 1:
//			home_Run1();
//			break;
//		case 2:
//			//���̽��� 
//			break;
////		case 3:
////			run_Mode = homeMode;
////			break;
////		case 4:
////			run_Mode = homeMode;
////			break;
//	}
//}



