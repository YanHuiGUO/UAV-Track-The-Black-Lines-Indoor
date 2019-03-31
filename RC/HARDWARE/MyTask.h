#ifndef _MYTASK_H
#define _MYTASK_H
#include "imu.h"
#include "QEI.h"
#include "common.h"
#include "absoluteEncoder.h"
#include "key2oled.h"
#include "nrf24l01.h"
#define MyLenth 300//3M
#define MyHeight 50//0.5M
enum //环数枚举
{
	Ten,
	Nine,
	Eight,
	Seven,
	Six,
	Five,
	Off
};
enum //方位枚举
{
	Middle,//中心
	On_Up,//正上
	On_Down,//正下
	On_Left,//正左
	On_Right,//正右
	Left_Up,//左上
	Left_Down,//左下
	Right_Up,//右上
	Right_Down//右下
};
typedef struct 
{
	//靶心的坐标
	int Y_Centra;
	int X_Centra;
	//移动距离设置
	int Y_DisSet;
	int X_DisSet;
	
	int  X_GetDis;
	int  Y_GetDis;	
	
	float  X_Kp;
	float  X_Ki;
	float  X_Kd; 
	float  X_PreError;
	float  X_Out;
	float  X_OutIntegral;
	
	float  Y_Kp;
	float  Y_Ki;
	float  Y_Kd; 
	float  Y_PreError;
	float  Y_Out;
	float  Y_OutIntegral;
	
	char 	 Cylinder;//环数标记
	char 	 Directon;//方位标记
}Target_Group;
extern Target_Group Target;
extern void Report_Pos(int My_X,int My_Y);
extern void GetDis(int *My_Xdis,int *My_Ydis);
extern void DisToMotor_Control(float My_Xset,float My_Yset);
extern void CameraPID(float My_Xset,float My_Yset);
extern void Key_Start_Init(void);
extern void function1(void);
extern void function2(void);
extern void function3(void);
#endif 