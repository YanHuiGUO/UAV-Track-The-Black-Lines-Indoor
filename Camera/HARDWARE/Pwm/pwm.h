#ifndef _PWM_H_
#define _PWM_H_

#include "Common.h"

typedef struct 
{
  UInt32 frequency;
  UInt32 maxDuty;
  UInt32 deadTime;
  UInt16 pidPeriod;
  Double wheelPara0;
  Double wheelPara1;
}MyPWMManageClass;

#define Drive_Harf_Bridge true//使用bts这样的驱动半桥时为true，一个信号驱动同侧的两个管子，那么死区就只能由硬件产生，软件不产生死区
//否则驱动对角的管子，就要输出两路反向的PWM的信号，每路驱动一对对角管子，那么两路信号的死区可以软件产生

//PWM信号产生初始化
void PWM_Init(u32 frequency);
void PWMSet(u8 channel,u16 duty);

extern MyPWMManageClass MyPWMManage;

#endif


