#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ƺ������壻��������
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

typedef enum
{
	stop,
	openmvMode,
	relativeSpeedMode,
	absoluteSpeedMode,
	coordinateMode
} ControlMode;

void Control_Display_Specs(void);
void Control_Init(void);
void Control_Loop(void);
void Stop_Run(void);
void Open_Mv_Run(void);
void Rel_Speed_Run(float vx, float vy, float w);
void Abs_Speed_Run(float vx, float vy, float w);
void Target_Run(float x, float y, float heading);
void Turn_Left90(void);
void Turn_Right90(void);


#endif
