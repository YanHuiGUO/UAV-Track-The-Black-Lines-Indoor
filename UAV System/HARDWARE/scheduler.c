#include "scheduler.h"
#include "include.h"
#include "time.h"
#include "mpu6050.h"
#include "led.h"
#include "imu.h"
#include "ctrl.h"
#include "height.h"
#include "nrf24l01.h"
#include "OLED.h"
//#include "ms5611.h"

s16 loop_cnt;

volatile float Cycle_T[GET_TIME_NUM][3];

enum
{
	NOW = 0,
	OLD,
	NEW,
};

loop_t loop;

void Loop_check()  //TIME INTTERRUPT
{
	loop.time++; //u16
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_100ms++;
	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //每累加一次，证明代码在预定周期内没有跑完。
	}
	else
	{	
		loop.check_flag = 1;	//该标志位在循环的最后被清零
	}
}

float Get_Cycle_T(u8 item)	
{
	Cycle_T[item][OLD] = Cycle_T[item][NOW];	//上一次的时间
	Cycle_T[item][NOW] = GetSysTime_us()/1000000.0f; //本次的时间
	Cycle_T[item][NEW] = ( ( Cycle_T[item][NOW] - Cycle_T[item][OLD] ) );//间隔的时间（周期）
	return Cycle_T[item][NEW];
}

void Duty_1ms()
{
	ANO_DT_Data_Exchange();												//数传通信定时调用
	nRF24L01_RxPacket(Nrf_Rx_Buf);
	GetRC(Nrf_Rx_Buf);
}
float inner_loop_time;
float test[5];
void Duty_2ms()
{

	
	inner_loop_time = Get_Cycle_T(0); 						//获取内环准确的执行周期
	
	test[0] = GetSysTime_us()/1000000.0f;
	
	MPU6050_Read(); 															//读取mpu6轴传感器

	MPU6050_Data_Prepare( inner_loop_time );			//mpu6轴传感器数据处理

	CTRL_1( inner_loop_time ); 								//内环角速度控制。输入：执行周期，期望角速度，测量角速度，角度前馈；输出：电机PWM占空比。<函数未封装>
	
	
	test[1] = GetSysTime_us()/1000000.0f;
}
	float outer_loop_time;
void Duty_5ms()
{
	outer_loop_time = Get_Cycle_T(2);								//获取外环准确的执行周期
	
	test[2] = GetSysTime_us()/1000000.0f;
	
	if(	mpu6050.Acc_CALIBRATE != 1 && mpu6050.Gyro_CALIBRATE != 1)
	/*IMU更新姿态。输入：半个执行周期，三轴陀螺仪数据（转换到度每秒），三轴加速度计数据（4096--1G）；输出：ROLPITYAW姿态角*/
 	IMUupdate(0.5f *outer_loop_time,mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&Roll,&Pitch,&Yaw);
	else {ref_q[0]=1;ref_q[1]=0;ref_q[2]=0;ref_q[3]=0;}
	
	CTRL_2( outer_loop_time ); 									// 外环角度控制。输入：执行周期，期望角度（摇杆量），姿态角度；输出：期望角速度。<函数未封装>
	
	test[3] = GetSysTime_us()/1000000.0f;
}
u8 testmyturn=0;
void Duty_10ms()
{
	
	Usart3_Send(US_100,1);                    //更新超声波数据
	

//**************		MS5611_Update(); 				//更新ms5611气压计数据

//**************	  ANO_AK8975_Read();			//获取电子罗盘数据	

	
		//90°转向控制
	//GetCameraTemp[2] == RightAngle
	if(HeightReady == 1)
	{
		if(((GetCameraTemp[2] == Cross) || (GetCameraTemp[2] ==RightAngle)) && (Fly_Turn.TurnComplete ==0) &&(0==Fly_Turn.TurnStart))
		{
				Fly_Turn.TurnStart = 1;
			  Fly_Turn.TurnTimes ++ ;
		}
		if(1==Fly_Turn.TurnStart)//在TurnDirection里面清了此标志
		{			
			Fly_Turn.TurnSwitch = 1;		 

		}
			if(Fly_Turn.TurnComplete ==1)	
			{	
				Fly_Turn.TurnSwitch = 0;
				Fly_Turn.TurnStart 	= 0;
				//if(testmyturn == 0)
				if(GetCameraTemp[2] == Line)
					Fly_Turn.TurnComplete = 0;
			}
	}
	
}

void Duty_20ms()
{
	if(ChooseRise_Land == 0)
	Ultra_Ctrl(40,0);
	else if(ChooseRise_Land == 1) Ultra_Ctrl(40,1);//降落
	
	//飞机姿态不正常时锁定保护
	if((MyUltraHeight > 65 && MyUltraHeight<200 )|| ABS(Roll)>15 || ABS(Pitch)>15)
		fly_ready = 0;
	

}

void Duty_50ms()
{
	if(MyUltraHeight>20)
			HeightReady = 1;
	if(MyUltraHeight >= 20 && ChooseRise_Land==0)
		Stay_Cross();
	else if(MyUltraHeight >= 20 && ChooseRise_Land==1) 
		Stay_Cross();
	else 
	{
		except_A.x  = 0;
		except_A.y  = 0;
	}
	StatusDis(fly_ready,Roll,Pitch,Yaw,MyUltraHeight,	Camera_Pos.Y_Speed,	Camera_Pos.X_Speed,GetCameraTemp[2]);
}	
void Duty_100ms()
{
	//Stay_Speed();
}	

void Duty_Loop()   					//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{

	if( loop.check_flag == 1 )
	{
		loop_cnt = time_1ms;
		
		Duty_1ms();							//周期1ms的任务
		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//周期2ms的任务
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//周期10ms的任务
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//周期20ms的任务
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//周期50ms的任务
		}
		if( loop.cnt_100ms >= 100 )
		{
			loop.cnt_100ms = 0;
			Duty_100ms();					//周期1000ms的任务
		}
		
		loop.check_flag = 0;		//循环运行完毕标志
	}
}

	

