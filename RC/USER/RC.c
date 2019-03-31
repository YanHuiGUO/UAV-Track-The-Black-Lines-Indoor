#include "RC.h"
#include "Timer.h"
#include "delay.h"
RC_Struct RC;
uint32_t frame_data=0;    /* һ֡���ݻ��� */
uint8_t  frame_cnt=0;
uint8_t  frame_flag=0;    /* һ֡���ݽ�����ɱ�־ */
/*************************************************************************************************
@f_name: void TIM14_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	��Ϊң�������ƽ��ȵĶ�ʱ��
@param:	 Prescaler  ��Ƶֵ  Period ����
@return: None
***************************************************************************************************/

void TIM14_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ���� 84
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM14, ENABLE);
}

/********************************************************************************************************************************************
@f_name: void RC_Init(void)
@brief:	 ����ң��Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void RC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHB1PeriphClockCmd(RC_CLK, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//
	
	

	GPIO_InitStructure.GPIO_Pin = RC_SINGAL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(RC_PORT, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);//PB11 ���ӵ��ж���11

  /* ����EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE11
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	//�ж����ã����ȼ��Ĵ�������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//�ж�
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


///*************************************************************************************************
//@f_name: void EXTI15_10_IRQHandler(void)
//@brief:	 �����������ⲿ�ж�
//@param:	 None
//@return: None
//***************************************************************************************************/



/*
 * ֡������4���ֽڣ���һ���ֽ���ң�ص�ID���ڶ����ֽ��ǵ�һ���ֽڵķ���
 * ������������ң�ص������ļ�ֵ�����ĸ��ֽ��ǵ������ֽڵķ���
 */
unsigned int IrDa_Process(void)
{
  uint8_t first_byte, sec_byte, tir_byte, fou_byte;  
  
  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  
  /* �ǵ����־λ */
  frame_flag = 0;
  
  if( (first_byte==(uint8_t)~sec_byte) && (first_byte == IRDA_ID) )
  {
    if( tir_byte == (u8)~fou_byte )
      return tir_byte;
  }
  
  return 0;   /* ���󷵻� */
}

/***********************/
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL))
  {
    time ++;
    delay_us(20);     // ��ʱ 20us
    if(time == 250)
      return time;   // ��ʱ���   
  }
  return time;
}

/**********************/
void	RC_LOOP(void)
{	  
	uint8_t key_val;
    if( frame_flag == 1 ) /* һ֡�������ݽ������ */
    {
      key_val = IrDa_Process();         
      /* ��ͬ��ң��������Ӧ��ͬ�ļ�ֵ����Ҫʵ�ʲ��� */
      switch( key_val )
      {
        case 0:
//        LED1_TOGGLE;  
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n Error \r\n");
				;
        break;
        
        case 162:
//        LED1_TOGGLE;  
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n POWER \r\n");
        break;
        
        case 226:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n MENU \r\n");
        break;
        
        case 34:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n TEST \r\n");
        break;
        
        case 2:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n + \r\n");
				;
        break;
        
        case 194:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n RETURN \r\n");
				;
        break;
        
        case 224:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n |<< \r\n");
				;
        break;
        
        case 168:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n > \r\n");
				;
        break;
        
        case 144:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n >>| \r\n");
				;
        break;
        
        case 104:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 0 \r\n");
				;
        break;
        
        case 152:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n - \r\n");
				;
        break;
        
        case 176:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n C \r\n");
				;
        break;
        
        case 48:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 1 \r\n");
				;
        break;
        
        case 24:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 2 \r\n");
				;
        break;
        
        case 122:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 3 \r\n");
				;
        break;
        
        case 16:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 4 \r\n");
				;
        break;
        
        case 56:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 5 \r\n");
				;
        break;
        
        case 90:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 6 \r\n");
				;
        break;
        
          case 66:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 7 \r\n");
					;
        break;
        
        case 74:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 8 \r\n");
				;
        break;
        
        case 82:
//          LED1_TOGGLE;
//          printf("\r\n key_val=%d \r\n",key_val);
//          printf("\r\n 9 \r\n");
				;
        break;
        
        default:       
        break;
      }      
    }
	}



