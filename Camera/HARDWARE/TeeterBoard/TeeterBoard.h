#ifndef __TEETERBOARD_H
#define __TEETERBOARD_H

typedef struct
{
	float Roll_Value;
	float Roll_Prevalue;
	short Roll_Error;
	float Roll_AngleSpeed;
	float Roll_Offset;
	int   Pluse_Num;
	float Distance;
	char  Flag;
	char  Task_Flag;
}BalanceType;

typedef struct
{
	float   Kp ;
	float   Ki ;
	float   Kd ;
	float   Angle_Speed;
	float   Angle_Set;
	float   Angle_Now;
	float   Angle_Pre;
	float   Angle_Delta;
	float   Angle_PreDelta;
	float   Angle_Offset;
	float   Angle_Out;
}AngleType;

extern BalanceType balance;
extern AngleType angle;

void Board_Task(void);
void Balance_PID_Init(void);
void Balance_Forward(void);
void Balance_Keep(void);
void Balance_PWM_Out(void);

#endif


