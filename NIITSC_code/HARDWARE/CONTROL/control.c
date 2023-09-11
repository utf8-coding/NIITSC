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

//Public:
int motorA_speed = 0, motorB_speed = 0, motorC_speed = 0, motorD_speed = 0;

//Internal:
const float stable_threshold = 40;
const u8 H = 1, W = 1; //NOT REAL VALUE!
const u8 speed_limit = 80;

int Check_Stable()
{
	float cur_x = OPS_x, cur_y = OPS_y, cur_heading = OPS_heading;
	delay_ms(50);
	if (fabs(cur_x - OPS_x + cur_y - OPS_y + cur_heading - OPS_heading) > stable_threshold)
		return 0;
	else
		return 1;
}

void Wheel_Speed_Calc(short vx, short vy, short vw)
{
	int temp[4];
	temp[0] = vx + vy + vw * (H + W); 
	temp[1] = -vx + vy + vw * (H + W);
	temp[2] = vx + vy - vw * (H + W); 
	temp[3] = -vx + vy - vw * (H + W);
	
	//Unify if too big:
	int max_speed = 0;
	for(int itr = 0; itr < 4; itr++)
	{
		if (abs(max_speed) < abs(temp[itr]))
			max_speed = temp[itr];
	}
	
	if (max_speed > 100)
	{
		motorA_speed=temp[0]*100/max_speed;
		motorB_speed=temp[1]*100/max_speed;
		motorC_speed=temp[2]*100/max_speed;
		motorD_speed=temp[3]*100/max_speed;
	} else 
	{
		motorA_speed=temp[0];
		motorB_speed=temp[1];
		motorC_speed=temp[2];
		motorD_speed=temp[3];
	}
}

float loc_pid_calculate(P_PID_HANDLE* pid, float input, float measure)
{
    pid->error = input - measure;         //????
    pid->sum_error += pid->error;

    //??????
		pid->kp_output = pid->kp * pid->error;
		pid->ki_output = pid->ki * pid->sum_error;
		pid->kd_output = pid->kd * (pid->error - pid->last_error);
		if((abs1(pid->ki_output) > pid->kiout_lim))
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
		
    //????
    if(abs1(pid->output) > pid->lim_output)
    {
        if(pid->output > 0)
            pid->output = pid->lim_output;
        else
            pid->output = -pid->lim_output;
    }
		pid->last_error = pid->error;
		return 0;
}

void To_Coordinate(float x,float y ,float w)
{
	loc_pid_calculate(&x_axis_pid,X_for_DMA,x);
	loc_pid_calculate(&y_axis_pid,Y_for_DMA,y);
	loc_pid_calculate(&z_axis_pid,Z_for_DMA,z);
	if((x_axis_pid.output+y_axis_pid.output)>x_axis_pid.lim_output)
	{
		float xy_total=x_axis_pid.output+y_axis_pid.output;
		x_axis_pid.output=(x_axis_pid.output*x_axis_pid.lim_output)/xy_total;
		y_axis_pid.output=(y_axis_pid.output*x_axis_pid.lim_output)/xy_total;
	}
	cal_vx=(y_axis_pid.output)*sin(Z_for_DMA*0.00017453)+(x_axis_pid.output)*cos(Z_for_DMA*0.00017453);
	cal_vy=(y_axis_pid.output)*cos(Z_for_DMA*0.00017453)-(x_axis_pid.output)*sin(Z_for_DMA*0.00017453);
	Kinematic_Analysis(cal_vx,cal_vy,z_axis_pid.output);

}
