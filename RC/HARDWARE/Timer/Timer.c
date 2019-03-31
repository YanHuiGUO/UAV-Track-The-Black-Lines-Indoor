#include "Timer.h"
#include "pwm.h"
#include "QEI.h"
#include "led.h"
#include "key_matrix.h"
#include "MyNvic.h"
#include "key2oled.h"
#include "dmp_mpu6050.h"
#include "TeeterBoard.h"
#include "ultrasonic.h"
#include  "Ano_I2c.h"
#include "imu.h"
#include "softdrive_mpu6050.h"
#include "ANO_Uart.h"
#include "Out_thr.h"
#include "imu_2.h"
#include "softdrive_mpu6050_2.h"
#include "Flag_Group.h"
#include "MyTask.h"
void  IMUupdate_2(float half_T_2,float gx_2, float gy_2, float gz_2, float ax_2, float ay_2, float az_2);
extern float ref_q_2[4];

#define 	CONTROL_PERIOD 	50
extern unsigned int thr ;
char flag_30ms=0;
char Half_Sec = 0;
u8   Cir_time;
u32  SysRunSecond;
bool Key_Start;
u32  Now_Time = 0;
u32  SysRunSecond;
extern s32 Left_Speed,Right_Speed;//��������ȡ���ٶ�ֵ


/***************************************************************************************
@f_name: void TIM5_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	 	Tim5ʱ�ӳ�ʼ����Ƶ��84M����ʱδ�õ�
@param:	 TIM_scale = ��Ƶ��, TIM_Period = ����
@return: None
****************************************************************************************/
void TIM5_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ����
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    //	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
    //TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM5, ENABLE);
}

/***************************************************************************************
@f_name: void TIM7_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	 ϵͳ�������ʱ�ӳ�ʼ����1ms��һ���жϣ�Ƶ��84M
@param:	 TIM_scale = ��Ƶ��, TIM_Period = ����
@return: None
****************************************************************************************/

void TIM7_Init(u32 TIM_scale, u32 TIM_Period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
    MyNVIC_Init(NVIC_PriorityGroup_2,2,0,TIM7_IRQn,ENABLE);	      //QEI�ٶȱջ�,���ܻ��к�վʱ��Ŀ��Ƴ�����ôͨ���жϽ���ȫ���ܵ�Ӱ�죬�����������ȼ��������ͨ�ŵ�
}

/***************************************************************************************
@f_name: void TIM7_IRQHandler(void)
@brief:	 ϵͳ�������ʱ���ж�
@param:	 None
@return: None
****************************************************************************************/
extern unsigned char tmp_buf[8];
int timetemp = 0;
void TIM7_IRQHandler(void)
{
	if (TIM7->SR&TIM_IT_Update && TIM7->DIER&TIM_IT_Update)
  {
		
			if( Cir_time >= CONTROL_PERIOD)
			{  
				Cir_time = 0;
			}
			if(Cir_time%1 == 0)
			{ 						
				
			}
			if(Cir_time%2 == 0)
			{				
//				MPU6050_Read(); 															//��ȡmpu6�ᴫ����
//				MPU6050_Data_Prepare(0.002);			            //mpu6�ᴫ�������ݴ���	
	
//			  MPU6050_Read_2(); 															//��ȡmpu6�ᴫ����
//				MPU6050_Data_Prepare_2(0.002);			            //mpu6�ᴫ�������ݴ���	
			}
			if(Cir_time%5 == 0)
			{
//				if((!mpu6050.Acc_CALIBRATE)&&(!mpu6050.Gyro_CALIBRATE ))
//       {
//	      IMUupdate(0.5f *0.005f,mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&SOFT_Roll,&SOFT_Pitch,&SOFT_Yaw);
//			 } 
//       else
//       {
//         ref_q[0] = 1;
//	       ref_q[1] = ref_q[2] = ref_q[3] = 0;
//				 ref_q_2[0] = 1;
//	       ref_q_2[1] = ref_q_2[2] = ref_q_2[3] = 0;
//       }	
				//Right_SpeedCount();
			//����ʽPID����
			//Left_SpeedCount();
			//Right_SpeedCount(); 
			//Motor_Output_Inc();
//			 Pendel_Wine.MyEncoudePlus=abEncoder_read();
//		   Pendel_Wine.Plus2Angle = 360*(float)(Pendel_Wine.MyEncoudePlus-8200)/16384;
//				if(Mode1 == MyModeChoose)
//					function1();
//				else if(Mode2 == MyModeChoose)
//					function2();
//				else if(Mode3 == MyModeChoose)
//					function3();
//			Right_AngleControl();
//			Motor_Output_Pos(RightAngleCtr.AngleCtrOut,0);
			//λ��ʽ�ٶ�PID����
			//Left_SpeedControl();
			//Right_SpeedControl();
			//Motor_Output_Pos(LeftSpeedCtr.SpeedCtrOut,RightSpeedCtr.SpeedCtrOut);
			
			//λ��ʽ�Ƕ�PID����
			//Left_AngleControl();
			//Right_AngleControl();
			//Motor_Output_Pos(LeftAngleCtr.AngleCtrOut,RightAngleCtr.AngleCtrOut);

			//ƽ��PD����
			//Balance_PWM_Out();
			}
			if(Cir_time%10 == 0)
			{							
//				if(1 == NRFSend)
//				{
//					RX_Mode();
//					NRFSend =0;
//				}
//				nRF24L01_RxPacket(Rx_Buf);
			} 
			if(Cir_time%30 == 0)
			{		
//						RX_Mode();
//						nRF24L01_RxPacket(Rx_Buf);
			} 
			if(Cir_time%50 == 0)
			{ 

				
//						TX_Mode();
//						NrfTransmit();

			}
		Cir_time++; 
		TIM7->SR = (UInt16)~TIM_IT_Update;
	}
}


