#ifndef _QEI_H_
#define _QEI_H_

#include "common.h"

#define MaxPwm  17000
#define MinPwm -17000

//����ʽ����ṹ��
typedef struct PID{
  int SetSpeed;
  double Proportion;              
  double Integral;               
  double Derivative;              
  int LastError;    							//Error[-1]
  int PrevError;    							//Error[-2]
	int PWMOut;
}PID;


//λ��ʽ���峣��
#define   SPEED_CONTROL_PERIOD                    5
#define   MOTOR_ENCODE_CONSTANT_RIGHT             7690//35000          //����Ŀ̲�����
#define   MOTOR_ENCODE_CONSTANT_LEFT              7690//35000
#define   MOTOR_SPEED_CONSTANT_RIGHT              1000.0/SPEED_CONTROL_PERIOD/MOTOR_ENCODE_CONSTANT_RIGHT //ÿ��ת��Ȧ��
#define   MOTOR_SPEED_CONSTANT_LEFT               1000.0/SPEED_CONTROL_PERIOD/MOTOR_ENCODE_CONSTANT_LEFT
#define   MOTOR_ANGLE_CONSTANT_LEFT               360.0/MOTOR_ENCODE_CONSTANT_LEFT   //ÿ�������Ӧ�ĽǶ�
#define   MOTOR_ANGLE_CONSTANT_RIGHT              360.0/MOTOR_ENCODE_CONSTANT_RIGHT
#define   Pai                                     3.1416
#define   SPEED_INTEGRAL_MAX                      17000

#define   MOTOR_OUT_DEAD_VAL                      0
#define   MOTOR_OUT_MAX                           17000
#define   MOTOR_OUT_MIN							             -17000
//λ��ʽ����ṹ��  �ٶ�
typedef struct
{
    float     Kp;
    float     Ki;
    float     Kd;   
    float     SpeedCtrOut;
    float     SpeedCtrIntegral;
    float     Speed;
    float     PreSpeed;
    float     SpeedSet;   //�趨�ٶ�
	  u8        SpeedConflag;
    u8        SpeedCtrPeriod;
	  s16       Cnt;
}SpeedClass;
//λ��ʽ����ṹ��   �Ƕ�
typedef struct
{
    float     Kp;
    float     Ki;
    float     Kd;   
    float     AngleCtrOut;
    float     AngleCtrIntegral;
    float     Angle;
    float     PreAngle;
    float     AngleSet;   //�趨�ٶ�
	  u8        AngleConflag;
    u8        AngleCtrPeriod; 
}AngleClass;
//������������ʽPID��ʼ��
void QEI_Init(void);
void PID_Init(void);
//�����������õ�����
void Left_SpeedCount(void);
void Right_SpeedCount(void);

//����ʽPID����
s32  Right_IncPID(s32 RightGetSpeed);
s32  Left_IncPID(s32 LeftGetSpeed);
void Motor_Output_Inc(void);
//λ��ʽPID����
void Left_SpeedControl(void);
void Left_AngleControl(void);
void Right_SpeedControl(void);
void Right_AngleControl(void);
void SpeedControl(void);
void Motor_Output_Pos(float PWM_Left,float PWM_Right);
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage);

extern PID PID_Left;
extern PID *pid_left;
extern PID PID_Right;
extern PID *pid_right;

extern SpeedClass LeftSpeedCtr;
extern SpeedClass RightSpeedCtr;
extern SpeedClass SpeedCtr;
extern AngleClass LeftAngleCtr;
extern AngleClass RightAngleCtr;

extern float fP;
extern float fD;
extern float fI;
extern float L_fP;
extern float L_fD;
extern float L_fI;
extern float L_fDelta;
extern float R_fP;
extern float R_fD;
extern float R_fI;
extern float fLeftVal;
extern float fRightVal;

#endif
