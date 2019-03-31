#ifndef _IMU_H_
#define	_IMU_H_

#include "stm32f4xx.h"
#include "math.h"
#include  "softdrive_mpu6050.h"

typedef struct 
{
	xyz_f_t err;
	xyz_f_t err_tmp;
	xyz_f_t err_lpf;
	xyz_f_t err_Int;
	xyz_f_t g;
	
}ref_t;

extern xyz_f_t reference_v;
void IMUupdate(float half_T,float gx, float gy, float gz, float ax, float ay, float az,float *rol,float *pit,float *yaw);
extern float SOFT_Roll,SOFT_Pitch,SOFT_Yaw; 
extern int Moving_Median(uint8_t item,uint8_t width_num,int in);

#define IMU_INTEGRAL_LIM  ( 2.0f *ANGLE_TO_RADIAN )
#define NORM_ACC_LPF_HZ 10  		//(Hz)
#define REF_ERR_LPF_HZ  1				//(Hz)
#define REAL   float
#define TAN_MAP_RES     0.003921569f     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293f
#define TAN_MAP_SIZE    256
#define MY_PPPIII   3.14159f
#define MY_PPPIII_HALF   1.570796f
extern float ref_q[4];
extern ref_t 	ref;
float my_abs(float f);
REAL fast_atan2(REAL y, REAL x);
float my_pow(float a);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float x,float zoom);
#endif

