#include "TeeterBoard.h"
#include "dmp_mpu6050.h"
#include "QEI.h"
#include "pwm.h"
#include "oled.h"
#include "delay.h"

#define circle_pulse 7690;
float PWM_Times = 1;

BalanceType balance;
AngleType angle;

typedef void(* Balance_Task)(void);
Balance_Task Balance_Carry[2] = {Balance_Forward,Balance_Keep};

/***************************************************************************************
@f_name: void Main_Task(void)
@brief:	 任务运行控制
@param:	 None
@return: None
****************************************************************************************/
	
void Board_Task(void)
{	
	balance.Task_Flag = 1;
	(*Balance_Carry[balance.Task_Flag])();
}

/***************************************************************************************
@f_name: void Balance_Forward(void)
@brief:	 向前运动至中心点附近
@param:	 None
@return: None
****************************************************************************************/
void Balance_Forward(void)
{
}

/***************************************************************************************
@f_name: void Balance_Deal(void)
@brief:	 平衡PID初始化
@param:	 None
@return: None
****************************************************************************************/

float Set_Kp = 0;
float angle_P = 0,angle_I = 0,angle_D = 0;

void Balance_PID_Init(void)
{
	angle.Angle_Set = 0;
	angle.Kp = 5;
	angle.Ki = 0;
	angle.Kd = 3.2;
}

/***************************************************************************************
@f_name: void Balance_Keep(void)
@brief:	 保持平衡
@param:	 None
@return: None
****************************************************************************************/

void Balance_Keep(void)
{
	angle.Angle_Now = DMP_Cal();
	angle.Angle_PreDelta = angle.Angle_Delta;
	angle.Angle_Delta = angle.Angle_Set - angle.Angle_Now;
	push(9,angle.Angle_Delta-angle.Angle_PreDelta);
	angle.Angle_Speed = (angle.Angle_Delta-angle.Angle_PreDelta)*50;
	if((angle.Angle_Delta>=100)||(angle.Angle_Delta<=-100))
	{
    angle.Kp = 7;
	}
	else
	angle.Kp = Set_Kp;
	
	angle_P = angle.Angle_Delta * angle.Kp;//JIADU
	angle_D = angle.Angle_Speed * angle.Kd;//JIAOSUDU
	angle.Angle_Out = angle_P + angle_D ;
	if(angle.Angle_Out > 0)angle.Angle_Out += 2230;  // 2230
	if(angle.Angle_Out < 0)angle.Angle_Out -= 2230;   // 2230
	if(angle.Angle_Out > 6000)angle.Angle_Out = 6000;
	if(angle.Angle_Out < -6000)angle.Angle_Out = -6000;
	
	//if(angle.Angle_Out > 70)angle.Angle_Out = 70;
	//if(angle.Angle_Out < -70)angle.Angle_Out = -70;
}

void Angle_PID_Change(void)
{
//	angle.Kp = UartData[0];
//	angle.Kd = UartData[1];
}

void Balance_PWM_Out(void)
{			
	s32 Left_Pwm = 0,Right_Pwm = 0;//PID计算出的速度
	
	Right_Pwm = -angle.Angle_Out;
	Left_Pwm = angle.Angle_Out;

	if(Right_Pwm > MaxPwm)
		 Right_Pwm = MaxPwm;
	if(Right_Pwm < MinPwm)
		 Right_Pwm = MinPwm;
	if(Left_Pwm > MaxPwm)
		 Left_Pwm = MaxPwm;
	if(Left_Pwm < MinPwm)
		 Left_Pwm = MinPwm;
	
	if(Right_Pwm > 0)
	{
		PWMSet(1,0);
		PWMSet(2,Right_Pwm);			
	}
	else
	{
		PWMSet(1,-Right_Pwm);
		PWMSet(2,0);
	}
	
	if(Left_Pwm > 0)
	{
		PWMSet(3,Left_Pwm);
		PWMSet(4,0);	
	}
	else
	{
		PWMSet(3,0);
		PWMSet(4,-Left_Pwm);	
	}			
}




