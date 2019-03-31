#ifndef _INCLUDE_H_
#define _INCLUDE_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "init.h"
#include "led.h"
#include "time.h"
#include "usart.h"
#include "i2c_soft.h"
#include "mpu6050.h"
#include "parameter.h"
#include "pwm_out.h"
#include "imu.h"
#include "data_transfer.h"
#include "ctrl.h"
#include "scheduler.h"
#include "Ultra.h"
#include "height.h"
#include "nrf24l01.h"
#include "Position_Camera.h"
#include "OLED.h"
/*****************************������������*****************************/
typedef 	unsigned 	char		u8;	  //�޷�����
typedef     u8                      Byte;
typedef     u8                      bool;
typedef 	unsigned 	short int	u16;
//typedef 	unsigned 	long  int	u32; 

typedef 	signed char			    s8;	  //�з�����
typedef 	short  int				s16;
//typedef 	long   int				s32;
/*****************************��C#����*********************************/
typedef     u8                      byte;
typedef     s16                     Int16;
typedef     double                  Double;
typedef     u16                     UInt16;
typedef     u32                     uint;
typedef     u32                     UInt32;
typedef     s32                     Int32;
/**********************************************************************/

#define ERROR       0
#define SUCCESS     1

#define true        1      //
#define false       0      //
	
//================ϵͳ===================
#define MAXMOTORS 		(4)		//�������
#define GET_TIME_NUM 	(5)		//???
#define CH_NUM 				(8) 	//���ջ�ͨ������


#define ANO_DT_USE_USART2 	//��������2��������
#define ANO_DT_USE_USB_HID	//�����ɿ�USBHID������λ������
//=======================================
/***************�ж����ȼ�******************/
#define NVIC_GROUP NVIC_PriorityGroup_3		//�жϷ���ѡ��
#define NVIC_PWMIN_P			1		//���ջ��ɼ�
#define NVIC_PWMIN_S			1
#define NVIC_TIME_P       2		//???
#define NVIC_TIME_S       0
#define NVIC_UART_P				5		//???
#define NVIC_UART_S				1
#define NVIC_UART2_P			3		//����2�ж�
#define NVIC_UART2_S			1
/***********************************************/

//================������===================
#define ACC_INIT_ADJ 		0				//???

#define OFFSET_AV_NUM 	50			//����ƫ��ֵʱ���ж��ٴ�ƽ��
#define FILTER_NUM 			10			//��ͨ�˲�������ٴ���ʷֵ

#define TO_ANGLE 				0.06103f //0.061036 //   4000/65536  +-2000   ???

#define FIX_GYRO_Y 			1.02f		//������Y����в���
#define FIX_GYRO_X 			1.02f		//������X����в���

#define TO_M_S2 				0.23926f   	//   980cm/s2    +-8g   980/4096
#define ANGLE_TO_RADIAN 0.01745329f //*0.01745 = /57.3	�Ƕ�ת����

#define MAX_ACC  4096.0f			//+-8G		���ٶȼ�����
#define TO_DEG_S 500.0f      	//T = 2ms  ???

enum
{
 A_X = 0,
 A_Y ,
 A_Z ,
 G_Y ,
 G_X ,
 G_Z ,
 TEM ,
 ITEMS ,
};
//=========================================

//================����=====================
#define MAX_CTRL_ANGLE			25.0f										//ң���ܴﵽ�����Ƕ�
#define ANGLE_TO_MAX_AS 		30.0f										//???
#define CTRL_2_INT_LIMIT 		0.5f *MAX_CTRL_ANGLE		//???

#define MAX_CTRL_ASPEED 	 	300.0f									//???
#define MAX_CTRL_YAW_SPEED 	200.0f									//YAW���������ٶ�
#define CTRL_1_INT_LIMIT 		0.5f *MAX_CTRL_ASPEED		//???


#define MAX_PWM				100			///%	���PWM���Ϊ100%����
#define MAX_THR       80 			///%	����ͨ�����ռ��80%����20%��������
#define READY_SPEED   20			///%	��������ת��20%����
//=========================================


#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
