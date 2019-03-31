#include "ctrl.h"
#include "data_transfer.h"
#include "height.h"
#include "Position_Camera.h"
ctrl_t ctrl_1;
ctrl_t ctrl_2;

void PID_Para_Init()		//设置默认PID参数     //定点参数 
																							// 450 85 650  
{																							//1500 60 3000
//====================================
	ctrl_2.PID[PIDROLL].kp 	= (float)450/1000;//550
	ctrl_2.PID[PIDPITCH].kp	= (float)550/1000;
	ctrl_2.PID[PIDYAW].kp		= (float)550/1000;

	ctrl_2.PID[PIDROLL].ki	= (float)85/1000;
	ctrl_2.PID[PIDPITCH].ki	= (float)100/1000;
	ctrl_2.PID[PIDYAW].ki		= (float)100/1000;

	ctrl_2.PID[PIDROLL].kd	= (float)600/1000;//1200
	ctrl_2.PID[PIDPITCH].kd	= (float)1200/1000;
	ctrl_2.PID[PIDYAW].kd		= (float)1200/1000;

//====================================

	ctrl_1.PID[PIDROLL].kp 	= (float)1600/1000;//1150
	ctrl_1.PID[PIDPITCH].kp	= (float)1250/1000;
	ctrl_1.PID[PIDYAW].kp		= (float)1250/1000;

	ctrl_1.PID[PIDROLL].ki	= (float)60/1000;
	ctrl_1.PID[PIDPITCH].ki	= (float)60/1000;
	ctrl_1.PID[PIDYAW].ki		= (float)60/1000;

	ctrl_1.PID[PIDROLL].kd	= (float)3250/1000;//2000
	ctrl_1.PID[PIDPITCH].kd	= (float)2000/1000;
	ctrl_1.PID[PIDYAW].kd		= (float)2000/1000;

	ctrl_1.PID[PIDROLL].kdamp		= 1;
	ctrl_1.PID[PIDPITCH].kdamp	= 1;
	ctrl_1.PID[PIDYAW].kdamp		= 1;

	ctrl_1.FB = 0.20;   //外  0<fb<1	
	
	Ultra_PID_Init();
	
	Camera_Pos.Speed_X.kp = (float)40/1000;
	Camera_Pos.Speed_X.ki = (float)0/1000;
	Camera_Pos.Speed_X.kd = (float)550/1000;
	
	Camera_Pos.Speed_Y.kp = (float)30/1000;
	Camera_Pos.Speed_Y.ki = (float)0/1000;
	Camera_Pos.Speed_Y.kd =	(float)385/1000 ;
	
	
//	Speed_Pos.Speed_X.kp = 18;
//	Speed_Pos.Speed_X.ki = 5;
//	Speed_Pos.Speed_X.kd = 10.5;
//	
//	Speed_Pos.Speed_Y.kp = 18;
//	Speed_Pos.Speed_Y.ki = 5;
//	Speed_Pos.Speed_Y.kd = 10.5;
}

float To_180_degrees(float x)
{
	return (x>180?(x-360):(x<-180?(x+360):x));
}

xyz_f_t except_A = {0,0,0};
xyz_f_t ctrl_angle_offset = {-1.2,1,0};

