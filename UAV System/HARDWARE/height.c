#include "include.h"
#define Ultra_Ctrl_T     20     //ms
#define RiseTime         1  //4s
#define LandTime         3
#define R_ULTRA_SPEED(x) 	((float)(x/RiseTime)/((float)(1000/Ultra_Ctrl_T)))//每秒上升X/RiseTime CM
#define L_ULTRA_SPEED(x) 	((float)(x/LandTime)/((float)(1000/Ultra_Ctrl_T)))
#define ULTRA_MAX_HEIGHT  100     // CM
#define ULTRA_MinHeight		10//超声波离地的最低距离
float exp_height_speed=0;
float height_ctrl_out = 0;
extern float wz_speed;
u8 HeightReady=0;
u8 ChooseRise_Land = 0;
height_pid ultra_pid;	

//高度PID//使用超声波//20ms调用一次
void Ultra_Ctrl(float set,u8 dir)
{
	if((1==Ultra_Ok) && (1==fly_ready))
	{
		ultra_pid.GetHeight = MyUltraHeight;	 
		
		if(0 == dir)
		{
				set = LIMIT(set,0,ULTRA_MAX_HEIGHT);
				
				exp_height_speed = R_ULTRA_SPEED(set);
				
				if(ultra_pid.ExceptHeight<set)
				ultra_pid.ExceptHeight += exp_height_speed;
				else ultra_pid.ExceptHeight =set;
				
				if(ultra_pid.ExceptHeight>ULTRA_MAX_HEIGHT)
					ultra_pid.ExceptHeight = ULTRA_MAX_HEIGHT;
				
				ultra_pid.Err_old = ultra_pid.Err;
				
				ultra_pid.Err = ultra_pid.ExceptHeight - ultra_pid.GetHeight;
			
				ultra_pid.Err_p  = ultra_pid.Kp * ultra_pid.Err;
				
				ultra_pid.Err_i += ultra_pid.Ki * ultra_pid.Err;
				
				ultra_pid.Err_d  = ultra_pid.Kd * (-0.1*wz_speed+0.9*(ultra_pid.Err - ultra_pid.Err_old));
				
				ultra_pid.Pid_Out = ultra_pid.Err_p + ultra_pid.Err_i + ultra_pid.Err_d;
				
				ultra_pid.Pid_Out = LIMIT(ultra_pid.Pid_Out,0,400);
				
				height_ctrl_out = ultra_pid.Pid_Out;
			}
		else if(1 == dir)
		{
				exp_height_speed = L_ULTRA_SPEED(set);	
				
				if(ultra_pid.ExceptHeight>ULTRA_MinHeight)
					ultra_pid.ExceptHeight -= exp_height_speed;
				
				else 
				{
					ultra_pid.ExceptHeight =ULTRA_MinHeight;
					fly_ready = 0;	
				}	
				
				ultra_pid.ExceptHeight = LIMIT(ultra_pid.ExceptHeight,ULTRA_MinHeight,set);
	
				ultra_pid.Err_old = ultra_pid.Err;
				
				ultra_pid.Err = ultra_pid.ExceptHeight - ultra_pid.GetHeight;
			
				ultra_pid.Err_p  = ultra_pid.Kp * ultra_pid.Err;
				
				ultra_pid.Err_i += ultra_pid.Ki * ultra_pid.Err;
				
				ultra_pid.Err_i = LIMIT(	ultra_pid.Err_i,-400,400);
				
				ultra_pid.Err_d  = ultra_pid.Kd * (-0.1*wz_speed+0.9*(ultra_pid.Err - ultra_pid.Err_old));
				
				ultra_pid.Pid_Out = ultra_pid.Err_p + ultra_pid.Err_i + ultra_pid.Err_d;
				
				ultra_pid.Pid_Out = LIMIT(ultra_pid.Pid_Out,0,400);
				
				height_ctrl_out = ultra_pid.Pid_Out;
				

		}
	}
	else //清零积分
	{
		ultra_pid.Err_p = 0;
		ultra_pid.Err_i = 0;
		ultra_pid.Err_d = 0;
		ultra_pid.Pid_Out = 0;
		height_ctrl_out = ultra_pid.Pid_Out;
	}
}
void Ultra_PID_Init(void)
{
	ultra_pid.Kp = (float)4500/1000;
	ultra_pid.Ki = (float)155/1000;
	ultra_pid.Kd = (float)3000/1000;
}

