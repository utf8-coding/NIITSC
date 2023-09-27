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
	relativeSpeedMode,
	absoluteSpeedMode,
	coordinateMode
} ControlMode;


extern u8 speed_limit, angular_speed_limit;

void Control_Display_Specs(void);
void Go_Position_Test(float x, float y, float heading);
void Wheel_Run_Loop(void);
void Set_Speed_All(float speedA, float speedB, float speedC, float speedD);
void Set_Target_Heading(float heading);
void Set_Target_Coordinate(float x, float y);
void Set_Control_Mode(ControlMode mode);
void Openmv_Correction(void);
u8 Openmv_Correction_Ready(void);
void Control_Init(void);
void Turn_Left90(void);
void Turn_Right90(void);


#endif
