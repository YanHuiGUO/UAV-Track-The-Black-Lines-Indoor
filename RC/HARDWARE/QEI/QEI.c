#include "QEI.h"
#include "pwm.h"
#include "UartLink.h"
#include "TeeterBoard.h"
#include "MyTask.h"
//#include  "Track.h"

PID PID_Left;
PID *pid_left = &PID_Left;
PID PID_Right;
PID *pid_right = &PID_Right;

SpeedClass LeftSpeedCtr;
SpeedClass RightSpeedCtr;

AngleClass LeftAngleCtr;
AngleClass RightAngleCtr;

SpeedClass SpeedCtr;

/*******************************************************************************************
@f_name: void QEIInit(void)
@brief:	 编码器时钟初始化，TIM3用作左轮测脉冲，TIM4用作右轮测脉冲
@param:	 None
@return: None
********************************************************************************************/

void QEI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);//CH1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);//CH2

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);//CH1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);//CH2

	/*TIM3编码器模式初始化*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能ＴＩＭ3
	TIM_DeInit(TIM3);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period =0xffff;       //
	TIM_TimeBaseStructure.TIM_Prescaler =0;	    //设置预分频：
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//编码配置                        编码模式
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
														 TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising上升沿捕获
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;         //比较滤波器
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM3->CNT =0;
	TIM_Cmd(TIM3, ENABLE);   //使能定时器3

	/*TIM4编码器模式初始化*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能ＴＩＭ4
	TIM_DeInit(TIM4);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period =0xffff;       //
	TIM_TimeBaseStructure.TIM_Prescaler =0;	    //设置预分频：
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//编码配置                        编码模式
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
														 TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising上升沿捕获
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;         //比较滤波器
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM4->CNT =0;
	TIM_Cmd(TIM4, ENABLE);   //使能定时器4
}

/******************************************************************************************
@f_name: void PID_Init(void) 
@brief:	 增量式PID参数初始化
@param:	 None
@return: None
*******************************************************************************************/

void PID_Init(void) 
{
	//增量式PID参数初始化
  pid_left->LastError   = 0;                    
  pid_left->Proportion  = 8;           
  pid_left->Integral    = 5.1;        
  pid_left->SetSpeed    = 50;          //每周期脉冲数
	
	pid_right->LastError   = 0;                    
  pid_right->Proportion  = 8;           
  pid_right->Integral    = 5.1;          
  pid_right->SetSpeed    = 50;         //每周期脉冲数50 
	
	//位置式速度PID参数初始化
	LeftSpeedCtr.Kp = 8;
	LeftSpeedCtr.Ki = 2;
	LeftSpeedCtr.Kd = 0;
	LeftSpeedCtr.SpeedSet = 200;                     //每秒钟转SpeedSet/100圈
	
	RightSpeedCtr.Kp = 8;
	RightSpeedCtr.Ki = 2;
	RightSpeedCtr.Kd = 0;
	RightSpeedCtr.SpeedSet = 200;                    //每秒钟转SpeedSet/100圈
	
	//位置式角度PID参数初始化
	LeftAngleCtr.Kp =25; //30;
	LeftAngleCtr.Ki =3;//5;
	LeftAngleCtr.Kd =60; //55;
	LeftAngleCtr.AngleSet = 0;                    //控制轮子转动角度大小
	
	RightAngleCtr.Kp =35; //35;
	RightAngleCtr.Ki =10;//10;
	RightAngleCtr.Kd =50; //50;
	RightAngleCtr.AngleSet = 0;                   //控制轮子转动角度大小
	
	//位置式两轮同一速度PID参数初始化
	SpeedCtr.Kp = 0;
	SpeedCtr.Ki = 0;
	SpeedCtr.Kd = 0;
	SpeedCtr.SpeedSet = 0;
	
	Target.X_Kp = -0.2;
	Target.X_Ki = -0.015;
	Target.X_Kd = 0;
	
	Target.Y_Kp = -0.25;
	Target.Y_Ki = -0.03;
	Target.Y_Kd = 0;
	
	Target.X_DisSet = 10;
	Target.Y_DisSet = 10;
}


/*****************************************************************************************
@f_name: void Left_SpeedCount(void)
@brief:	 左右轮脉冲数读取
@param:	 None
@return: None
******************************************************************************************/
int Mypulse = 0;
void Left_SpeedCount(void) //左编码器中断服务
{
	LeftSpeedCtr.Cnt = (s16)(TIM3->CNT);
	TIM3->CNT = 0;
	Mypulse +=LeftSpeedCtr.Cnt;
}

void Right_SpeedCount(void) //左编码器中断服务
{
	RightSpeedCtr.Cnt = (s16)(TIM4->CNT);
//	Mypulse += RightSpeedCtr.Cnt;
	TIM4->CNT = 0;
}

