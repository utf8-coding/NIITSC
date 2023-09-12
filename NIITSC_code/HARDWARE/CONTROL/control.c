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

//Public:
u8 speed_limit = 80, angular_speed_limit = 20;
float target_x = 0, target_y = 0, target_heading = 0;

//Internal:
float vx = 0, vy = 0, w = 0;
int motorA_speed = 0, motorB_speed = 0, motorC_speed = 0, motorD_speed = 0;
const float PI = 3.14159265, H = 0.188, W = 0.25, Wheel_C = 0.035*2*PI, Speed_PWM_Ratio = 22*Wheel_C/800;
const float x_kp = 0.85, x_ki = 0.0045, y_kp = 0.80, y_ki = 0.0030, xy_kd = 0.0005, xy_ki_limit = 0, z_kp = 0.03, z_ki = 0.00015, z_kd = 0.0, z_ki_limit = 0;
PID x_pid,y_pid,heading_pid;

void set_pid(PID* pid,float kp,float ki,float kd,float kiout_lim,float lim_output);

void PID_Init(void)
{
	set_pid(&x_pid, x_kp, x_ki, xy_kd, xy_ki_limit, speed_limit*Speed_PWM_Ratio);
	set_pid(&y_pid, y_kp, y_ki, xy_kd, xy_ki_limit, speed_limit*Speed_PWM_Ratio);
	set_pid(&heading_pid, z_kp, z_ki, z_kd, z_ki_limit, 0.3);
}

void set_pid(PID* pid,float kp,float ki,float kd,float kiout_lim,float lim_output)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->kiout_lim=kiout_lim;
	pid->lim_output=lim_output;
}

void check_stable(void)
{
	static float prev_x,prev_y, prev_heading;
	if(fabs(prev_x - OPS_x) < 0.01 && fabs(prev_y - OPS_y) < 0.01 && fabs(OPS_heading - prev_heading) < 1)
		flag_stable = 1;
	else
		flag_stable = 0;
	
	if(fabs(x_pid.error) < 0.015 && fabs(y_pid.error) < 0.015 && fabs(heading_pid.error) < 1.5)
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
	
//	if(fabs(pid->ki_output)> pid->kiout_lim && pid->enable_lim_sum_error)
//  {
//      if(pid->ki_output > 0)
//			{
//          pid->ki_output = pid->kiout_lim;
//					pid->sum_error=pid->kiout_lim / pid->ki;
//			}
//      else
//			{
//          pid->ki_output = -pid->kiout_lim;
//					pid->sum_error=-pid->kiout_lim / pid->ki;
//			}
//	}
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

void Wheel_Speed_Calc(float vx, float vy, float vw) //m/s
{
	int temp[4];
	temp[0] = (vy + vx - vw * (H + W))/Speed_PWM_Ratio; 
	temp[1] = (vy - vx + vw * (H + W))/Speed_PWM_Ratio;
	temp[2] = (vy - vx - vw * (H + W))/Speed_PWM_Ratio; 
	temp[3] = (vy + vx + vw * (H + W))/Speed_PWM_Ratio;
	
	//Unify if too big:
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
		LED_Flip();
		motorA_speed=temp[0];
		motorB_speed=temp[1];
		motorC_speed=temp[2];
		motorD_speed=temp[3];
	}
}

void Set_Target_Coordinate(float x,float y ,float heading)
{
	target_x = x; target_y = y; target_heading = heading;
}

void Wheel_Run_Loop()
{
	if (!flag_run)
	{
		Set_Speed_All(0, 0, 0, 0);
		return;
	}
	
	pid_calculate(&x_pid,OPS_x, target_x);
	pid_calculate(&y_pid,OPS_y, target_y);
	pid_calculate(&heading_pid,OPS_heading, target_heading);
//	if((x_pid.output+y_pid.output)>x_pid.lim_output)
//	{
//		float xy_total=x_pid.output+y_pid.output;
//		x_pid.output=(x_pid.output*x_pid.lim_output)/xy_total;
//		y_pid.output=(y_pid.output*x_pid.lim_output)/xy_total;
//	}
	vx = -(y_pid.output)*sin(OPS_heading/PI)+(x_pid.output)*cos(OPS_heading/PI);
	vy = -(y_pid.output)*cos(OPS_heading/PI)-(x_pid.output)*sin(OPS_heading/PI);
	w = -heading_pid.output;
	Wheel_Speed_Calc(vx,vy,w);
	
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
	check_stable();
}

void Go_Direction_Test(float vx,float vy,float vw)
{
	Wheel_Speed_Calc(vx, vy, vw);
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
}

void Go_Position_Test(float x, float y, float heading)
{
	target_x = x; target_y = y; target_heading = heading;
	pid_calculate(&x_pid, OPS_x, target_x);
	pid_calculate(&y_pid, OPS_y, target_y);
	pid_calculate(&heading_pid, OPS_heading+OPS_ring*360, target_heading);
	vx = -((y_pid.output)*sin(OPS_heading/PI)+(x_pid.output)*cos(OPS_heading/PI));
	vy = -((y_pid.output)*cos(OPS_heading/PI)-(x_pid.output)*sin(OPS_heading/PI));
	w = -heading_pid.output;
	Wheel_Speed_Calc(vx,vy,w);
	
	Set_Speed_All(motorA_speed, motorB_speed, motorC_speed, motorD_speed);
	
	check_stable();
	if (flag_arrive)
	{
		flag_run = 0;
		flag_start = 0;
	}
}

void Control_Display_Specs(void)
{
	OLED_ShowString(1, 1, "Ctrl:");
	OLED_ShowSignedNum(2, 1, (int)(vx*100), 4);
	OLED_ShowSignedNum(2, 7, (int)(vy*100), 4);
	OLED_ShowSignedNum(3, 1, (int)(w*100), 4);
}
