#include "MyTask.h"
Pendel_Group Pendel_Wine;
char MyModeChoose = Mode3;
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
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 连接到中断线3

  /* 配置EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	//中断设置，优先级寄存器划分
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//中断
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/******************************************************************************************
@f_name: void function1(void) 
@brief:	 功能1,调用周期5ms
@param:	 None
@return: None
*******************************************************************************************/
char MyFunc1_Start = 0;
void function1(void)
{
//	if(ABS(ref.g.x)>0.3)
//	{
//		MyFunc1_Start = 1;
//	}	
	if(1==MyFunc1_Start)
	{
		RightAngleCtr.AngleSet +=MyDivAngle;
		Right_AngleControl();
		Motor_Output_Pos(RightAngleCtr.AngleCtrOut,0);
	}
}
/******************************************************************************************
@f_name: void function2(void) 
@brief:	 功能2,调用周期5ms
@param:	 None
@return: None
*******************************************************************************************/
#define MotorToGoal_T 0.005
#define MotorCircel_Count (0.5/MotorToGoal_T)
int MyFunc2_count = 0;
char MyFunc2_Start = 0;//启动信号
char MyFunc2_State = 0;//状态标识
void function2(void)
{
		switch(MyFunc2_State)
		{
			case Func2_State1: 	
													RightAngleCtr.AngleSet = - Pendel_Wine.Plus2Angle;
													if(1==MyFunc2_Start)
													{
														MyFunc2_State = Func2_State2;					
													}
													else MyFunc2_State = Func2_State1;
													break;	
//			case Func2_State2:													
//													RightAngleCtr.AngleSet = 	-10-ABS(ref.g.x)*20;	
//												
//													if(ABS(RightAngleCtr.AngleSet)>=30)
//													MyFunc2_State = Func2_State3;	 
//													else 		MyFunc2_State = Func2_State2;
//													break;	
			case Func2_State2:	
													RightAngleCtr.AngleSet = 0;	 
											  	break;
		}
	
}
/******************************************************************************************
@f_name: void function3(void) 
@brief:	 功能3,调用周期5ms
@param:	 None
@return: None
*******************************************************************************************/
#define Angle2Rad (MyPI/180)
#define Rad2Angle (180/MyPI)
float My_Motor_Angel = 0;
void function3(void)
{
	if(Pendel_Wine.Plus2Angle>=0)//面对摆，右摆起，电机目标值为负
	{
		My_Motor_Angel = 90+ABS(Pendel_Wine.Plus2Angle)-Rad2Angle*fast_atan2(150-100.5*my_sin(ABS( Pendel_Wine.Plus2Angle)*Angle2Rad),100-100.5*my_cos(ABS( Pendel_Wine.Plus2Angle)*Angle2Rad));//输入是弧度制
  	RightAngleCtr.AngleSet = -(My_Motor_Angel);
		if(Pendel_Wine.Plus2Angle>=45)
  	RightAngleCtr.AngleSet = -(My_Motor_Angel);
	}
	else if(Pendel_Wine.Plus2Angle<0)//面对摆，左摆起，电机目标值为正
	{
		My_Motor_Angel = ABS(Pendel_Wine.Plus2Angle)-Rad2Angle*fast_atan2(100-100.5*my_cos(ABS( Pendel_Wine.Plus2Angle)*Angle2Rad),150+100.5*my_sin(ABS( Pendel_Wine.Plus2Angle)*Angle2Rad));//输入是弧度制
		RightAngleCtr.AngleSet = My_Motor_Angel;
		if(Pendel_Wine.Plus2Angle<=-45)
  	RightAngleCtr.AngleSet = (My_Motor_Angel);
	}
}
/******************************************************************************************
@f_name: void EXTI3_IRQHandler(void) 
@brief:	 按键中断（上升沿触发）
@param:	 None
@return: None
*******************************************************************************************/
void EXTI3_IRQHandler(void)
{    			    
   if(EXTI_GetFlagStatus(EXTI_Line3))  //确定是EXTI3中断
   {  
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==1)
				MyFunc2_Start = 1;
	  EXTI_ClearFlag(EXTI_Line3);	   //清除中断标志位
   }   
}