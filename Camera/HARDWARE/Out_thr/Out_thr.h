#ifndef OUT_THR_H
#define OUT_THR_H
#include "stm32f4xx.h"
#include "Flag_Group.h"
#include "pwm.h"
typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float error;
	float error_d;
	float pre_error;
	float Weight;
	float Expect;
	float Out_fp;
	float Out_fi;
	float Out_fiper;
	float Out_fd;
	float Output;
	float Old_Measure;

}xyz_object;

typedef struct
{
	xyz_object Pitch;
	xyz_object Roll;
	xyz_object Yaw;
}ControlPID;

typedef struct 
{
	u16 Motor_1;
	u16 Motor_2;
	u16 Motor_3;
	u16 Motor_4;
}Out_Thr;
extern ControlPID _Inner_Control;
extern ControlPID _Out_Control;
extern Out_Thr    Motor;
extern unsigned int thr;
void All_Out(int M_1,int M_2,int M_3,int M_4);
void Inner_Control(float T);
void Out_Control(float T) ;
void Control(void);
void LimIntegal(float *a,float *b,float *c,int MAX1,int MAX2);
void Expect_Set(float a,float b,float c);
float To_180_degrees(float x);
void Clear_Inter(void);
extern u16 Thr_Low;
extern u16 Thr_High;
#endif

