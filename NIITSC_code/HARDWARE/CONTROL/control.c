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

//==================== Public vars =====================:
float speed_limit = 0.5, angular_speed_limit = 0.1;

//==================== Internal vars =====================:
//constants & PIDs
PID x_pid, y_pid, heading_pid, openmv_x_PID, openmv_y_PID, motorA_PID, motorB_PID, motorC_PID, motorD_PID;
const float H = 0.188, W = 0.25;
const float xy_kp = 0.80,	 	xy_ki = 0.05, 			xy_kd = 0, 			xy_ki_limit = 0.1,
			heading_kp = 0.07, 	heading_ki = 0.001, 	heading_kd = 0, 	heading_ki_limit = 0.1,
			motor_kp = 0.5, 	motor_ki = 0.01, 		motor_kd = 0, 		motor_ki_limit = 30,
			mv_kp = 0.008, 		mv_ki = 0.0005, 		mv_kd = 0, 			mv_ki_limit = 0.1;
const float openmv_correction_threshold = 5;

//Location system mode
ControlMode control_mode = stop;
//Turning system
int ideal_heading = 0, ideal_ring = 0; //For 90 degree turning
//Coordinate mode
float target_x = 0, target_y = 0, target_heading = 0;
//Abs/Rel speed mode
float absVx = 0, absVy = 0, w = 0, relVx = 0, relVy = 0; //Absolute and Relative speed.
//Motor control
float motorA_target_speed = 0, motorB_target_speed = 0, motorC_target_speed = 0, motorD_target_speed = 0;

//Internal functions:
//==================== PID Calc related =====================:
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

//==================== convinient setters(forced to be used) =====================:
void set_target_motor_speed(float speedA, float speedB, float speedC, float speedD)
{
		motorA_target_speed = speedA;
		motorB_target_speed = speedB;
		motorC_target_speed = speedC;
		motorD_target_speed = speedD;
}

void set_abs_speed(float vx, float vy, float _w)
{
	absVx = vx; absVy = vy; w = _w;
}

void set_rel_speed(float vx, float vy, float _w)
{
	relVx = vx; relVy = vy; w = _w;
}

void set_target(float tx, float ty, float heading)
{
	target_x = tx; target_y = ty; target_heading = heading;
}

void set_mode(ControlMode mode)
{
	clear_state_stop();
	control_mode = mode;
}

//==================== control loop related =====================:
void clear_state_stop() //Remove the effect of previous PIDs
{
	set_target(OPS_x, OPS_y, OPS_heading);
	set_abs_speed(0, 0, 0);
	set_rel_speed(0, 0, 0);
	Set_Pwm_All(0, 0, 0, 0);

	reset_pid(&motorA_PID);
	reset_pid(&motorB_PID);
	reset_pid(&motorC_PID);
	reset_pid(&motorD_PID);

	reset_pid(&x_pid);
	reset_pid(&y_pid);
	reset_pid(&heading_pid);
}

void check_stable(void)
{
	static float prev_x,prev_y, prev_heading;
	if(fabs(prev_x - OPS_x) < 0.01 && fabs(prev_y - OPS_y) < 0.01 && fabs(OPS_heading - prev_heading) < 1)
		flag_stable = 1;
	else
		flag_stable = 0;
	prev_x = OPS_x; prev_y = OPS_y; prev_heading = OPS_heading;
}

//Only available for coordinate mode.
void check_arrive(void)
{
	if(fabs(x_pid.error) < 0.025 && fabs(y_pid.error) < 0.025 && fabs(heading_pid.error) < 1.5)
			flag_arrive = 1;
		else
			flag_arrive = 0;
}


//==================== control calculations =====================:

//Public functions:
//==================== exposed control functions =====================:
void Control_Init(void)
{
	set_pid(&x_pid, xy_kp, xy_ki, xy_kd, xy_ki_limit, speed_limit);
	set_pid(&y_pid, xy_kp, xy_ki, xy_kd, xy_ki_limit, speed_limit);
	set_pid(&heading_pid, heading_kp, heading_ki, heading_kd, heading_ki_limit, speed_limit);
	
	set_pid(&motorA_PID, motor_kp, motor_ki, motor_kd, motor_ki_limit, 100);
	set_pid(&motorA_PID, motor_kp, motor_ki, motor_kd, motor_ki_limit, 100);
	set_pid(&motorA_PID, motor_kp, motor_ki, motor_kd, motor_ki_limit, 100);
	set_pid(&motorA_PID, motor_kp, motor_ki, motor_kd, motor_ki_limit, 100);

	set_pid(&openmv_x_PID, mv_kp, mv_ki, mv_kd, mv_ki_limit, angular_speed_limit); //speed_limit is problematic here, needs fine tune
	set_pid(&openmv_y_PID, mv_kp, mv_ki, mv_kd, mv_ki_limit, angular_speed_limit);
}

