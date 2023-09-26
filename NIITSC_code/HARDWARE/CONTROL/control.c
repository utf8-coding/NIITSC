#include "control.h"
#include "motor.h"
#include "encoder.h"
#include "sys.h"
#include "stdlib.h"
#include "ops.h"
#include "delay.h"
#include "math.h"
#include "flags.h"
#include "stdio.h"
#include "led.h"
#include "oled.h"
#include "openmv.h"
#include "encoder.h"

//Public:
u8 speed_limit = 80, angular_speed_limit = 20;

//Internal:
const float PI = 3.14159265, H = 0.188, W = 0.25, Wheel_C = 0.035*2*PI;
const float xy_kp     	= 0.80,  				xy_ki = 0.05,   	 	 xy_kd = 0,  		  xy_ki_limit = 0.1, 
						heading_kp  = 0.07,   heading_ki  = 0.001,  heading_kd = 0,  heading_ki_limit = 0.1,
						wheel_kp 		= 0.5, 			wheel_ki 	= 0.01,   	wheel_kd = 0, 	 wheel_ki_limit = 30,
						mv_kp   	  = 0.008, 				mv_ki = 0.0005, 		 mv_kd = 0,  		  mv_ki_limit = 0.1;

int ideal_heading = 0, ideal_ring = 0;

float target_x = 0, target_y = 0, target_heading = 0;
float MotorA_Target_Speed = 0, MotorB_Target_Speed = 0, MotorC_Target_Speed = 0, MotorD_Target_Speed = 0;
float vx = 0, vy = 0, w = 0;

ControlMode control_mode = coordinateMode;

float motorA_speed = 0, motorB_speed = 0, motorC_speed = 0, motorD_speed = 0;

const float openmv_correction_threshold = 5;
PID x_pid, y_pid, heading_pid, openmv_x_PID, openmv_y_PID, motorA_PID, motorB_PID, motorC_PID, motorD_PID;

//Internal functions:
void set_pid(PID* pid,float kp,float ki,float kd,float kiout_lim,float lim_output)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->kiout_lim=kiout_lim;
	pid->lim_output=lim_output;
}

void reset_pid(PID* pid)
{
	pid->sum_error = 0;
	pid->last_error = 0;
	pid->last_last_error = 0;
	pid->output = 0;
}

void check_stable(void)
{
	static float prev_x,prev_y, prev_heading;
	if(fabs(prev_x - OPS_x) < 0.01 && fabs(prev_y - OPS_y) < 0.01 && fabs(OPS_heading - prev_heading) < 1)
		flag_stable = 1;
	else
		flag_stable = 0;
	
	if(fabs(x_pid.error) < 0.025 && fabs(y_pid.error) < 0.025 && fabs(heading_pid.error) < 1.5)
		flag_arrive = 1;
	else
		flag_arrive = 0;
	prev_x = OPS_x; prev_y = OPS_y; prev_heading = OPS_heading;
}

float pid_calculate(PID* pid, float input, float measure)
{
  pid->error = input - measure;
	pid->sum_error += pid->error;
	
	pid->kp_output = pid->kp * pid->error;
	pid->ki_output = pid->ki * pid->sum_error;
	pid->kd_output = pid->kd * (pid->error - pid->last_error);
	
	if(fabs(pid->ki_output)> pid->kiout_lim)
  {
    if(pid->ki_output > 0)
		{
      pid->ki_output = pid->kiout_lim;
			pid->sum_error=pid->kiout_lim / pid->ki;
		}
    else
		{
      pid->ki_output = -pid->kiout_lim;
			pid->sum_error=-pid->kiout_lim / pid->ki;
		}
	}
	
  pid->output = pid->kp_output + pid->ki_output + pid->kd_output ;
	
  if(fabs(pid->output) > pid->lim_output)
  {
		if(pid->output >= 0)
			pid->output = pid->lim_output;
		else
			pid->output = -pid->lim_output;
  }
	pid->last_error = pid->error;
	return 0;
}

void wheel_speed_calc(float vx, float vy, float vw) //m/s
{
	int temp[4];
	temp[0] = (vy + vx + vw * (H + W)); 
	temp[1] = (vy - vx - vw * (H + W));
	temp[2] = (vy - vx + vw * (H + W)); 
	temp[3] = (vy + vx - vw * (H + W));
	
	//Unify if too big:(problematic)
	int max_speed = 0;
	for(int itr = 0; itr < 4; itr++)
	{
		if (abs(max_speed) < abs(temp[itr]))
			max_speed = temp[itr];
	}
	
	if (abs(max_speed) > speed_limit)
	{
		motorA_speed=temp[0]*speed_limit/abs(max_speed);
		motorB_speed=temp[1]*speed_limit/abs(max_speed);
		motorC_speed=temp[2]*speed_limit/abs(max_speed);
		motorD_speed=temp[3]*speed_limit/abs(max_speed);
	} else 
	{
		motorA_speed=temp[0];
		motorB_speed=temp[1];
		motorC_speed=temp[2];
		motorD_speed=temp[3];
	}
}

