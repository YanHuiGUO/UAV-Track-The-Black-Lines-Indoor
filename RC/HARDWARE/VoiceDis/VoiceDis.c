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
////    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		   //����IO����
////    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //�����ʽ��������߿�©��
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	   //����������
////    GPIO_Init(VoiceDisPort, &GPIO_InitStructure);
////
////	ExtiConfig(VoiceDisPort,VoiceDisEchoPin,EXTI_Trigger_Falling,GPIO_PuPd_UP);
//}

void Start_Distance_Measure()  //�������β���Ҫ��20MS����
{
   u32 timeout=0;
   if(VoiceDisCtr.RecOk == true || VoiceDisCtr.RecOk == false )
   {
       VoiceDisCtr.RecOk = none;   //��־λ��δ�յ��κ��ź�
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
       while(EchO_IN == 0);  //�ȴ��ߵ�ƽ������echo�ĸߵ�ƽ���峤�ȼ�Ϊ����ʱ��
       VoiceDisCtr.t1=GetSys10UsTime();  //�յ��ߵ�ƽ��¼1��ʱ�䣬�½��ؽ����жϼ�¼��һ��ʱ�䣬�ٸ���ʱ��������롣
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
