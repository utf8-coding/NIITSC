#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ƺ������壻��������
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
#define a_PARAMETER 0.095
#define b_PARAMETER 0.11715
#define Velocity_KP 5
#define Velocity_KI 0.3
#define max_speed 4000

extern int position;
extern int Encoder_A, Encoder_B, Encoder_C, Encoder_D;
extern int motorA_speed, motorB_speed, motorC_speed, motorD_speed;

extern	int Balance_Pwm,Velocity_Pwm;
int TIM1_UP_IRQHandler(void);
int stable_confirm(int a);
void stop(void);
void Set_Pwm_FrontRight(int motor);//��ֵ��PWM�Ĵ���
void Set_Pwm_FrontLeft(int motor);//��ֵ��PWM�Ĵ���
void Set_Pwm_BackRight(int motor);//��ֵ��PWM�Ĵ���
void Set_Pwm_BackLeft(int motor);//��ֵ��PWM�Ĵ���
void Set_Pwm_Car(int motor_A,int motor_B,int motor_C,int motor_D);//����pwm��ֵ
void Kinematic_Analysis(short Vx, short Vy, short Vz);
void stop(void);
void Key(void);  //�����޸�����״̬ 
void Xianfu_Pwm(void);  //����PWM��ֵ
void TIM7_IRQHandler(void);//�жϷ�����
void TIM7_Init(u16 arr,u16 psc);//��ʱ��7��ʼ������
int Position_PID (int position,int target);
int Incremental_PI_A (int Encoder_A,int Target_A);
int Incremental_PI_B (int Encoder_B,int Target_B);
int Incremental_PI_C (int Encoder_C,int Target_C);
int Incremental_PI_D (int Encoder_D,int Target_D);
float myabss(float a);







#endif
