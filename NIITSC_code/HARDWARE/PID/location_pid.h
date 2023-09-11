#include "sys.h"

extern float cal_vx,cal_vy;

typedef struct
{
	u8 enable_lim_sum_error	: 1;	//??????
	u8 enable_lim_ouput		: 1;	//??????
	
	float kp;  			//????
	float ki;  			//????
	float kd;  			//????
//	float sum_error_lim;
	float kiout_lim;	//??????	
	float lim_output;		//???? ??????

	float sum_error;		//????
	float last_error;		//??????
	float last_last_error;	//???????
	
	float kd_output;
	float ki_output;
	float kp_output;

	float output;
	float error;
}  P_PID_HANDLE;

extern P_PID_HANDLE x_axis_pid,y_axis_pid,z_axis_pid,tof_z_axis_pid;


float loc_pid_calculate(P_PID_HANDLE* pid, float input, float measure);
//void loc_pid_init(P_PID_HANDLE pid,float kp,float ki,float kd,float kiout_lim,float lim_output);
void location_togo(float x,float y ,float z);
void location_togo_xz(float x,float y ,float z);
void loc_pid_init(P_PID_HANDLE* pid,float kp,float ki,float kd,float kiout_lim,float lim_output);
void update_maxspeed(P_PID_HANDLE* pid,float lim_output);

//void loc_pid_update(P_PID_HANDLE pid,float kp,float ki,float kd);
//void loc_pid_reset(P_PID_HANDLE pid);
