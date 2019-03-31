#include "MyTask.h"
Target_Group Target;
void Key_Start_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 ���ӵ��ж���3

  /* ����EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	//�ж����ã����ȼ��Ĵ�������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//�ж�
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/******************************************************************************************
@f_name: void function1(void) 
@brief:	 ����1,��������5ms
@param:	 None
@return: None
*******************************************************************************************/
void function1(void)
{

}
/******************************************************************************************
@f_name: void function2(void) 
@brief:	 ����2,��������5ms
@param:	 None
@return: None
*******************************************************************************************/
void function2(void)
{
		
	
}
/******************************************************************************************
@f_name: void function3(void) 
@brief:	 ����3,��������5ms
@param:	 None
@return: None
*******************************************************************************************/
void function3(void)
{

}
/******************************************************************************************
@f_name: void DisToMotor_Control(int My_Xset,int My_Yset)
@brief:	 �趨�ľ���ת��Ϊ����Ŀ�����
@param:	 �����趨ֵ(������Ϊ0��)��X����Ϊ��ƫ����Ϊ��� Y����Ϊ��ƫ����Ϊ��ƫ
@return: None
*******************************************************************************************/	
float MyMotor1_AngleSet = 0;//��̨�ϵĵ��
float MyMotor2_AngleSet = 0;//������̨�ĵ��
void DisToMotor_Control(float My_Xset,float My_Yset)
{	
	float MyL1 =0;
	float MyL2 = 0;
	float MyL3 = 0;
	MyL1 = my_sqrt(MyLenth*MyLenth+(MyHeight+My_Yset)*(MyHeight+My_Yset));//���ĵ�ͼ�����ֱ�߾���		
	MyL2 = my_sqrt(MyLenth*MyLenth+MyHeight*MyHeight);
	MyL3 = my_sqrt(MyLenth*MyLenth+My_Xset*My_Xset);
	if(My_Xset>=0)
	{
		//�޷���ȷ����
//			MyL2 = my_sqrt(MyLenth*MyLenth+MyHeight*MyHeight+My_Xset*My_Xset);
//			MyMotor1_AngleSet = acos((MyL1*MyL1+MyL2*MyL2-My_Xset*My_Xset)/2*MyL1*MyL2);	
		//���Ƴ�ֱ������		
		MyMotor1_AngleSet = -fast_atan2(MyHeight+My_Yset,MyL3)*57.3f;
		MyMotor2_AngleSet = -(fast_atan2(MyHeight,MyLenth)*57.3f-fast_atan2(ABS(My_Xset),MyL1))*57.3f;

	}
	else if(My_Xset<0)
	{
		//�޷���ȷ����
//			MyL2 = my_sqrt(MyLenth*MyLenth+MyHeight*MyHeight+My_Xset*My_Xset);
//			MyMotor1_AngleSet = acos((MyL1*MyL1+MyL2*MyL2-My_Xset*My_Xset)/2*MyL1*MyL2);	
		//���Ƴ�ֱ������		
		MyMotor1_AngleSet = fast_atan2(MyHeight+My_Yset,MyL3)*57.3f;
		MyMotor2_AngleSet = -(fast_atan2(MyHeight,MyLenth)*57.3f-fast_atan2(ABS(My_Xset),MyL1))*57.3f;

	}
}
/******************************************************************************************
@f_name: void Report_Pos(int My_X,int My_Y)
@brief:	 ����
@param:	 ���������
@return: None
*******************************************************************************************/
float Zigma = 0;
void Report_Pos(int My_X,int My_Y)
{
	int Radius = 0;
	int Temp_X,Temp_Y;

	Target.X_Centra = 29;
	Target.Y_Centra = 33;
	//����ת��
	Temp_X = -(My_X-Target.X_Centra);
	Temp_Y = (My_Y-Target.Y_Centra);
	if(My_X<100&&My_Y<100)
	{
		//�����ⶨ
		Radius = my_sqrt((My_X - Target.X_Centra)*(My_X - Target.X_Centra) \
								+ (My_Y -Target.Y_Centra)*(My_Y -Target.Y_Centra));
		if(Radius>0 && Radius<=5)
			Target.Cylinder = Ten;
		else if(Radius>5 && Radius<=10)
			Target.Cylinder = Nine;
		else if(Radius>10 && Radius<=15)
			Target.Cylinder = Eight;
		else if(Radius>15 && Radius<=20)
			Target.Cylinder = Seven;
		else if(Radius>20 && Radius<=25)
			Target.Cylinder = Six;
		else if(Radius>25 && Radius<=30)
			Target.Cylinder = Five;
		else Target.Cylinder = Off;
		
		//��λ�ⶨ//�ٲ��������������޺��˻����������¾�������
		if(Radius>=0 && Radius<=5)
			Target.Directon = Middle;
	  else 
		{
			if(Temp_X>0&&Temp_Y>0)//��һ����
			{
				if(Temp_X>1)
				{
				Zigma = fast_atan2(Temp_Y,Temp_X)*57.3f;
			  if(Zigma<=22.5)
					Target.Directon = On_Right;
				else  if(Zigma<=67.5)
					Target.Directon = Right_Up;
				else  if(Zigma<90)
					Target.Directon = On_Up;
				}
				else Target.Directon = On_Up;
			}
		}
	}
	else 
	{
		Target.Cylinder = Off;
	}
}
/******************************************************************************************
@f_name: void DisToMotor_Control(int My_Xset,int My_Yset)
@brief:	 �趨�ľ���ת��Ϊ����Ŀ�����
@param:	 �����趨ֵ(������Ϊ0��)��X����Ϊ��ƫ����Ϊ��� Y����Ϊ��ƫ����Ϊ��ƫ
@return: None
*******************************************************************************************/
void CameraPID(float My_Xset,float My_Yset)
{
	float CameraX_Error,Camera_XP=0,Camera_XI,Camera_XD=0;
	float CameraY_Error,Camera_YP=0,Camera_YI,Camera_YD=0;
	
  GetDis(&Target.X_GetDis,&Target.Y_GetDis);	
	if(Target.X_GetDis<100 && Target.Y_GetDis<100)
	{
		Target.X_PreError = CameraX_Error;
		CameraX_Error = My_Xset - Target.X_GetDis;
			Camera_XP = Target.X_Kp * CameraX_Error;
			Camera_XI = Target.X_Ki * CameraX_Error;
			Camera_XD = Target.X_Kd * (CameraX_Error-Target.X_PreError);
			Target.X_OutIntegral +=Camera_XI;
			
			LIMIT(Target.X_OutIntegral,-10,10);
			Target.X_Out = Camera_XP + Target.X_OutIntegral + Camera_XD;
			
			LeftAngleCtr.AngleSet = Target.X_Out;
		
		
			Target.Y_PreError = CameraY_Error;
			CameraY_Error = My_Yset - Target.Y_GetDis;
//		if(ABS(CameraY_Error) > 10)
//			Target.Y_Ki = 0;
//		else Target.Y_Ki = -0.03;
			Camera_YP = Target.Y_Kp * CameraY_Error;
			Camera_YI = Target.Y_Ki * CameraY_Error;
			Camera_YD = Target.Y_Kd * (CameraY_Error-Target.Y_PreError);
			Target.Y_OutIntegral +=Camera_YI;
			
			LIMIT(Target.Y_OutIntegral,-10,10);
			Target.Y_Out = Camera_YP + Target.Y_OutIntegral + Camera_YD;
			
			RightAngleCtr.AngleSet = Target.Y_Out;

	}
}
/******************************************************************************************
@f_name: void GetDis(int My_Xset,int My_Yset)
@brief:	 �趨�ľ���ת��Ϊ����Ŀ�����
@param:	 �����趨ֵ(������Ϊ0��)��X����Ϊ��ƫ����Ϊ��� Y����Ϊ��ƫ����Ϊ��ƫ
@return: None
*******************************************************************************************/
void GetDis(int *My_Xdis,int *My_Ydis)
{
		if(tmp_buf[0]==0xff && tmp_buf[7]==(tmp_buf[1]+tmp_buf[6]))
				{
					*My_Xdis=(tmp_buf[1]<<8)+tmp_buf[2];
					*My_Ydis=(tmp_buf[3]<<8)+tmp_buf[4];;
					//OLED_ShowInt(0,2,(s32)((tmp_buf[5]<<8)+tmp_buf[6]));
				}
}
///******************************************************************************************
//@f_name: void EXTI3_IRQHandler(void) 
//@brief:	 �����жϣ������ش�����
//@param:	 None
//@return: None
//*******************************************************************************************/
//void EXTI3_IRQHandler(void)
//{    			    
//   if(EXTI_GetFlagStatus(EXTI_Line3))  //ȷ����EXTI3�ж�
//   {  
//	  EXTI_ClearFlag(EXTI_Line3);	   //����жϱ�־λ
//   }   
//}