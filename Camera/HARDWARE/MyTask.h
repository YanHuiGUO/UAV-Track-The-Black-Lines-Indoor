#ifndef _MYTASK_H
#define _MYTASK_H
#include "imu.h"
#include "QEI.h"
#include "common.h"
#include "absoluteEncoder.h"
#define Length 1.005//摆杆长度m
#define MyGyro 9.8//m/s2
#define MyPI 3.141592
#define My_Pendle_T (2*MyPI*my_sqrt(Length/MyGyro))//2.0121 s
#define My_Func1_T 0.005//5ms
#define MyDivAngle  (360/(My_Pendle_T/My_Func1_T)) //0.9°
typedef struct 
{
	float MyGoalAngle;
	int MyEncoudePlus;//中值8200,面对左摆起小于8200，右摆起大于8200
	float Plus2Angle;
}Pendel_Group;
enum 
{ 
	Mode1,
	Mode2,
	Mode3,
	Mode4,
	Mode5,
	Mode6,
};
enum 
{ 
	Func2_State1,
	Func2_State2,
	Func2_State3,
	Func2_State4,
	Func2_State5,
	Func2_State6,
};
extern char MyFunc1_Start,MyFunc2_Start;
extern char MyModeChoose;
extern Pendel_Group Pendel_Wine;
extern void Key_Start_Init(void);
extern void function1(void);
extern void function2(void);
extern void function3(void);
#endif 