//For main loop:
void Control_Loop(void)
{
	check_stable();

	switch (control_mode)
	{
		case stop:
			break;
		
		case relativeSpeedMode:
		{

			break;
		}
		
		default:
			break;
	}
}

//Different modes/moves:
void Stop_Run(void)
{
	set_mode(stop);
}

void Rel_Speed_Run(float vx, float vy, float w)
{

}

void Abs_Speed_Run(float vx, float vy, float w)
{

}

void Target_Run(float x, float y, float heading)
{

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
	OLED_ShowSignedNum(2, 1, (int)(relVx*100), 4);
	OLED_ShowSignedNum(2, 7, (int)(relVy*100), 4);
	OLED_ShowSignedNum(3, 1, (int)(w*100), 4);
	
	OLED_ShowSignedNum(4, 1, (int) x_pid.error * 1000, 3);
	OLED_ShowSignedNum(4, 5, (int) y_pid.error * 1000, 3);
}

//==================== temp unused funcs =====================:

// void wheel_speed_calc(float vx, float vy, float vw) //m/s
// {
// 	int temp[4];
// 	temp[0] = (vy + vx + vw * (H + W)); 
// 	temp[1] = (vy - vx - vw * (H + W));
// 	temp[2] = (vy - vx + vw * (H + W)); 
// 	temp[3] = (vy + vx - vw * (H + W));
	
// 	//Unify if too big:(problematic)
// 	int max_speed = 0;
// 	for(int itr = 0; itr < 4; itr++)
// 	{
// 		if (abs(max_speed) < abs(temp[itr]))
// 			max_speed = temp[itr];
// 	}
	
// 	if (abs(max_speed) > speed_limit)
// 	{
// 		motorA_speed=temp[0]*speed_limit/abs(max_speed);
// 		motorB_speed=temp[1]*speed_limit/abs(max_speed);
// 		motorC_speed=temp[2]*speed_limit/abs(max_speed);
// 		motorD_speed=temp[3]*speed_limit/abs(max_speed);
// 	} else 
// 	{
// 		motorA_speed=temp[0];
// 		motorB_speed=temp[1];
// 		motorC_speed=temp[2];
// 		motorD_speed=temp[3];
// 	}
// }

// void coordinate_Loop(void)
// {
// 	pid_calculate(&x_pid, target_x, OPS_x);
// 	pid_calculate(&y_pid, target_y, OPS_y);
// 	pid_calculate(&heading_pid, target_heading, OPS_heading+OPS_ring*360);
	
// 	relVx = -(y_pid.output)*sin(OPS_heading/PI)+(x_pid.output)*cos(OPS_heading/PI);
// 	relVy = (y_pid.output)*cos(OPS_heading/PI)+(x_pid.output)*sin(OPS_heading/PI);
// 	w = heading_pid.output;
	
// 	wheel_speed_calc(vx,vy,w);
	
// 	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
// 	check_stable();
// }

// void Set_Control_Mode(ControlMode mode)
// {
// 	control_mode = mode;
// 	if(mode == coordinateMode)
// 	{
// 		reset_pid(&x_pid);
// 		reset_pid(&y_pid);
// 		reset_pid(&heading_pid);
// 	}
		
// 	target_x = OPS_x; target_x = OPS_y; target_heading = OPS_heading;
// 	vx = 0; vy = 0; w = 0;
// }	


// void Wheel_Run_Loop(void)
// {
// 	if (control_mode == stop)
// 	{
// 		Set_Speed_All(0, 0, 0, 0);
// 		return;
// 	}
// 	else if(control_mode == coordinateMode)
// 	{
// 		coordinate_Loop();
// 		wheel_speed_calc(vx, vy, w);
// 		Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
// 	}
// 	else if(control_mode == velocityMode)
// 	{
// 		wheel_speed_calc(vx, vy, w);
// 		Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
// 	}
// }


// void Go_Position_Test(float x, float y, float heading)
// {
// 	Set_Target_Coordinate(x, y);
// 	Set_Target_Heading(heading);
// 	Wheel_Run_Loop();
// 	if (flag_arrive)
// 	{
// 		flag_start = 0;
// 	}
// }