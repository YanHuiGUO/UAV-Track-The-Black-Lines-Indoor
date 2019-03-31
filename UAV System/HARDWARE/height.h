#ifndef __HEIGHT_H
#define __HEIGHT_H
#include "stm32f4xx.h"
typedef struct
{	
	float Kp;
	float Kd;
	float Ki;
	float ExceptHeight;
	float GetHeight;
	float Err;
	float Err_old;
	float Err_p;
	float Err_d;
	float Err_i;
	float Pid_Out;
}height_pid;
extern float wz_acc ,wz_speed;
void Height_Ctrl(float T,float thr);

void Ultra_PID_Init(void);

void Ultra_Ctrl(float set,u8 dir);

extern float height_ctrl_out;

extern u8 ChooseRise_Land;

extern u8 HeightReady;

extern height_pid ultra_pid;	
#endif