void coordinate_Loop(void)
{
	pid_calculate(&x_pid, target_x, OPS_x);
	pid_calculate(&y_pid, target_y, OPS_y);
	pid_calculate(&heading_pid, target_heading, OPS_heading+OPS_ring*360);
	
	vx = -(y_pid.output)*sin(OPS_heading/PI)+(x_pid.output)*cos(OPS_heading/PI);
	vy = (y_pid.output)*cos(OPS_heading/PI)+(x_pid.output)*sin(OPS_heading/PI);
	w = heading_pid.output;
	
	wheel_speed_calc(vx,vy,w);
	
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
	check_stable();
}

//Public functions:
void Control_Init(void)
{
	set_pid(&x_pid, xy_kp, xy_ki, xy_kd, xy_ki_limit, speed_limit);
	set_pid(&y_pid, xy_kp, xy_ki, xy_kd, xy_ki_limit, speed_limit);
	set_pid(&heading_pid, heading_kp, heading_ki, heading_kd, heading_ki_limit, speed_limit);
	
	set_pid(&openmv_x_PID, mv_kp, mv_ki, mv_kd, mv_ki_limit, speed_limit); //speed_limit is problematic here, needs fine tune
	set_pid(&openmv_y_PID, mv_kp, mv_ki, mv_kd, mv_ki_limit, speed_limit);
}

void Set_Control_Mode(ControlMode mode)
{
	control_mode = mode;
	if(mode == coordinateMode)
	{
		reset_pid(&x_pid);
		reset_pid(&y_pid);
		reset_pid(&heading_pid);
	}
		
	target_x = OPS_x; target_x = OPS_y; target_heading = OPS_heading;
	vx = 0; vy = 0; w = 0;
}	

void Set_Target_Coordinate(float x, float y)
{
	target_x = x; target_y = y;
}

void Set_Target_Heading(float heading)
{
	target_heading = heading;
}

void Turn_Right90(void)
{
	ideal_heading += 90;
	float turn_heading = ideal_ring*360 + ideal_heading;
	
	wheel_speed_calc(0, 0, 0.5);
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
	while(OPS_heading + OPS_ring*360 <= turn_heading-1);
	
	if (ideal_heading >= 180)
	{
		ideal_heading -= 360;
		ideal_ring += 1;
	}
	target_heading = ideal_heading;
	
	Set_Speed_All(0, 0, 0, 0);
}

void Turn_Left90(void)
{
	ideal_heading -= 90;
	float turn_heading = ideal_ring*360 + ideal_heading;
	
	wheel_speed_calc(0, 0, -0.5);
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
	while(OPS_heading + OPS_ring*360 >= turn_heading+1);
	
	if (ideal_heading <= -180)
	{
		ideal_heading += 360;
		ideal_ring -= 1;
	}
	target_heading = ideal_heading;
	
	Set_Speed_All(0, 0, 0, 0);
}

void Set_Speed_All(float speedA, float speedB, float speedC, float speedD)
{
	MotorA_Target_Speed = speedA;
	MotorB_Target_Speed = speedB;
	MotorC_Target_Speed = speedC;
	MotorD_Target_Speed = speedD;
}
void Set_Abs_Speed(float x,float y,float vw) //相对坐标系方向
{
	vx = x; vy = y; w = vw;
}

void Wheel_Run_Loop(void)
{
	if (control_mode == stop)
	{
		Set_Speed_All(0, 0, 0, 0);
		return;
	}
	else if(control_mode == coordinateMode)
	{
		coordinate_Loop();
		wheel_speed_calc(vx, vy, w);
		Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	}
	else if(control_mode == velocityMode)
	{
		wheel_speed_calc(vx, vy, w);
		Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	}
}


void Go_Position_Test(float x, float y, float heading)
{
	Set_Target_Coordinate(x, y);
	Set_Target_Heading(heading);
	Wheel_Run_Loop();
	if (flag_arrive)
	{
		flag_start = 0;
	}
}

void Openmv_Correction(void)
{
	pid_calculate(&openmv_x_PID, 0, mv_target_cx);
	pid_calculate(&openmv_y_PID, 0, mv_target_cy);
	//Set_Speed(openmv_x_PID.output, openmv_y_PID.output, 0);
}

u8 Openmv_Correction_Ready(void)
{
	if(openmv_x_PID.error < openmv_correction_threshold && openmv_y_PID.error < openmv_correction_threshold)
		return 1;
	else 
		return 0;
}
void Control_Display_Specs(void)
{
	OLED_ShowString(1, 1, "Ctrl:");
	OLED_ShowSignedNum(2, 1, (int)(vx*100), 4);
	OLED_ShowSignedNum(2, 7, (int)(vy*100), 4);
	OLED_ShowSignedNum(3, 1, (int)(w*100), 4);
	
	OLED_ShowSignedNum(4, 1, (int) x_pid.error * 1000, 3);
	OLED_ShowSignedNum(4, 5, (int) y_pid.error * 1000, 3);
}