void CTRL_2(float T)
{
//=========================== 期望角度 ========================================
//	except_A.x  = 0;
//	except_A.y  = 0;
	if( Fly_Turn.TurnSwitch  == 1 )
	{
		//except_A.z += (s16)( MAX_CTRL_YAW_SPEED *( (CH_FLI[3])/500.0f ) ) *T ;
		TurnDirection(Fly_Turn.TurnTimes*90, &except_A.z,  -50 , T);
	}
	else if(Fly_Turn.TurnSwitch == 0)
	{
		if(HeightReady == 0)
		{
			except_A.z = 0;
		}
			//except_A.z += 1 *3.14 *T *( Yaw - except_A.z );
	}

	
	
	except_A.z = To_180_degrees(except_A.z);
//==============================================================================
	

  /* 得到角度误差 */
	ctrl_2.err.x =  To_180_degrees( ctrl_angle_offset.x + except_A.x - Roll  );
	ctrl_2.err.y =  To_180_degrees( ctrl_angle_offset.y + except_A.y - Pitch );
	ctrl_2.err.z =  To_180_degrees( ctrl_angle_offset.z + except_A.z - Yaw	 );
	/* 计算角度误差权重 */
	ctrl_2.err_weight.x = ABS(ctrl_2.err.x)/ANGLE_TO_MAX_AS;
	ctrl_2.err_weight.y = ABS(ctrl_2.err.y)/ANGLE_TO_MAX_AS;
	ctrl_2.err_weight.z = ABS(ctrl_2.err.z)/ANGLE_TO_MAX_AS;
	/* 角度误差微分（跟随误差曲线变化）*/
	ctrl_2.err_d.x = 10 *ctrl_2.PID[PIDROLL].kd  *(ctrl_2.err.x - ctrl_2.err_old.x) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.x );
	ctrl_2.err_d.y = 10 *ctrl_2.PID[PIDPITCH].kd *(ctrl_2.err.y - ctrl_2.err_old.y) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.y );
	ctrl_2.err_d.z = 10 *ctrl_2.PID[PIDYAW].kd 	 *(ctrl_2.err.z - ctrl_2.err_old.z) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.z );
	/* 角度误差积分 */
	ctrl_2.err_i.x += ctrl_2.PID[PIDROLL].ki  *ctrl_2.err.x *T;
	ctrl_2.err_i.y += ctrl_2.PID[PIDPITCH].ki *ctrl_2.err.y *T;
	ctrl_2.err_i.z += ctrl_2.PID[PIDYAW].ki 	*ctrl_2.err.z *T;
	/* 角度误差积分分离 */
	ctrl_2.eliminate_I.x = Thr_Weight *CTRL_2_INT_LIMIT;
	ctrl_2.eliminate_I.y = Thr_Weight *CTRL_2_INT_LIMIT;
	ctrl_2.eliminate_I.z = Thr_Weight *CTRL_2_INT_LIMIT;
	/* 角度误差积分限幅 */
	ctrl_2.err_i.x = LIMIT( ctrl_2.err_i.x, -ctrl_2.eliminate_I.x,ctrl_2.eliminate_I.x );
	ctrl_2.err_i.y = LIMIT( ctrl_2.err_i.y, -ctrl_2.eliminate_I.y,ctrl_2.eliminate_I.y );
	ctrl_2.err_i.z = LIMIT( ctrl_2.err_i.z, -ctrl_2.eliminate_I.z,ctrl_2.eliminate_I.z );
	/* 对用于计算比例项输出的角度误差限幅 */
	ctrl_2.err.x = LIMIT( ctrl_2.err.x, -90, 90 );
	ctrl_2.err.y = LIMIT( ctrl_2.err.y, -90, 90 );
	ctrl_2.err.z = LIMIT( ctrl_2.err.z, -90, 90 );
	/* 角度PID输出 */
	ctrl_2.out.x = ctrl_2.PID[PIDROLL].kp  *( ctrl_2.err.x + ctrl_2.err_d.x + ctrl_2.err_i.x );//
	ctrl_2.out.y = ctrl_2.PID[PIDPITCH].kp *( ctrl_2.err.y + ctrl_2.err_d.y + ctrl_2.err_i.y );
	ctrl_2.out.z = ctrl_2.PID[PIDYAW].kp   *( ctrl_2.err.z + ctrl_2.err_d.z + ctrl_2.err_i.z );
	/* 记录历史数据 */	
	ctrl_2.err_old.x = ctrl_2.err.x;
	ctrl_2.err_old.y = ctrl_2.err.y;
	ctrl_2.err_old.z = ctrl_2.err.z;
	
}

xyz_f_t except_AS;

float g_old[ITEMS];
float wz_acc ,wz_speed= 0;
float wx_speed ,wy_speed=0;
float wx_acctemp,wy_acctemp=0;

