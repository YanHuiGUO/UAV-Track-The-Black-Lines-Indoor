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
		loop.err_flag ++;     //ÿ�ۼ�һ�Σ�֤��������Ԥ��������û�����ꡣ
	}
	else
	{	
		loop.check_flag = 1;	//�ñ�־λ��ѭ�����������
	}
}

float Get_Cycle_T(u8 item)	
{
	Cycle_T[item][OLD] = Cycle_T[item][NOW];	//��һ�ε�ʱ��
	Cycle_T[item][NOW] = GetSysTime_us()/1000000.0f; //���ε�ʱ��
	Cycle_T[item][NEW] = ( ( Cycle_T[item][NOW] - Cycle_T[item][OLD] ) );//�����ʱ�䣨���ڣ�
	return Cycle_T[item][NEW];
}

void Duty_1ms()
{
	ANO_DT_Data_Exchange();												//����ͨ�Ŷ�ʱ����
	nRF24L01_RxPacket(Nrf_Rx_Buf);
	GetRC(Nrf_Rx_Buf);
}
float inner_loop_time;
float test[5];
void Duty_2ms()
{

	
	inner_loop_time = Get_Cycle_T(0); 						//��ȡ�ڻ�׼ȷ��ִ������
	
	test[0] = GetSysTime_us()/1000000.0f;
	
	MPU6050_Read(); 															//��ȡmpu6�ᴫ����

	MPU6050_Data_Prepare( inner_loop_time );			//mpu6�ᴫ�������ݴ���

	CTRL_1( inner_loop_time ); 								//�ڻ����ٶȿ��ơ����룺ִ�����ڣ��������ٶȣ��������ٶȣ��Ƕ�ǰ������������PWMռ�ձȡ�<����δ��װ>
	
	
	test[1] = GetSysTime_us()/1000000.0f;
}
	float outer_loop_time;
void Duty_5ms()
{
	outer_loop_time = Get_Cycle_T(2);								//��ȡ�⻷׼ȷ��ִ������
	
	test[2] = GetSysTime_us()/1000000.0f;
	
	if(	mpu6050.Acc_CALIBRATE != 1 && mpu6050.Gyro_CALIBRATE != 1)
	/*IMU������̬�����룺���ִ�����ڣ��������������ݣ�ת������ÿ�룩��������ٶȼ����ݣ�4096--1G���������ROLPITYAW��̬��*/
 	IMUupdate(0.5f *outer_loop_time,mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&Roll,&Pitch,&Yaw);
	else {ref_q[0]=1;ref_q[1]=0;ref_q[2]=0;ref_q[3]=0;}
	
	CTRL_2( outer_loop_time ); 									// �⻷�Ƕȿ��ơ����룺ִ�����ڣ������Ƕȣ�ҡ����������̬�Ƕȣ�������������ٶȡ�<����δ��װ>
	
	test[3] = GetSysTime_us()/1000000.0f;
}
u8 testmyturn=0;
void Duty_10ms()
{
	
	Usart3_Send(US_100,1);                    //���³���������
	

//**************		MS5611_Update(); 				//����ms5611��ѹ������

//**************	  ANO_AK8975_Read();			//��ȡ������������	

	
		//90��ת�����
	//GetCameraTemp[2] == RightAngle
	if(HeightReady == 1)
	{
		if(((GetCameraTemp[2] == Cross) || (GetCameraTemp[2] ==RightAngle)) && (Fly_Turn.TurnComplete ==0) &&(0==Fly_Turn.TurnStart))
		{
				Fly_Turn.TurnStart = 1;
			  Fly_Turn.TurnTimes ++ ;
		}
		if(1==Fly_Turn.TurnStart)//��TurnDirection�������˴˱�־
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
	else if(ChooseRise_Land == 1) Ultra_Ctrl(40,1);//����
	
	//�ɻ���̬������ʱ��������
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

void Duty_Loop()   					//�����������Ϊ1ms���ܵĴ���ִ��ʱ����ҪС��1ms��
{

	if( loop.check_flag == 1 )
	{
		loop_cnt = time_1ms;
		
		Duty_1ms();							//����1ms������
		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}
		if( loop.cnt_100ms >= 100 )
		{
			loop.cnt_100ms = 0;
			Duty_100ms();					//����1000ms������
		}
		
		loop.check_flag = 0;		//ѭ��������ϱ�־
	}
}

	