/***************************************************************************************
@f_name:  int32 Left_IncPID(int32 GetSpeed)
@brief:	  左电机增量式PID
@param:	 None
@return: None
****************************************************************************************/

s32  Left_IncPID(s32 LeftGetSpeed) 
{ 
    s32  iError,iIncpid;                          //???? 
		Left_SpeedCount();
    iError  = pid_left->SetSpeed - LeftGetSpeed;         //Setspeed设定速度，GetSpeed编码器读取速度  
    iIncpid = (s32)(pid_left->Proportion * iError           //E[k]? 
										- pid_left->Integral * pid_left->LastError);    //E[k-1]? 
    pid_left->LastError = iError; 
    return(iIncpid);                             //????? 
}

/**************************************************************************************
@f_name: int32  Right_IncPID(int32 GetSpeed)
@brief:	 右电机增量式PID
@param:	 None
@return: None
****************************************************************************************/

s32  Right_IncPID(s32 RightGetSpeed) 
{ 
    s32  iError,iIncpid;                          //???? 
		Right_SpeedCount();
    iError  = -pid_right->SetSpeed - RightGetSpeed;         //Setspeed设定速度，GetSpeed编码器读取速度  
    iIncpid = (s32)(pid_right->Proportion * iError           //E[k]? 
										- pid_right->Integral * pid_right->LastError);    //E[k-1]? 
    pid_right->LastError = iError; 
    return(iIncpid);                             //????? 
}

/**************************************************************************************
@f_name:  void Motor_Output_Inc(void)
@brief:	  PWM输出
@param:	 None
@return: None
***************************************************************************************/

void Motor_Output_Inc(void)
{			
	pid_right->PWMOut += Right_IncPID(RightSpeedCtr.Cnt);
	pid_left->PWMOut += Left_IncPID(LeftSpeedCtr.Cnt);

	if(pid_right->PWMOut > MaxPwm)
		 pid_right->PWMOut = MaxPwm;
	if(pid_right->PWMOut < MinPwm)
		 pid_right->PWMOut = MinPwm;
	if(pid_left->PWMOut > MaxPwm)
		 pid_left->PWMOut = MaxPwm;
	if(pid_left->PWMOut < MinPwm)
		 pid_left->PWMOut = MinPwm;
	
	if(pid_right->PWMOut > 0)
	{
		PWMSet(1,0);
		PWMSet(2,pid_right->PWMOut);			
	}
	else
	{
		PWMSet(1,-pid_right->PWMOut);
		PWMSet(2,0);
	}
	
	if(pid_left->PWMOut > 0)
	{
		PWMSet(3,pid_left->PWMOut);
		PWMSet(4,0);	
	}
	else
	{
		PWMSet(3,0);
		PWMSet(4,-pid_left->PWMOut);	
	}			
}

/**************************************************************************************
@f_name:  void Left_SpeedControl(void)
@brief:	  左轮速度位置式PID
@param:	 None
@return: None
***************************************************************************************/

float L_fPreDelta;
float L_fDelta = 0;
float L_fP = 0,L_fI = 0,L_fD = 0;
void Left_SpeedControl(void)
{ 
  L_fP = 0,L_fI = 0,L_fD = 0;
	Left_SpeedCount();
	LeftSpeedCtr.PreSpeed = LeftSpeedCtr.Speed;
	LeftSpeedCtr.Speed = (float)LeftSpeedCtr.Cnt * MOTOR_SPEED_CONSTANT_LEFT; //每周期转过圆周的百分数			     
	LeftSpeedCtr.Cnt = 0;
	L_fPreDelta = L_fDelta;            
  L_fDelta  = LeftSpeedCtr.SpeedSet/100 - LeftSpeedCtr.Speed;
	//速度误差计算
  L_fP =  L_fDelta * LeftSpeedCtr.Kp * 100;
	L_fD =  (L_fDelta - L_fPreDelta) * LeftSpeedCtr.Kd * 100;
  L_fI =  L_fDelta * LeftSpeedCtr.Ki * 100;
	LeftSpeedCtr.SpeedCtrIntegral += L_fI;
		
	if( LeftSpeedCtr.SpeedCtrIntegral > SPEED_INTEGRAL_MAX)
    {
        LeftSpeedCtr.SpeedCtrIntegral = SPEED_INTEGRAL_MAX;
    }
    else if( LeftSpeedCtr.SpeedCtrIntegral < -SPEED_INTEGRAL_MAX)
    {
        LeftSpeedCtr.SpeedCtrIntegral = -SPEED_INTEGRAL_MAX;
    }

	LeftSpeedCtr.SpeedCtrOut = 	L_fP + L_fD + LeftSpeedCtr.SpeedCtrIntegral;
}

