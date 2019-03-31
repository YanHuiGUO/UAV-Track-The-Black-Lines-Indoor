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
@brief:	 �����������ظߵ�ƽ�ļ�ʱ��
@param:	 Prescaler  ��Ƶֵ  Period ����
@return: None
***************************************************************************************************/

void TIM13_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ����
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
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
@brief:	 ������Ӳ��ģ���ʼ��
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
	
	GPIO_InitStructure.GPIO_Pin = ULTRA_TRIG;   //LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);                   //��ʼ��GPIO
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);

	GPIO_InitStructure.GPIO_Pin = ULTRA_ECHO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE2 ���ӵ��ж���2

  /* ����EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
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

/*************************************************************************************************
@f_name: void Ranging(void)
@brief:	 �����������źţ� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
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
@brief:	 �����ⲿ�ж�
@param:	 None
@return: None
***************************************************************************************************/

void EXTI15_10_IRQHandler(void)
{    			    
	unsigned int  pulse_time = 0;
  unsigned int  leader_code_flag = 0; /* �������־λ�������������ʱ����ʾһ֡���ݿ�ʼ */
  unsigned int  irda_data = 0;        /* �����ݴ�λ */
//   if(EXTI_GetFlagStatus(EXTI_Line13))  //ȷ����EXTI6�ж�
//   {  
//     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //����Ǹߵ�ƽ������
//	  {
//         TIM_SetCounter(TIM13,0);          //װ�س�ֵ   
//         TIM_Cmd(TIM13, ENABLE);    //ʹ��ʹ��TIM2	     
//	  }

//	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //����ǵ͵�ƽ����
//	  {
//	     TIM_Cmd(TIM13, DISABLE);    //ʹ��ʹ��TIM2
//			 ultra_count=TIM_GetCounter(TIM13);
//			 Ultra_Distance();
//	  }
//	  EXTI_ClearFlag(EXTI_Line13);	   //����жϱ�־λ
//   }  
//	else  
		if(EXTI_GetITStatus(EXTI_Line11) != RESET ) /* ȷ���Ƿ������EXTI Line�ж� */
	{   
    
//						 if(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)==RESET)  //����Ǹߵ�ƽ������
//			{
//					 TIM_SetCounter(TIM13,0);          //װ�س�ֵ   
//					 TIM_Cmd(TIM13, ENABLE);    //ʹ��ʹ��TIM2	     
//			}

//			if(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)==SET)  //����ǵ͵�ƽ����
//			{
//				 TIM_Cmd(TIM13, DISABLE);    //ʹ��ʹ��TIM2
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
        if( GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL)== SET )        /* ֻ�����ߵ�ƽ��ʱ�� */
     {       
       pulse_time = Get_Pulse_Time();
        /* >=5ms ���������ź� �����ָ��Ż���������ʱ��Ҳ��break����while(1)ѭ�� */
        if( pulse_time >= 250 )                
        {	
					break;
						RC.Error = 1 ; /* �����źű�־�������зɿ�״̬�ı�*/
        }
        
        if(pulse_time>=200 && pulse_time<250)         /* ���ǰ��λ 4ms~4.5ms */
        {
          leader_code_flag = 1;
        }
        else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
        {
          irda_data = 0;
        }
        else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms��1ms~2ms */
        {
          irda_data =1 ; 
        }        
        else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms��2ms~4ms */
        {/* �����룬�ڵڶ����жϳ��� */
          frame_flag = 1;               /* һ֡���ݽ������ */
          frame_cnt++;                  /* ����������1 */
          isr_cnt ++;                   /* ���ж�һ�μ�1 */
             break  ;                 /* ����while(1)ѭ�� */
        }
        
        if( leader_code_flag == 1 )
        {/* �ڵ�һ���ж������ */
          frame_data <<= 1;
          frame_data += irda_data;
          frame_cnt = 0;
          isr_cnt = 1;
        }
      }      
    // while(1)   
		EXTI_ClearITPendingBit(EXTI_Line11);     //����жϱ�־λ
    //LED2_TOGGLE;
	}  
}
}


//void EXTI3_IRQHandler(void)
//{    			    
//   if(EXTI_GetFlagStatus(EXTI_Line3))  //ȷ����EXTI6�ж�
//   {  
//     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //����Ǹߵ�ƽ������
//	  {
//         TIM_SetCounter(TIM13,0);          //װ�س�ֵ   
//         TIM_Cmd(TIM13, ENABLE);    //ʹ��ʹ��TIM2	     
//	  }

//	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //����ǵ͵�ƽ����
//	  {
//	     TIM_Cmd(TIM13, DISABLE);    //ʹ��ʹ��TIM2
//			 ultra_count=TIM_GetCounter(TIM13);
//			 Ultra_Distance();
//	  }
//	  EXTI_ClearFlag(EXTI_Line3);	   //����жϱ�־λ
//   }   
//}
/*************************************************************************************************
@f_name: void Ultra_Distance(void)
@brief:	 ������������㣬ȡ10��ƽ������λΪCM
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




