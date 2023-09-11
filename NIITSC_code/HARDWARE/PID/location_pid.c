#include "location_pid.h"
#include "Ops.h"
#include "math.h"
#include "control.h"

extern int X_for_DMA,Y_for_DMA,Z_for_DMA;

P_PID_HANDLE x_axis_pid,y_axis_pid,z_axis_pid,tof_z_axis_pid;
float cal_vx,cal_vy;
//??????
float abs1(float a )
{
	if(a<0)return -a;
	else return a;
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
void location_togo(float x,float y ,float z)
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
void location_togo_xz(float x,float y ,float z)
{
	loc_pid_calculate(&x_axis_pid,X_for_DMA,x);
	loc_pid_calculate(&z_axis_pid,Z_for_DMA,z);
	if((x_axis_pid.output+y_axis_pid.output)>x_axis_pid.lim_output)
	{
		float xy_total=x_axis_pid.output+y_axis_pid.output;
		x_axis_pid.output=(x_axis_pid.output*x_axis_pid.lim_output)/xy_total;
		y_axis_pid.output=(y_axis_pid.output*x_axis_pid.lim_output)/xy_total;
	}
	cal_vx=(y_axis_pid.output)*sin(Z_for_DMA*0.017453)+(x_axis_pid.output)*cos(Z_for_DMA*0.017453);
	Kinematic_Analysis(cal_vx,cal_vy,z_axis_pid.output);
}
void update_maxspeed(P_PID_HANDLE* pid,float lim_output)
{
		pid->lim_output=lim_output;

}
void loc_pid_init(P_PID_HANDLE* pid,float kp,float ki,float kd,float kiout_lim,float lim_output)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->kiout_lim=kiout_lim;
	pid->lim_output=lim_output;
}
