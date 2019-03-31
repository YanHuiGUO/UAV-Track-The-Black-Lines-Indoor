#include "RC.h"
#include "Timer.h"
#include "delay.h"
RC_Struct RC;
uint32_t frame_data=0;    /* 一帧数据缓存 */
uint8_t  frame_cnt=0;
uint8_t  frame_flag=0;    /* 一帧数据接收完成标志 */
/*************************************************************************************************
@f_name: void TIM14_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	作为遥控器测电平宽度的定时器
@param:	 Prescaler  分频值  Period 周期
@return: None
***************************************************************************************************/

void TIM14_Init(u32 TIM_scale, u32 TIM_Period)//TIM_Period为16位的数 84
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//计数器重装值
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM14, ENABLE);
}

/********************************************************************************************************************************************
@f_name: void RC_Init(void)
@brief:	 红外遥控硬件模块初始化
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(RC_PORT, &GPIO_InitStructure);//初始化GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);//PB11 连接到中断线11

  /* 配置EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE11
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	//中断设置，优先级寄存器划分
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//中断
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


///*************************************************************************************************
//@f_name: void EXTI15_10_IRQHandler(void)
//@brief:	 超声波输入外部中断
//@param:	 None
//@return: None
//***************************************************************************************************/



/*
 * 帧数据有4个字节，第一个字节是遥控的ID，第二个字节是第一个字节的反码
 * 第三个数据是遥控的真正的键值，第四个字节是第三个字节的反码
 */
unsigned int IrDa_Process(void)
{
  uint8_t first_byte, sec_byte, tir_byte, fou_byte;  
  
  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  
  /* 记得清标志位 */
  frame_flag = 0;
  
  if( (first_byte==(uint8_t)~sec_byte) && (first_byte == IRDA_ID) )
  {
    if( tir_byte == (u8)~fou_byte )
      return tir_byte;
  }
  
  return 0;   /* 错误返回 */
}

/***********************/
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while(GPIO_ReadInputDataBit(RC_PORT,RC_SINGAL))
  {
    time ++;
    delay_us(20);     // 延时 20us
    if(time == 250)
      return time;   // 超时溢出   
  }
  return time;
}

/**********************/
void	RC_LOOP(void)
{	  
	uint8_t key_val;
    if( frame_flag == 1 ) /* 一帧红外数据接收完成 */
    {
      key_val = IrDa_Process();         
      /* 不同的遥控器面板对应不同的键值，需要实际测量 */
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



