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
/*****************************设置数据类型*****************************/
typedef 	unsigned 	char		u8;	  //无符号型
typedef     u8                      Byte;
typedef     u8                      bool;
typedef 	unsigned 	short int	u16;
//typedef 	unsigned 	long  int	u32; 

typedef 	signed char			    s8;	  //有符号型
typedef 	short  int				s16;
//typedef 	long   int				s32;
/*****************************与C#兼容*********************************/
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
	
//================系统===================
#define MAXMOTORS 		(4)		//电机数量
#define GET_TIME_NUM 	(5)		//???
#define CH_NUM 				(8) 	//接收机通道数量


#define ANO_DT_USE_USART2 	//开启串口2数传功能
#define ANO_DT_USE_USB_HID	//开启飞控USBHID连接上位机功能
//=======================================
/***************中断优先级******************/
#define NVIC_GROUP NVIC_PriorityGroup_3		//中断分组选择
#define NVIC_PWMIN_P			1		//接收机采集
#define NVIC_PWMIN_S			1
#define NVIC_TIME_P       2		//???
#define NVIC_TIME_S       0
#define NVIC_UART_P				5		//???
#define NVIC_UART_S				1
#define NVIC_UART2_P			3		//串口2中断
#define NVIC_UART2_S			1
/***********************************************/

//================传感器===================
#define ACC_INIT_ADJ 		0				//???

#define OFFSET_AV_NUM 	50			//计算偏移值时进行多少次平均
#define FILTER_NUM 			10			//低通滤波计算多少次历史值

#define TO_ANGLE 				0.06103f //0.061036 //   4000/65536  +-2000   ???

#define FIX_GYRO_Y 			1.02f		//陀螺仪Y轴固有补偿
#define FIX_GYRO_X 			1.02f		//陀螺仪X轴固有补偿

#define TO_M_S2 				0.23926f   	//   980cm/s2    +-8g   980/4096
#define ANGLE_TO_RADIAN 0.01745329f //*0.01745 = /57.3	角度转弧度

#define MAX_ACC  4096.0f			//+-8G		加速度计量程
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

//================控制=====================
#define MAX_CTRL_ANGLE			25.0f										//遥控能达到的最大角度
#define ANGLE_TO_MAX_AS 		30.0f										//???
#define CTRL_2_INT_LIMIT 		0.5f *MAX_CTRL_ANGLE		//???

#define MAX_CTRL_ASPEED 	 	300.0f									//???
#define MAX_CTRL_YAW_SPEED 	200.0f									//YAW的最大调整速度
#define CTRL_1_INT_LIMIT 		0.5f *MAX_CTRL_ASPEED		//???


#define MAX_PWM				100			///%	最大PWM输出为100%油门
#define MAX_THR       80 			///%	油门通道最大占比80%，留20%给控制量
#define READY_SPEED   20			///%	解锁后电机转速20%油门
//=========================================


#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
