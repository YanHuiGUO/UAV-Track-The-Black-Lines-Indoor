#include "Out_thr.h"
#include "Flag_Group.h"
#include "imu.h"
#define Angle_MAX 15
#define Aspeed_MAX 500
#define Aspeed_Yaw_MAX 500
#define FB  0.2
unsigned int thr;
u16 Thr_Low = 0;
u16 Thr_High = 0;
ControlPID _Inner_Control;
ControlPID _Out_Control;
Flag_Group Flag_ThrControl;
Out_Thr    Motor;
//���ƽ������ٿ�ʼ����ʱ������Ҫ����
/*************************************************************************************************
@f_name:	void Clear_Inter(void)
@brief:	 ���¿�������״̬����������
@param:		None
@return:  None
***************************************************************************************************/
void Clear_Inter(void)
{
	_Out_Control.Roll.Out_fi  = 0;
	_Out_Control.Yaw.Out_fi   = 0;
	_Out_Control.Pitch.Out_fi = 0;
	
	_Inner_Control.Roll.Out_fi  = 0;
	_Inner_Control.Yaw.Out_fi   = 0;
	_Inner_Control.Pitch.Out_fi = 0;
}
/*************************************************************************************************
@f_name:	float To_180_degrees(float x)
@brief:	 ���Ƕ�ֵת����-180~+180��
@param:		�Ƕ�ֵX
@return: �Ƕ�ֵ
***************************************************************************************************/
float To_180_degrees(float x)
{
	return (x>180?(x-360):(x<-180?(x+360):x));
}
/*************************************************************************************************
@f_name:	void Expect_Set(float a,float b,float c)
@brief:	 ��̬��������
@param:		Roll,Pitch,Yaw
@return: None
***************************************************************************************************/
void Expect_Set(float a,float b,float c)
{
	_Out_Control.Roll.Expect  = a;
	_Out_Control.Pitch.Expect = b;
	_Out_Control.Yaw.Expect   = c;
}
/*************************************************************************************************
@f_name:	void LimIntegal(float *a,float *b,float *c,int MAX1,int MAX2)
@brief:	�����޷�
@param:		Roll���֣�Pitch���֣�Yaw���֣�ǰ�������ֵ��Yaw���ֵ
@return: None
***************************************************************************************************/
void LimIntegal(float *a,float *b,float *c,int MAX1,int MAX2)
{
			if(	*a > MAX1)
    {
        	*a = MAX1;
    }
    else if( *a < -MAX1)
    {
        	*a = -MAX1;
    }		
			if(	*b > MAX1)
    {
        	*b = MAX1;
    }
    else if( *b < -MAX1)
    {
        	*b = -MAX1;
    }		
			if(	*c > MAX2)
    {
        	*c = MAX2;
    }
    else if( *c < -MAX2)
    {
        	*c = -MAX2;
    }
}
/*************************************************************************************************
@f_name:	void Out_Control(float T)
@brief:	 �⻷����(�ǶȻ�)
@param:	TΪ��������
@return: None
***************************************************************************************************/
void Out_Control(float T)
{
		//�������޷�����ֹ�����ʱ�򳬵���
	_Out_Control.Roll.Expect  = LIMIT(_Out_Control.Roll.Expect,-30,30);
	_Out_Control.Pitch.Expect = LIMIT(_Out_Control.Pitch.Expect,-30,30);
	_Out_Control.Yaw.Expect   = LIMIT(_Out_Control.Yaw.Expect,-30,30);
	
	_Out_Control.Roll.error 	= To_180_degrees( _Out_Control.Roll.Expect - SOFT_Roll);
	_Out_Control.Pitch.error  = To_180_degrees( _Out_Control.Pitch.Expect - SOFT_Pitch);
	_Out_Control.Yaw.error  	= To_180_degrees( _Out_Control.Yaw.Expect -	SOFT_Yaw);
	
	_Out_Control.Roll.Weight	= ABS(_Out_Control.Roll.error)  / Aspeed_MAX;
	_Out_Control.Pitch.Weight	= ABS(_Out_Control.Pitch.error) / Aspeed_MAX;
	_Out_Control.Yaw.Weight	  = ABS(_Out_Control.Yaw.error)   / Aspeed_Yaw_MAX;
		
	_Out_Control.Roll.error_d  = _Out_Control.Roll.error - _Out_Control.Roll.pre_error ;
	_Out_Control.Pitch.error_d = _Out_Control.Pitch.error - _Out_Control.Pitch.pre_error ;
	_Out_Control.Yaw.error_d   = _Out_Control.Yaw.error - _Out_Control.Yaw.pre_error ;
	
	_Out_Control.Roll.pre_error  =  _Out_Control.Roll.error ;
	_Out_Control.Pitch.pre_error =  _Out_Control.Pitch.error;
	_Out_Control.Yaw.pre_error   =  _Out_Control.Yaw.error ;
	
	_Out_Control.Roll.Out_fp = 	_Out_Control.Roll.Kp *  _Out_Control.Roll.error;
	_Out_Control.Roll.Out_fd = 	_Out_Control.Roll.Kd *  _Out_Control.Roll.error_d * (0.65f + 0.35f * _Out_Control.Roll.Weight);
	_Out_Control.Roll.Out_fi += _Out_Control.Roll.Ki * 	_Out_Control.Roll.error;

	_Out_Control.Pitch.Out_fp = 	_Out_Control.Pitch.Kp *  _Out_Control.Pitch.error;
	_Out_Control.Pitch.Out_fd = 	_Out_Control.Pitch.Kd *  _Out_Control.Pitch.error_d * (0.65f + 0.35f * _Out_Control.Pitch.Weight);;
	_Out_Control.Pitch.Out_fi +=  _Out_Control.Pitch.Ki *  _Out_Control.Pitch.error;
	
	_Out_Control.Yaw.Out_fp = 	_Out_Control.Yaw.Kp *  _Out_Control.Yaw.error;
	_Out_Control.Yaw.Out_fd = 	_Out_Control.Yaw.Kd *  _Out_Control.Yaw.error_d * (0.65f + 0.35f * _Out_Control.Yaw.Weight);
	_Out_Control.Yaw.Out_fi +=  _Out_Control.Yaw.Ki *  _Out_Control.Yaw.error;
	//�ڻ�����������΢������Լ��ٴ��Ŷ��Ի��ֵĸ��ŷ�ֹ����ͻȻ����
	
	//�����޷�
	LimIntegal(	&_Out_Control.Roll.Out_fi,&	_Out_Control.Pitch.Out_fi,&_Out_Control.Yaw.Out_fi,Angle_MAX,Angle_MAX);
		
		//�ڻ����������
		//�������������������������PID�⣬���PID��������Ӧ��
	_Out_Control.Roll.Output  = _Out_Control.Roll.Out_fp +  _Out_Control.Roll.Out_fi + _Out_Control.Roll.Out_fd ;															
	_Out_Control.Pitch.Output = _Out_Control.Pitch.Out_fp +  _Out_Control.Roll.Out_fi + _Out_Control.Pitch.Out_fd ;														
	_Out_Control.Yaw.Output   = _Out_Control.Yaw.Out_fp +  _Out_Control.Yaw.Out_fi + _Out_Control.Yaw.Out_fd ;												
}


