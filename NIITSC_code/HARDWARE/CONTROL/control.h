#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
控制函数定义；参数定义
**************************************************************************/

typedef struct
{
	u8 enable_lim_sum_error	: 1;
	u8 enable_lim_ouput		: 1;
	
	float kp;
	float ki;
	float kd;
	
//	float sum_error_lim;
	float kiout_lim;
	float lim_output;

	float sum_error;
	float last_error;
	float last_last_error;
	
	float kd_output;
	float ki_output;
	float kp_output;

	float output;
	float error;
}  PID;


extern u8 speed_limit, angular_speed_limit;
extern float target_x, target_y, target_heading;

void PID_Init(void);
void Wheel_Run_Loop(void);
void Set_Target_Coordinate(float x,float y ,float heading);
void Wheel_Speed_Calc(float vx, float vy, float vw);
void Go_Direction_Test(float vx,float vy,float vw);
void Go_Position_Test(float x, float y, float heading);
void Control_Display_Specs(void);

#endif