float shangchu  =0;
void CTRL_1(float T)  //x roll,y pitch,z yaw
{
	
	xyz_f_t EXP_LPF_TMP;
	/* 给期望（目标）角速度 */
	EXP_LPF_TMP.x = MAX_CTRL_ASPEED *(ctrl_2.out.x/ANGLE_TO_MAX_AS);//*( (CH_FLI[0])/500.0f );//
	EXP_LPF_TMP.y = MAX_CTRL_ASPEED *(ctrl_2.out.y/ANGLE_TO_MAX_AS);//*( (CH_FLI[1])/500.0f );//
	EXP_LPF_TMP.z = MAX_CTRL_ASPEED *(ctrl_2.out.z/ANGLE_TO_MAX_AS);
	
	except_AS.x = EXP_LPF_TMP.x;//20 *3.14 *T *( EXP_LPF_TMP.x - except_AS.x );//
	except_AS.y = EXP_LPF_TMP.y;//20 *3.14 *T *( EXP_LPF_TMP.y - except_AS.y );//
  except_AS.z = EXP_LPF_TMP.z;//20 *3.14 *T *( EXP_LPF_TMP.z - except_AS.z );//
	/* 期望角速度限幅 */
	except_AS.x = LIMIT(except_AS.x, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );
	except_AS.y = LIMIT(except_AS.y, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );
	except_AS.z = LIMIT(except_AS.z, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );

	/* 角速度直接微分（角加速度），负反馈可形成角速度的阻尼（阻碍角速度的变化）*/
	ctrl_1.damp.x = ctrl_1.PID[PIDROLL].kdamp  *( mpu6050.Gyro_deg.x - g_old[A_X]) *( 0.002f/T );
	ctrl_1.damp.y = ctrl_1.PID[PIDPITCH].kdamp *(-mpu6050.Gyro_deg.y - g_old[A_Y]) *( 0.002f/T );
	ctrl_1.damp.z = ctrl_1.PID[PIDYAW].kdamp	 *(-mpu6050.Gyro_deg.z - g_old[A_Z]) *( 0.002f/T );	
	/* 角速度误差 */
	ctrl_1.err.x =  ( except_AS.x - mpu6050.Gyro_deg.x ) *(300.0f/MAX_CTRL_ASPEED);
	ctrl_1.err.y =  ( except_AS.y + mpu6050.Gyro_deg.y ) *(300.0f/MAX_CTRL_ASPEED);  //-y
	ctrl_1.err.z =  ( except_AS.z + mpu6050.Gyro_deg.z ) *(300.0f/MAX_CTRL_ASPEED);	 //-z
	/* 角速度误差权重 */
	ctrl_1.err_weight.x = ABS(ctrl_1.err.x)/MAX_CTRL_ASPEED;
	ctrl_1.err_weight.y = ABS(ctrl_1.err.y)/MAX_CTRL_ASPEED;
	ctrl_1.err_weight.z = ABS(ctrl_1.err.z)/MAX_CTRL_YAW_SPEED;
	/* 角速度微分 */
	ctrl_1.err_d.x = ( ctrl_1.PID[PIDROLL].kd  *( -10 *ctrl_1.damp.x) *( 0.002f/T ) );
	ctrl_1.err_d.y = ( ctrl_1.PID[PIDPITCH].kd *( -10 *ctrl_1.damp.y) *( 0.002f/T ) );
	ctrl_1.err_d.z = ( ctrl_1.PID[PIDYAW].kd   *( -10 *ctrl_1.damp.z) *( 0.002f/T ) );

//	ctrl_1.err_d.x += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDROLL].kd  *(ctrl_1.err.x - ctrl_1.err_old.x) *( 0.002f/T ) - ctrl_1.err_d.x);
//	ctrl_1.err_d.y += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDPITCH].kd *(ctrl_1.err.y - ctrl_1.err_old.y) *( 0.002f/T ) - ctrl_1.err_d.y);
//	ctrl_1.err_d.z += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDYAW].kd   *(ctrl_1.err.z - ctrl_1.err_old.z) *( 0.002f/T ) - ctrl_1.err_d.z);

	/* 角速度误差积分 */
	ctrl_1.err_i.x += ctrl_1.PID[PIDROLL].ki  *(ctrl_1.err.x - ctrl_1.damp.x) *T;
	ctrl_1.err_i.y += ctrl_1.PID[PIDPITCH].ki *(ctrl_1.err.y - ctrl_1.damp.y) *T;
	ctrl_1.err_i.z += ctrl_1.PID[PIDYAW].ki 	*(ctrl_1.err.z - ctrl_1.damp.z) *T;
	/* 角速度误差积分分离 */
	ctrl_1.eliminate_I.x = Thr_Weight *CTRL_1_INT_LIMIT ;
	ctrl_1.eliminate_I.y = Thr_Weight *CTRL_1_INT_LIMIT ;
	ctrl_1.eliminate_I.z = Thr_Weight *CTRL_1_INT_LIMIT ;
	/* 角速度误差积分限幅 */
	ctrl_1.err_i.x = LIMIT( ctrl_1.err_i.x, -ctrl_1.eliminate_I.x,ctrl_1.eliminate_I.x );
	ctrl_1.err_i.y = LIMIT( ctrl_1.err_i.y, -ctrl_1.eliminate_I.y,ctrl_1.eliminate_I.y );
	ctrl_1.err_i.z = LIMIT( ctrl_1.err_i.z, -ctrl_1.eliminate_I.z,ctrl_1.eliminate_I.z );
	/* 角速度PID输出 */
	ctrl_1.out.x = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.x),0,1)*except_AS.x + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDROLL].kp  *( ctrl_1.err.x + ctrl_1.err_d.x + ctrl_1.err_i.x ) );
										//*(MAX_CTRL_ASPEED/300.0f);
	ctrl_1.out.y = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.y),0,1)*except_AS.y + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDPITCH].kp *( ctrl_1.err.y + ctrl_1.err_d.y + ctrl_1.err_i.y ) );
										//*(MAX_CTRL_ASPEED/300.0f);
	ctrl_1.out.z = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.z),0,1)*except_AS.z + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDYAW].kp   *( ctrl_1.err.z + ctrl_1.err_d.z + ctrl_1.err_i.z ) );
										//*(MAX_CTRL_ASPEED/300.0f);

	if((1==Ultra_Ok) && (1==fly_ready))
	{						
		wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *0.002 ) ) ) *(reference_v.z *mpu6050.Acc.z + reference_v.x *mpu6050.Acc.x + reference_v.y *mpu6050.Acc.y - 4096 - wz_acc);
		
	  wz_speed +=(wz_acc/4096.0f) *10000 * 0.002;
		
		if( ABS( wz_speed ) > 1350 )
		{
				wz_speed = 0;
		}
	}
	
	if(fly_ready == 1)
	{
		wx_acctemp += ( 1 / ( 1 + 1 / ( 20 *3.14f *0.002 ) ) )*(a_final[0] - wx_acctemp);
		wy_acctemp += ( 1 / ( 1 + 1 / ( 20 *3.14f *0.002 ) ) )*(a_final[1] - wy_acctemp);
		
		wx_acctemp = LIMIT(wx_acctemp,-650,650);
		wy_acctemp = LIMIT(wy_acctemp,-650,650);

		wx_speed += (wx_acctemp/4096.0f) * 0.002*10000;
		wy_speed += (wy_acctemp/4096.0f) * 0.002*10000;
		
			if( ABS( wx_speed ) > 1000 )
			{
				wx_speed = 0;
			}
			if(ABS(wy_speed)  > 1000 )
			{
				wy_speed= 0;
			}
	}
	
	Thr_Ctrl(T);// 油门控制
	
	All_Out(ctrl_1.out.x,ctrl_1.out.y,ctrl_1.out.z);


	ctrl_1.err_old.x = ctrl_1.err.x;
	ctrl_1.err_old.y = ctrl_1.err.y;
	ctrl_1.err_old.z = ctrl_1.err.z;

	g_old[A_X] =  mpu6050.Gyro_deg.x ;
	g_old[A_Y] = -mpu6050.Gyro_deg.y ;
	g_old[A_Z] = -mpu6050.Gyro_deg.z ;

}