/**************************************************************************************
@f_name:  void Right_SpeedControl(void)
@brief:	  右轮速度位置式PID
@param:	 None
@return: None
***************************************************************************************/

float R_fPreDelta;
float R_fDelta = 0;
float R_fP = 0,R_fI = 0,R_fD = 0;
void Right_SpeedControl(void)
{ 
  R_fP = 0,R_fI = 0,R_fD = 0; 
	Right_SpeedCount();
	RightSpeedCtr.PreSpeed = RightSpeedCtr.Speed;
	RightSpeedCtr.Speed = (float)RightSpeedCtr.Cnt  * MOTOR_SPEED_CONSTANT_LEFT; //每周期转过圆周的百分数			     
	RightSpeedCtr.Cnt = 0;
	  R_fPreDelta = R_fDelta;            
    R_fDelta  =  -RightSpeedCtr.SpeedSet/100 - RightSpeedCtr.Speed;
	//速度误差计算
    R_fP =  R_fDelta * RightSpeedCtr.Kp * 100;
	  R_fD =  (R_fDelta - R_fPreDelta) * RightSpeedCtr.Kd * 100;
    R_fI =  R_fDelta * RightSpeedCtr.Ki * 100;
	  RightSpeedCtr.SpeedCtrIntegral += R_fI;
		
	if( RightSpeedCtr.SpeedCtrIntegral > SPEED_INTEGRAL_MAX)
    {
        RightSpeedCtr.SpeedCtrIntegral = SPEED_INTEGRAL_MAX;
    }
    else if( RightSpeedCtr.SpeedCtrIntegral < -SPEED_INTEGRAL_MAX)
    {
        RightSpeedCtr.SpeedCtrIntegral = -SPEED_INTEGRAL_MAX;
    }

	RightSpeedCtr.SpeedCtrOut = 	R_fP + R_fD + RightSpeedCtr.SpeedCtrIntegral;
}

/**************************************************************************************
@f_name:  void Left_AngleControl(void)
@brief:	  左轮角度位置式PID
@param:	 None
@return: None
***************************************************************************************/

void Left_AngleControl(void)
{ 
  L_fP = 0,L_fI = 0,L_fD = 0;
	Left_SpeedCount();
	LeftAngleCtr.PreAngle = LeftAngleCtr.Angle;
	LeftAngleCtr.Angle = (float)LeftSpeedCtr.Cnt  * MOTOR_ANGLE_CONSTANT_LEFT + LeftAngleCtr.PreAngle; //每周期转过度数		     
	LeftSpeedCtr.Cnt = 0;
	L_fPreDelta = L_fDelta;            
	L_fDelta  = LeftAngleCtr.AngleSet - LeftAngleCtr.Angle;
//速度误差计算
	L_fP =  L_fDelta * LeftAngleCtr.Kp *100;
	L_fD =  (L_fDelta - L_fPreDelta) * LeftAngleCtr.Kd  *100;
	L_fI =  L_fDelta * LeftAngleCtr.Ki *10;
	LeftAngleCtr.AngleCtrIntegral += L_fI;
		
	if( LeftAngleCtr.AngleCtrIntegral > SPEED_INTEGRAL_MAX)
    {
        LeftAngleCtr.AngleCtrIntegral = SPEED_INTEGRAL_MAX;
    }
    else if( LeftAngleCtr.AngleCtrIntegral < -SPEED_INTEGRAL_MAX)
    {
        LeftAngleCtr.AngleCtrIntegral = -SPEED_INTEGRAL_MAX;
    }

	LeftAngleCtr.AngleCtrOut = 	L_fP + L_fD + LeftAngleCtr.AngleCtrIntegral;
}

/**************************************************************************************
@f_name:  void Right_AngleControl(void)
@brief:	  右轮角度位置式PID
@param:	 None
@return: None
***************************************************************************************/

void Right_AngleControl(void)
{ 
  R_fP = 0,R_fI = 0,R_fD = 0;
	RightAngleCtr.PreAngle = 0;
	Right_SpeedCount();
	RightAngleCtr.PreAngle = RightAngleCtr.Angle;
	RightAngleCtr.Angle = (float)RightSpeedCtr.Cnt  * MOTOR_ANGLE_CONSTANT_RIGHT  + RightAngleCtr.PreAngle; //每周期转过度数		     
	RightSpeedCtr.Cnt = 0;
	R_fPreDelta = R_fDelta;            
	R_fDelta  = -RightAngleCtr.AngleSet - RightAngleCtr.Angle;
//速度误差计算
	R_fP =  R_fDelta * RightAngleCtr.Kp *100 ;
	R_fD =  (R_fDelta - R_fPreDelta) * RightAngleCtr.Kd *100 ;
	R_fI =  R_fDelta * RightAngleCtr.Ki * 10 ;
	RightAngleCtr.AngleCtrIntegral += R_fI;
		
	if( RightAngleCtr.AngleCtrIntegral > SPEED_INTEGRAL_MAX)
    {
        RightAngleCtr.AngleCtrIntegral = SPEED_INTEGRAL_MAX;
    }
    else if( RightAngleCtr.AngleCtrIntegral < -SPEED_INTEGRAL_MAX)
    {
        RightAngleCtr.AngleCtrIntegral = -SPEED_INTEGRAL_MAX;
    }

	RightAngleCtr.AngleCtrOut = 	R_fP + R_fD + RightAngleCtr.AngleCtrIntegral;
}

