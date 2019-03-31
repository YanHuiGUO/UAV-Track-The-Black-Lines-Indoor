#include "Balence.h"
Anglestr Angle;
unsigned int flag=0;
void Angle_init(void)
{		Angle.GRAVITY_OFFSET=2000;
	  Angle.GYROSCOPE_OFFSET=2756;
	  Angle.GRAVITY_ANGLE_RATIO=11.71;
	  Angle.GRAVITY_ADJUST_TIME_CONSTANT=0.07;
	  Angle.GYROSCOPE_ANGLE_SIGMA_FREQUENCY=500;
	  Angle.GYROSCOPE_ANGLE_RATIO=0.15;
}
void AngleCalculate(void)
{    float fDeltaValue;
	   Angle.AccZ=Get_Adc(5);
	   Angle.AngleSpeed=Get_Adc(6);
	   Angle.GraAngle=((float)Angle.AccZ-(float)Angle.GRAVITY_OFFSET)/Angle.GRAVITY_ANGLE_RATIO;//计算加速度计输出角度与设定角度的差值
	   Angle.GyroAngleSpeed=((float)Angle.AngleSpeed-(float)Angle.GYROSCOPE_OFFSET)/Angle.GYROSCOPE_ANGLE_RATIO;//计算陀螺仪的角速度
	   Angle.FinalAngle=Angle.GyroAngleIntegera;//最终输出角度赋值
	   fDeltaValue=(Angle.GraAngle-Angle.FinalAngle)/Angle.GRAVITY_ADJUST_TIME_CONSTANT;//陀螺仪积分值与加速度对比误差值
	   Angle.GyroAngleIntegera += (Angle.GyroAngleSpeed+fDeltaValue)/Angle.GYROSCOPE_ANGLE_SIGMA_FREQUENCY;//波形融合值
	   #if(0)
	  // if((Angle.GraAngle>10)&&(flag==0))
	//	 {	 
	  // push(0,Angle.FinalAngle);
	  // push(1,Angle.GraAngle);
	//	 push(3,Angle.AngleSpeed);
			 
	  // Send_NData();
	//		flag=1;
	//	 }
	//	 else if(flag==1)
	//	 {	push(2,Angle.GraAngle);
//push(4,Angle.AngleSpeed);
	//		  Send_NData();
	//		  flag=2;
  //   }
	   #endif
}
