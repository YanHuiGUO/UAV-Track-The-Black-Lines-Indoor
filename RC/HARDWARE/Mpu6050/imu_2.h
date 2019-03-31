#ifndef _IMU_H_
#define	_IMU_H_

#include "stm32f4xx.h"
#include "math.h"
#include "imu.h"
#include "softdrive_mpu6050_2.h"
extern xyz_f_t reference_v_2;

typedef struct 
{
	xyz_f_t err;
	xyz_f_t err_tmp;
	xyz_f_t err_lpf;
	xyz_f_t err_Int;
	xyz_f_t g;
	
}ref_t_2;
extern float SOFT_Roll_2,SOFT_Pitch_2,SOFT_Yaw_2;

float my_abs(float f);
float fast_atan2(float y, float x);
float my_pow(float a);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float x,float zoom);
//,float *rol_,float *pit_,float *yaw_); 
#endif

