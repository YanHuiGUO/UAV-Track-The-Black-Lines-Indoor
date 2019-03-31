#include "VoiceDis.h"
#include "exti.h"
#include "rtc.h"
#include "delay.h"
#include "VoiceDis.h"

VoiceDisClass   VoiceDisCtr;

//void VoiceDis_Init()
//{
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
//	GPIODirModeSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);

//	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
//	GPIODirModeSet(GPIO_PORTB_BASE,GPIO_PIN_1,GPIO_DIR_MODE_IN);
//	GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_1,GPIO_RISING_EDGE);
//	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_1);
//	IntEnable(INT_GPIOB);	
////	GPIO_InitTypeDef  GPIO_InitStructure;
////
////    RCC_AHB1PeriphClockCmd(VoiceDisPer, ENABLE);
////
////    GPIO_InitStructure.GPIO_Pin   = VoiceDisTrigPin;
////    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		   //配置IO方向
////    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //输出方式（推挽或者开漏）
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	   //设置上下拉
////    GPIO_Init(VoiceDisPort, &GPIO_InitStructure);
////
////	ExtiConfig(VoiceDisPort,VoiceDisEchoPin,EXTI_Trigger_Falling,GPIO_PuPd_UP);
//}

void Start_Distance_Measure()  //相邻两次测量要隔20MS以上
{
   u32 timeout=0;
   if(VoiceDisCtr.RecOk == true || VoiceDisCtr.RecOk == false )
   {
       VoiceDisCtr.RecOk = none;   //标志位置未收到任何信号
	   Trig_ON;      
	   delayus(10);
       Trig_OFF;      
	   NVIC_EnableIRQ(EXTI15_10_IRQn);
       timeout = 0;
       do
       {
            timeout++;
            if(timeout > 1000000)
            {
                VoiceDisCtr.t1 = 0;
                VoiceDisCtr.t2 = 0;
                VoiceDisCtr.Distance = 0;
                NVIC_DisableIRQ(EXTI15_10_IRQn);
                VoiceDisCtr.RecOk = false;  
                break;
            }
       }
       while(EchO_IN == 0);  //等待高电平，测量echo的高电平脉冲长度即为往返时间
       VoiceDisCtr.t1=GetSys10UsTime();  //刚到高电平记录1个时间，下降沿进入中断记录另一个时间，再根据时间差计算距离。
   }
}


void EXTI15_10_IRQHandler(void)
{
	s32 t;
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line11);
		VoiceDisCtr.t2= GetSys10UsTime();
        t=VoiceDisCtr.t2-VoiceDisCtr.t1;
      	if(t < 0)
      	{
      	  VoiceDisCtr.RecOk = false;
       	  return;
      	}
      	VoiceDisCtr.Distance=t/100000.0/2.0*350;
      	if(VoiceDisCtr.Distance > 4500)
      	{
       	  VoiceDisCtr.RecOk = false;
       	  return;
      }
      VoiceDisCtr.RecOk = true;
	}
}
