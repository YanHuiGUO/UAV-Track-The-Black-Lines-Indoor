#include "ultrasonic.h"
#include "Timer.h"
#include "delay.h"
#include "UartLink.h"
#include "RC.h"
#include "softdrive_mpu6050.h"
int ultra_count = 0;
int distance = 0;
int Ultra_Dis[5];
uint8_t isr_cnt;
/*************************************************************************************************
@f_name: void TIM3_Init(u16 Prescaler,u16 Period)
@brief:	 作超声波返回高电平的计时器
@param:	 Prescaler  分频值  Period 周期
@return: None
***************************************************************************************************/

void TIM13_Init(u32 TIM_scale, u32 TIM_Period)//TIM_Period为16位的数
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//计数器重装值
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
    //	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
    //TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM13, ENABLE);
}

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 超声波硬件模块初始化
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHB1PeriphClockCmd(ULTRA_CLK, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = ULTRA_TRIG;   //LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);                   //初始化GPIO
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);

	GPIO_InitStructure.GPIO_Pin = ULTRA_ECHO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);//初始化GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE2 连接到中断线2

  /* 配置EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
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

/*************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波发射信号， 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 None
@return: None
***************************************************************************************************/

void Ultra_Ranging(void)
{
		TRIG_Send=1;
		delay_us(20);
		TRIG_Send=0; 	
}

/*************************************************************************************************
@f_name: void EXTI15_10_IRQHandler(void)
@brief:	 输入外部中断
@param:	 None
@return: None
***************************************************************************************************/

void EXTI15_10_IRQHandler(void)
{    			    
	unsigned int  pulse_time = 0;
  unsigned int  leader_code_flag = 0; /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
  unsigned int  irda_data = 0;        /* 数据暂存位 */
//   if(EXTI_GetFlagStatus(EXTI_Line13))  //确定是EXTI6中断
//   {  
//     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //如果是高电平进来的
//	  {
//         TIM_SetCounter(TIM13,0);          //装载初值   
//         TIM_Cmd(TIM13, ENABLE);    //使能使能TIM2	     
//	  }

//	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //如果是低电平进来
//	  {
//	     TIM_Cmd(TIM13, DISABLE);    //使能使能TIM2
//			 ultra_count=TIM_GetCounter(TIM13);
//			 Ultra_Distance();
//	  }
//	  EXTI_ClearFlag(EXTI_Line13);	   //清除中断标志位
//   }  
//	else  
		if(EXTI_GetITStatus(EXTI_Line11) != RESET ) /* 确保是否产生了EXTI Line中断 */
	{   
    
//						 if(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)==RESET)  //如果是高电平进来的
//			{
//					 TIM_SetCounter(TIM13,0);          //装载初值   
//					 TIM_Cmd(TIM13, ENABLE);    //使能使能TIM2	     
//			}

//			if(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)==SET)  //如果是低电平进来
//			{
//				 TIM_Cmd(TIM13, DISABLE);    //使能使能TIM2
//					pulse_time = TIM_GetCounter(TIM13);
//				   pulse_time /=40 ;
//			}
//		if(!RC.first_In_EXit)
//			{
//				RC.first_In_EXit = 1;
//		
//			}
    while(1 )
	{
		if(RC.Error)
		{
			RC.Error = 0;
			break;
		}
        if( GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)== SET )        /* 只测量高电平的时间 */
     {       
       pulse_time = Get_Pulse_Time();
        /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
        if( pulse_time >= 250 )                
        {	
					break;
						RC.Error = 1 ; /* 错误信号标志，不进行飞控状态改变*/
        }
        
        if(pulse_time>=200 && pulse_time<250)         /* 获得前导位 4ms~4.5ms */
        {
          leader_code_flag = 1;
        }
        else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
        {
          irda_data = 0;
        }
        else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms：1ms~2ms */
        {
          irda_data =1 ; 
        }        
        else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms：2ms~4ms */
        {/* 连发码，在第二次中断出现 */
          frame_flag = 1;               /* 一帧数据接收完成 */
          frame_cnt++;                  /* 按键次数加1 */
          isr_cnt ++;                   /* 进中断一次加1 */
             break  ;                 /* 跳出while(1)循环 */
        }
        
        if( leader_code_flag == 1 )
        {/* 在第一次中断中完成 */
          frame_data <<= 1;
          frame_data += irda_data;
          frame_cnt = 0;
          isr_cnt = 1;
        }
      }      
    // while(1)   
		EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
    //LED2_TOGGLE;
	}  
}
}


//void EXTI3_IRQHandler(void)
//{    			    
//   if(EXTI_GetFlagStatus(EXTI_Line3))  //确定是EXTI6中断
//   {  
//     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //如果是高电平进来的
//	  {
//         TIM_SetCounter(TIM13,0);          //装载初值   
//         TIM_Cmd(TIM13, ENABLE);    //使能使能TIM2	     
//	  }

//	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //如果是低电平进来
//	  {
//	     TIM_Cmd(TIM13, DISABLE);    //使能使能TIM2
//			 ultra_count=TIM_GetCounter(TIM13);
//			 Ultra_Distance();
//	  }
//	  EXTI_ClearFlag(EXTI_Line3);	   //清除中断标志位
//   }   
//}
/*************************************************************************************************
@f_name: void Ultra_Distance(void)
@brief:	 超声波距离计算，取10次平均，单位为CM
@param:	 None
@return: None
***************************************************************************************************/


void Ultra_Distance(void)
{		
		distance=(float)(17.3641*ultra_count/10);
//		Ultra_Dis[4] = distance;
//		for(i = 0;i < 4;i++)
//		{
//			Ultra_Dis[i] = Ultra_Dis[i+1];
//			distance += Ultra_Dis[i];
//		}		
//		distance = (u16)distance/5;
	distance = KalmanFilter(distance) * cos((float)SOFT_Roll/57.295779)*cos((float)SOFT_Pitch/57.295779);
	//	push(10,distance);
}




