#ifndef _POSITION_CAMERA_H
#define _POSITION_CAMERA_H
#include "include.h"
typedef struct
{
	int Get_X_Pos;
	int Get_Y_Pos;
	
	u8  Img_Mode;//黑线类型

	float Except_Xspeed;
	float Except_Yspeed;
	float X_Speed;
	float Y_Speed;
  xyz_f_t Speed_Err;
	xyz_f_t Speed_Err_old;
	xyz_f_t Speed_Err_d;
	xyz_f_t Speed_Err_i;
	xyz_f_t Speed_Pid_Out;
	pid_t Speed_X;
	pid_t Speed_Y;

}Position_Group;

typedef struct
{
	float GryoExcept;
//	float GryoError;
//	float GryoError_old;
//	float GryoError_d;
//	float GryoError_i;
//	float Pid_Out;
//	float kp;
//	float kd;
//	float ki;
	int TurnTimes;
	u8 TurnStart;
	u8 TurnSwitch;//切换Z轴角速度期望
	u8 TurnComplete;//转向完成标志
}TurnGroup;
void Stay_Cross(void);
void Stay_Speed(void);
void ClearDirection(void);
void TurnDirection(float ToAngle,float *exceptz, float TurnSpeed ,float T);
extern Position_Group Camera_Pos;
extern Position_Group Speed_Pos;
extern TurnGroup Fly_Turn;
#endif

