#ifndef _Balence_H
#define _Balence_H
#include "sys.h"
#include "adc.h"
#include "UartLink.h"
typedef struct 
{
  float    GraAngle;//加速度计输出角度
	float    FinalAngle;//最终输出角度
	float    GyroAngleIntegera;//融合算法输出角度值
	float    GyroAngleSpeed;//陀螺仪角速度输出值
	float    GRAVITY_ADJUST_TIME_CONSTANT;//调整系数，一般为2，3，4，不过可能是小数，看实际调试
	float    GYROSCOPE_ANGLE_RATIO;//需要调额参数
  unsigned int    AccZ;//加速度计采样回的AD值
  unsigned int    GRAVITY_OFFSET;//加速度计直立静止下的AD值
  float    GRAVITY_ANGLE_RATIO;//加速度计旋转90度的AD值
  unsigned int    AngleSpeed;//陀螺仪采回的角速度的AD值
  unsigned int    GYROSCOPE_OFFSET;//陀螺仪静止状态下输出的AD值
  unsigned int    GYROSCOPE_ANGLE_SIGMA_FREQUENCY;//陀螺仪积分频率，1ms=1000
  
}Anglestr;  // 角度参数
extern Anglestr Angle;
extern void Angle_init(void);//参数赋值
extern void AngleCalculate(void);
#endif 