/*************************************************************************************************
@f_name:	void Inner_Control(float T)
@brief:	 �ڻ����ƣ����ٶȻ���
@param:	TΪ��������
@return: None
***************************************************************************************************/
void Inner_Control(float T)
{
	
	// �Խ��ٶ��޷�
//	_Out_Control.Roll.Output    =   LIMIT(_Out_Control.Roll.Output  , -Aspeed_MAX,Aspeed_MAX );
//	_Out_Control.Pitch.Output   =   LIMIT(_Out_Control.Pitch.Output , -Aspeed_MAX,Aspeed_MAX );
//	_Out_Control.Yaw.Output   	=  	LIMIT(_Out_Control.Yaw.Output   , -Aspeed_MAX,Aspeed_MAX );
	
  _Inner_Control.Roll.error 	=   _Out_Control.Roll.Output  - mpu6050.Gyro_deg.x;
	_Inner_Control.Pitch.error  =  -_Out_Control.Pitch.Output - mpu6050.Gyro_deg.y;
	_Inner_Control.Yaw.error  	=  -_Out_Control.Yaw.Output   - mpu6050.Gyro_deg.z;
	
	_Inner_Control.Roll.Weight	= ABS(_Inner_Control.Roll.error)  / Aspeed_MAX;
	_Inner_Control.Pitch.Weight	= ABS(_Inner_Control.Pitch.error) / Aspeed_MAX;
	_Inner_Control.Yaw.Weight	  = ABS(_Inner_Control.Yaw.error)   / Aspeed_Yaw_MAX;
	
	//ֱ���ò���ֵ������΢�����Ҫ����������Ӱ�죬�����������ᡣ
	_Inner_Control.Roll.error_d  = mpu6050.Gyro_deg.x  - _Inner_Control.Roll.pre_error ;
	_Inner_Control.Pitch.error_d = mpu6050.Gyro_deg.y  - _Inner_Control.Pitch.pre_error ;
	_Inner_Control.Yaw.error_d   = mpu6050.Gyro_deg.z  - _Inner_Control.Yaw.pre_error ;
	
	_Inner_Control.Roll.pre_error  =  mpu6050.Gyro_deg.x;
	_Inner_Control.Pitch.pre_error =  mpu6050.Gyro_deg.y;
	_Inner_Control.Yaw.pre_error   =  mpu6050.Gyro_deg.z ;
	
	_Inner_Control.Roll.Out_fp = 	_Inner_Control.Roll.Kp *  _Inner_Control.Roll.error;
	_Inner_Control.Roll.Out_fd = -_Inner_Control.Roll.Kd *	_Inner_Control.Roll.error_d;
	_Inner_Control.Roll.Out_fi += _Inner_Control.Roll.Ki *  _Inner_Control.Roll.error;

	_Inner_Control.Pitch.Out_fp =  _Inner_Control.Pitch.Kp * _Inner_Control.Pitch.error;
	_Inner_Control.Pitch.Out_fd = -_Inner_Control.Pitch.Kd * _Inner_Control.Pitch.error_d; 
	_Inner_Control.Pitch.Out_fi += _Inner_Control.Pitch.Ki * _Inner_Control.Pitch.error;
	
	_Inner_Control.Yaw.Out_fp = 	_Inner_Control.Yaw.Kp *  _Inner_Control.Yaw.error;
	_Inner_Control.Yaw.Out_fd =  -_Inner_Control.Yaw.Kd *  _Inner_Control.Yaw.error_d;
	_Inner_Control.Yaw.Out_fi +=  _Inner_Control.Yaw.Ki *  _Inner_Control.Yaw.error;
	//�ڻ�����������΢������Լ��ٴ��Ŷ��Ի��ֵĸ��ŷ�ֹ����ͻȻ����
	
	//�����޷�
	LimIntegal(	&_Inner_Control.Roll.Out_fi,&	_Inner_Control.Pitch.Out_fi,&_Inner_Control.Yaw.Out_fi,Aspeed_MAX,Aspeed_Yaw_MAX);
		
		//�ڻ����������
		//�������������������������PID�⣬���PID��������Ӧ��
	_Inner_Control.Roll.Output  =(1-(float)FB) *( _Inner_Control.Roll.Out_fp +  _Inner_Control.Roll.Out_fi + _Inner_Control.Roll.Out_fd ) \
																+ (float)FB *(LIMIT((0.45f + 0.55f*_Inner_Control.Roll.Weight),0,1)) * _Inner_Control.Roll.Expect;
	_Inner_Control.Pitch.Output =(1-(float)FB) *( _Inner_Control.Pitch.Out_fp +  _Inner_Control.Pitch.Out_fi + _Inner_Control.Pitch.Out_fd ) \
																+ (float)FB *(LIMIT((0.45f + 0.55f*_Inner_Control.Pitch.Weight),0,1)) * _Inner_Control.Pitch.Expect;
	_Inner_Control.Yaw.Output   =(1-(float)FB) *( _Inner_Control.Yaw.Out_fp +  _Inner_Control.Yaw.Out_fi + _Inner_Control.Yaw.Out_fd ) \
																+ (float)FB *(LIMIT((0.45f + 0.55f*_Inner_Control.Yaw.Weight),0,1)) * _Inner_Control.Yaw.Expect;
}
/*************************************************************************************************
@f_name:	void Control(void)
@brief:	 ��������
@param:	 None
@return: None
***************************************************************************************************/
void Control(void)//PWMռ�ձ���45.6%��ʼ�������ת�� thr/5000
{
	//thr��Χ0-2500
	thr = Thr_Low;
	thr = LIMIT(thr,Thr_Low,Thr_High);
	if(Flag_ThrControl.Fly_ready ==1 && !Flag_ThrControl.Fly_Stop)
	{
		Motor.Motor_1 = thr - _Inner_Control.Roll.Output - _Inner_Control.Pitch.Output - _Inner_Control.Yaw.Output ;
		Motor.Motor_2 = thr + _Inner_Control.Roll.Output - _Inner_Control.Pitch.Output + _Inner_Control.Yaw.Output ; 
		Motor.Motor_3 = thr + _Inner_Control.Roll.Output + _Inner_Control.Pitch.Output - _Inner_Control.Yaw.Output ; 
		Motor.Motor_4 = thr - _Inner_Control.Roll.Output + _Inner_Control.Pitch.Output + _Inner_Control.Yaw.Output ;	
	}  
	else if(!Flag_ThrControl.Fly_ready || Flag_ThrControl.Fly_Stop)
	{
		Motor.Motor_1 = 0;
		Motor.Motor_2 = 0; 
		Motor.Motor_3 = 0; 
		Motor.Motor_4 = 0;
	}
	//All_Out(Motor.Motor_1,Motor.Motor_2,Motor.Motor_3,Motor.Motor_4);
}
/*************************************************************************************************
@f_name:	void All_Out(int M_1,int M_2,int M_3,int M_4)
@brief:	 �����ռ�ձ�(40%-80%)
@param:	 ���1�����2�����3�����4
@return: None 0-5000
***************************************************************************************************/
//Motor��Χ0-2000
void All_Out(int M_1,int M_2,int M_3,int M_4)
{
	PWMSet(4,LIMIT(M_4,M_4+1500,4000));
	PWMSet(3,LIMIT(M_3,M_3+1500,4000));
	PWMSet(2,LIMIT(M_2,M_2+1500,4000));
	PWMSet(1,LIMIT(M_1,M_1+1500,4000));
}