/**************************************************************************************
@f_name:  void SpeedControl(void)
@brief:	  两轮速度位置式PID（两轮同一PID）
@param:	 None
@return: None
***************************************************************************************/

float fPreDelta;
float fDelta = 0;
float fP = 0,fI = 0,fD = 0;
void SpeedControl(void)
{ 
  fP = 0,fI = 0,fD = 0;  
	Left_SpeedCount();
	Right_SpeedCount();

	SpeedCtr.PreSpeed = SpeedCtr.Speed;
	SpeedCtr.Speed = ((float)LeftSpeedCtr.Cnt  * MOTOR_SPEED_CONSTANT_LEFT //每周期转过圆周的百分数
				    + (float)RightSpeedCtr.Cnt * MOTOR_SPEED_CONSTANT_RIGHT )/2; 
	LeftSpeedCtr.Cnt = 0;
	RightSpeedCtr.Cnt = 0;

	fPreDelta = fDelta;            
  fDelta  = SpeedCtr.SpeedSet/100 - SpeedCtr.Speed;
	//速度误差计算
    fP =  fDelta * SpeedCtr.Kp * 100;
	  fD =  (fDelta - fPreDelta) * SpeedCtr.Kd * 100;
    fI =  fDelta * SpeedCtr.Ki * 100;
	
	SpeedCtr.SpeedCtrIntegral += fI;
	if( SpeedCtr.SpeedCtrIntegral > SPEED_INTEGRAL_MAX)
    {
        SpeedCtr.SpeedCtrIntegral = SPEED_INTEGRAL_MAX;
    }
    else if( SpeedCtr.SpeedCtrIntegral < -SPEED_INTEGRAL_MAX)
    {
        SpeedCtr.SpeedCtrIntegral = -SPEED_INTEGRAL_MAX;
    }

	SpeedCtr.SpeedCtrOut = 	fP + fD + SpeedCtr.SpeedCtrIntegral;
}

/**************************************************************************************
@f_name:  void Motor_Output_Pos(void)
@brief:	  位置式PWM输出
@param:	 None
@return: None
***************************************************************************************/

float fLeftVal=0,fRightVal=0;
void Motor_Output_Pos(float PWM_Left,float PWM_Right)
{   
	fLeftVal  =	 PWM_Left;
	fRightVal =  PWM_Right;

	if(fLeftVal > 0)
		 fLeftVal += MOTOR_OUT_DEAD_VAL;
	else if(fLeftVal < 0)
		 fLeftVal -= MOTOR_OUT_DEAD_VAL;  
	if(fRightVal> 0)
		 fRightVal += MOTOR_OUT_DEAD_VAL;
	else if(fRightVal< 0)
		 fRightVal -= MOTOR_OUT_DEAD_VAL;

	if(fLeftVal >= MOTOR_OUT_MAX)
	{
		 fLeftVal = MOTOR_OUT_MAX;
	}
	else if(fLeftVal <= MOTOR_OUT_MIN)
	{
		 fLeftVal = MOTOR_OUT_MIN;
	}
	if(fRightVal >= MOTOR_OUT_MAX)
	{
		 fRightVal = MOTOR_OUT_MAX;
	}
	else if(fRightVal <= MOTOR_OUT_MIN)
	{
		 fRightVal = MOTOR_OUT_MIN;
	}  
	SetMotorVoltage(fLeftVal,fRightVal);
}

void SetMotorVoltage(float fLeftVoltage,float fRightVoltage)
{
    int nOut = 0;
	
	if(fRightVoltage > 0)
	{
	    nOut = (int)fRightVoltage;
	    PWMSet(2,nOut);
	    PWMSet(1,0) ;
	}
	else
	{
	    nOut = (int)-fRightVoltage;
	    PWMSet(2,0);
	    PWMSet(1,nOut) ;
	}
    
	if(fLeftVoltage > 0)
	{
		 nOut = (int)fLeftVoltage;
	   PWMSet(3,nOut) ;
	   PWMSet(4,0);
	}
    else
	{
	   	nOut = (int)-fLeftVoltage;
	    PWMSet(3,0);
	    PWMSet(4,nOut) ;
	}
}



	



