#include "Position_Camera.h"
Position_Group Camera_Pos;
Position_Group Speed_Pos;
float Wx_Speed_0,Wy_Speed_0;	
void Stay_Cross(void)
{
	Camera_Pos.Except_Xspeed = 100;
	Camera_Pos.Except_Yspeed = 100;
	Wx_Speed_0 += ( 1 / ( 1 + 1 / ( 0.05f *3.14f *0.1f ) ) ) *( wx_speed - Wx_Speed_0  );
	Wy_Speed_0 += ( 1 / ( 1 + 1 / ( 0.05f *3.14f *0.1f ) ) ) *( wy_speed - Wy_Speed_0  ) ;
	
	//H_MID对应Pitch，W_MID对应Roll
	
	if(Roll<=0 && GetCameraTemp[1]>0)
	Camera_Pos.X_Speed = GetCameraTemp[1] + LIMIT(Roll*6,-100,0);
	else if (GetCameraTemp[1]>0)Camera_Pos.X_Speed = GetCameraTemp[1] + LIMIT(Roll*5.8,0,100);
	else if(GetCameraTemp[1]==0)  Camera_Pos.X_Speed =0;
	
	if(Pitch<=0 && GetCameraTemp[0]>0)
	Camera_Pos.Y_Speed = GetCameraTemp[0] + LIMIT(Pitch*5.8,-100,0);
	else if(GetCameraTemp[0]>0) Camera_Pos.Y_Speed = GetCameraTemp[0] + LIMIT(Pitch*4.5,0,100);	
	else if(GetCameraTemp[0]==0)Camera_Pos.Y_Speed = 0;
		
	Speed_Pos.X_Speed = Wx_Speed_0;
	Speed_Pos.Y_Speed = Wy_Speed_0;	
	
	//X轴位置控制,输出Roll，异号
	Camera_Pos.Speed_Err_old.x = Camera_Pos.Speed_Err.x ;
	Camera_Pos.Speed_Err.x =  Camera_Pos.Except_Xspeed - Camera_Pos.X_Speed;
	Camera_Pos.Speed_Err_d.x =(0.05*(-mpu6050.Gyro_deg.x) + 0.95*(Camera_Pos.Speed_Err.x-Camera_Pos.Speed_Err_old.x));
	Camera_Pos.Speed_Err_i.x += Camera_Pos.Speed_X.ki*Camera_Pos.Speed_Err.x ;
	
	Camera_Pos.Speed_Err_i.x = LIMIT(Camera_Pos.Speed_Err_i.x,-4,4);
	
	Camera_Pos.Speed_Pid_Out.x = Camera_Pos.Speed_X.kp*Camera_Pos.Speed_Err.x + Camera_Pos.Speed_Err_i.x + Camera_Pos.Speed_X.kd*Camera_Pos.Speed_Err_d.x;
	
	Camera_Pos.Speed_Pid_Out.x = LIMIT(Camera_Pos.Speed_Pid_Out.x,-8,8);
	
	//Y轴位置控制，输出Pitch，同号
	Camera_Pos.Speed_Err_old.y = Camera_Pos.Speed_Err.y ;
	Camera_Pos.Speed_Err.y =  Camera_Pos.Except_Yspeed - Camera_Pos.Y_Speed;
	Camera_Pos.Speed_Err_d.y = (0.05*mpu6050.Gyro_deg.y + 0.95*(Camera_Pos.Speed_Err.y-Camera_Pos.Speed_Err_old.y));
	Camera_Pos.Speed_Err_i.y += Camera_Pos.Speed_Y.ki*Camera_Pos.Speed_Err.y ;
	
	Camera_Pos.Speed_Err_i.y = LIMIT(Camera_Pos.Speed_Err_i.y,-4,4);
	
	Camera_Pos.Speed_Pid_Out.y = Camera_Pos.Speed_Y.kp*Camera_Pos.Speed_Err.y + Camera_Pos.Speed_Err_i.y + Camera_Pos.Speed_Y.kd*Camera_Pos.Speed_Err_d.y;
	
	Camera_Pos.Speed_Pid_Out.y = LIMIT(Camera_Pos.Speed_Pid_Out.y,-8,8);
	//except_A.x = 0;
	//except_A.y = 0;

//	if(	ABS(Camera_Pos.Speed_Err.x)<15)
		  except_A.y  =  -1.2;

	
		if((Fly_Turn.TurnSwitch == 0) || (Fly_Turn.TurnComplete ==1)) except_A.x =  Camera_Pos.Speed_Pid_Out.x;
		
		else if( Fly_Turn.TurnSwitch == 1)
		{
			except_A.x = -1.5;
			except_A.y = 4;
		}
		
//	else 	
//	if(	ABS(Camera_Pos.Speed_Err.y)<15)
//		except_A.y  = 0;
//	else  
	//except_A.y =  Camera_Pos.Speed_Pid_Out.y;	
}	

