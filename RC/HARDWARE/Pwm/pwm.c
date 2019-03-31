#include "pwm.h"

MyPWMManageClass MyPWMManage;

/***************************************************************************************
@f_name: void PWM_Init(UInt32 frequency)
@brief:	 PWM�źŲ�����ʼ��
@param:	 frequency = PWMƵ��
@return: None
****************************************************************************************/

void PWM_Init(UInt32 frequency)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    uint32_t period;
    MyPWMManage.frequency = frequency;
    period = 84000000UL/frequency; //������ģ
    MyPWMManage.maxDuty = period;
    MyPWMManage.deadTime = 0;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);//LED1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);//LED2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);//LED3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);//LED3

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = period;      //TIM4�����Ƶ��Ϊ84M��PWMƵ��Ϊ84000000/8400=10KHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���Ķ���ģʽƵ�����������½�ģʽ��һ��

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

/***************************************************************************************
@f_name: void PWMSet(u8 channel,u16 duty)
@brief:	 PWM���ã�����0~17000����Ӧpwm��С�����ֵ
				 ���֣�  1>2��ת   1<2��ת
				 ����:   3>4��ת   4>3��ת
@param:	 channel = ͨ��, duty = PWMֵ
@return: None
****************************************************************************************/

void PWMSet(u8 channel,u16 duty)
{
   u16 pwm;
   pwm = duty * MyPWMManage.maxDuty/20000;
   if(channel == 1)
   {
   	   TIM2->CCR1 = pwm;
   }
   else if(channel == 2)
   {
   	   TIM2->CCR2 = pwm;
   }
   else if(channel == 3)
   {
   	   TIM2->CCR3 = pwm;
   }
   else if(channel == 4)
   {
   	   TIM2->CCR4 = pwm;
   }
}