float thr_value;
u8 Thr_Low;
float Thr_Weight;
extern float Tool_Thr_value;//上位机给的油门值
void Thr_Ctrl(float T)
{
	static float thr;
	static float Thr_tmp;
	//thr = 500 + CH_FLI[2];
	thr = 400;
	
//	Thr_tmp += 10 *3.14f *T *(thr/400.0f - Thr_tmp);
//	Thr_Weight = LIMIT(Thr_tmp,0,1);    
 	Thr_Weight = 1;
	
	if( thr < 100 )
	{
		Thr_Low = 1;
	}
	else
	{
		Thr_Low = 0;
	}
	thr_value = thr + height_ctrl_out;
	thr_value = LIMIT(thr_value,0,10 *MAX_THR *MAX_PWM/100);
}


float motor[4];
float posture_value[MAXMOTORS];
float curve[MAXMOTORS];
s16 motor_out[4];

void All_Out(float out_roll,float out_pitch,float out_yaw)
{
	
	u8 i;
	float posture_value[MAXMOTORS];
  float curve[MAXMOTORS];
	
	out_yaw = LIMIT(out_yaw,-300,300);//转向限幅
	
	posture_value[0] = - out_roll + out_pitch + out_yaw ;
	posture_value[1] = + out_roll + out_pitch - out_yaw ;
	posture_value[2] = + out_roll - out_pitch + out_yaw ;
	posture_value[3] = - out_roll - out_pitch - out_yaw ;
	
	for(i=0;i<4;i++)
	{
		posture_value[i] = LIMIT(posture_value[i], -1000,1000 );
	}
	
	curve[0] = (0.55f + 0.45f *ABS(posture_value[0])/1000.0f) *posture_value[0] ;
	curve[1] = (0.55f + 0.45f *ABS(posture_value[0])/1000.0f) *posture_value[1] ;
	curve[2] = (0.55f + 0.45f *ABS(posture_value[0])/1000.0f) *posture_value[2] ;
	curve[3] = (0.55f + 0.45f *ABS(posture_value[0])/1000.0f) *posture_value[3] ;
	
  motor[0] = thr_value + Thr_Weight *curve[0] ;
	motor[1] = thr_value + Thr_Weight *curve[1] ;
	motor[2] = thr_value + Thr_Weight *curve[2] ;
	motor[3] = thr_value + Thr_Weight *curve[3] ;
	/* 是否解锁fly_ready */
	if(fly_ready)
	{
		if( !Thr_Low )  //油门拉起
		{
			for(i=0;i<4;i++)
			{
				motor[i] = LIMIT(motor[i], (10 *READY_SPEED),(10*MAX_PWM));
			}
		}
		else						//油门低
		{
			for(i=0;i<4;i++)
			{
				motor[i] = LIMIT(motor[i], 0,(10*MAX_PWM) );
			}
		}
	}
	else
	{
		//锁定后一直清零积分项，防止积分累计炸机
		ctrl_1.err_i.x = 0;
		ctrl_1.err_i.y = 0;
		ctrl_1.err_i.z = 0;
		
		ctrl_2.err_i.x = 0;
		ctrl_2.err_i.y = 0;
		ctrl_2.err_i.z = 0;
		
		ctrl_1.out.x  = 0;
		ctrl_1.out.y  = 0;
		ctrl_1.out.z  = 0;
		
		ctrl_2.out.x  = 0;
		ctrl_2.out.y  = 0;
		ctrl_2.out.z  = 0;
		
		for(i=0;i<4;i++)
		{
			motor[i] = 0;
		}
	}
	/* xxx */
	motor_out[0] = (s16)(motor[0]);  
	motor_out[1] = (s16)(motor[1]);	 
	motor_out[2] = (s16)(motor[2]);
	motor_out[3] = (s16)(motor[3]);

	
	SetPwm(motor_out,0,1000); //1000
	
}


