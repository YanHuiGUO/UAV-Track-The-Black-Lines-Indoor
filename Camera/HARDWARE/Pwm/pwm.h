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

#define Drive_Harf_Bridge true//ʹ��bts��������������ʱΪtrue��һ���ź�����ͬ����������ӣ���ô������ֻ����Ӳ���������������������
//���������ԽǵĹ��ӣ���Ҫ�����·�����PWM���źţ�ÿ·����һ�ԶԽǹ��ӣ���ô��·�źŵ����������������

//PWM�źŲ�����ʼ��
void PWM_Init(u32 frequency);
void PWMSet(u8 channel,u16 duty);

extern MyPWMManageClass MyPWMManage;

#endif


