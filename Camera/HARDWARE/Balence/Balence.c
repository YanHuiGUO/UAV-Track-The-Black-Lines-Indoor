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
	   Angle.GraAngle=((float)Angle.AccZ-(float)Angle.GRAVITY_OFFSET)/Angle.GRAVITY_ANGLE_RATIO;//������ٶȼ�����Ƕ����趨�ǶȵĲ�ֵ
	   Angle.GyroAngleSpeed=((float)Angle.AngleSpeed-(float)Angle.GYROSCOPE_OFFSET)/Angle.GYROSCOPE_ANGLE_RATIO;//���������ǵĽ��ٶ�
	   Angle.FinalAngle=Angle.GyroAngleIntegera;//��������Ƕȸ�ֵ
	   fDeltaValue=(Angle.GraAngle-Angle.FinalAngle)/Angle.GRAVITY_ADJUST_TIME_CONSTANT;//�����ǻ���ֵ����ٶȶԱ����ֵ
	   Angle.GyroAngleIntegera += (Angle.GyroAngleSpeed+fDeltaValue)/Angle.GYROSCOPE_ANGLE_SIGMA_FREQUENCY;//�����ں�ֵ
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