void Stay_Speed(void)
{
	Wx_Speed_0 += ( 1 / ( 1 + 1 / ( 0.05f *3.14f *0.1f ) ) ) *( wx_speed - Wx_Speed_0  );
	Wy_Speed_0 += ( 1 / ( 1 + 1 / ( 0.05f *3.14f *0.1f ) ) ) *( wy_speed - Wy_Speed_0  ) ;
	
	Speed_Pos.X_Speed = Wx_Speed_0;
	Speed_Pos.Y_Speed = Wy_Speed_0;	
	

	//X轴速度控制,输出Roll，异号
	Speed_Pos.Speed_Err.x = Speed_Pos.Speed_X.kp/1000 * (Speed_Pos.Except_Xspeed - Speed_Pos.X_Speed);
	Speed_Pos.Speed_Err_d.x = Speed_Pos.Speed_X.kd/1000 * (-a_final[0]);
	Speed_Pos.Speed_Err_i.x += Speed_Pos.Speed_X.ki/1000 * (Speed_Pos.Except_Xspeed - Speed_Pos.X_Speed) ;
	
	Speed_Pos.Speed_Err_i.x = LIMIT(Speed_Pos.Speed_Err_i.x,-2.5,2.5);
	
	Speed_Pos.Speed_Pid_Out.x = Speed_Pos.Speed_Err.x + Speed_Pos.Speed_Err_i.x + Speed_Pos.Speed_Err_d.x;
	
	Speed_Pos.Speed_Pid_Out.x = LIMIT(Speed_Pos.Speed_Pid_Out.x,-5,5);
	
	//Y轴速度控制，输出Pitch，同号
	Speed_Pos.Speed_Err.y = Speed_Pos.Speed_Y.kp/1000 * (Speed_Pos.Except_Yspeed - Speed_Pos.Y_Speed);
	Speed_Pos.Speed_Err_d.y = Speed_Pos.Speed_Y.kd/1000 * (-a_final[1]);
	Speed_Pos.Speed_Err_i.y += Speed_Pos.Speed_Y.ki/1000 * (Speed_Pos.Except_Yspeed - Speed_Pos.Y_Speed) ;
	
	Speed_Pos.Speed_Err_i.y = LIMIT(Speed_Pos.Speed_Err_i.y,-2.5,2.5);
	
	Speed_Pos.Speed_Pid_Out.y = Speed_Pos.Speed_Err.y + Speed_Pos.Speed_Err_i.y + Speed_Pos.Speed_Err_d.y;
	
	Speed_Pos.Speed_Pid_Out.y = LIMIT(Speed_Pos.Speed_Pid_Out.y,-5,5);
	
	except_A.x = -Speed_Pos.Speed_Pid_Out.x;
	except_A.y =  Speed_Pos.Speed_Pid_Out.y;	
}	
/***************************************************************************************
@f_name: void TurnDirection(float ToAngle,float exceptz, float TurnSpeed ,float T)
@brief:	转角函数
@param:	 目标角度值;启动标志
@return: None
****************************************************************************************/
TurnGroup Fly_Turn;	
float YawTemp = 0;
//TurnSpeed 正负决定转的方向
//转速计算公式：1s 90°， (90/(1/0.005))/MAX_CTRL_YAW_SPEED/T*100
void TurnDirection(float ToAngle,float *exceptz, float TurnSpeed ,float T)
{

	if( Fly_Turn.TurnComplete == 0)
		YawTemp +=  (float)MAX_CTRL_YAW_SPEED*((float)TurnSpeed/100)*T;
	
	if(ABS(YawTemp) >= ABS(ToAngle))
		Fly_Turn.TurnComplete = 1;
	else Fly_Turn.TurnComplete = 0;
	
	*exceptz = YawTemp;
}