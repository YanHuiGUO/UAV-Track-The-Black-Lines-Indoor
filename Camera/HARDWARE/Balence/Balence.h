#ifndef _Balence_H
#define _Balence_H
#include "sys.h"
#include "adc.h"
#include "UartLink.h"
typedef struct 
{
  float    GraAngle;//���ٶȼ�����Ƕ�
	float    FinalAngle;//��������Ƕ�
	float    GyroAngleIntegera;//�ں��㷨����Ƕ�ֵ
	float    GyroAngleSpeed;//�����ǽ��ٶ����ֵ
	float    GRAVITY_ADJUST_TIME_CONSTANT;//����ϵ����һ��Ϊ2��3��4������������С������ʵ�ʵ���
	float    GYROSCOPE_ANGLE_RATIO;//��Ҫ�������
  unsigned int    AccZ;//���ٶȼƲ����ص�ADֵ
  unsigned int    GRAVITY_OFFSET;//���ٶȼ�ֱ����ֹ�µ�ADֵ
  float    GRAVITY_ANGLE_RATIO;//���ٶȼ���ת90�ȵ�ADֵ
  unsigned int    AngleSpeed;//�����ǲɻصĽ��ٶȵ�ADֵ
  unsigned int    GYROSCOPE_OFFSET;//�����Ǿ�ֹ״̬�������ADֵ
  unsigned int    GYROSCOPE_ANGLE_SIGMA_FREQUENCY;//�����ǻ���Ƶ�ʣ�1ms=1000
  
}Anglestr;  // �ǶȲ���
extern Anglestr Angle;
extern void Angle_init(void);//������ֵ
extern void AngleCalculate(void);
#endif 
