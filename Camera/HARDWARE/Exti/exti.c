#include "exti.h"
#include "key_matrix.h"

//u32  GPIO_Pinx[] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,
//				   GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};

u32  RCC_AHB1Periph_GPIOx[] = {RCC_AHB1Periph_GPIOA,RCC_AHB1Periph_GPIOB,RCC_AHB1Periph_GPIOC,RCC_AHB1Periph_GPIOD,RCC_AHB1Periph_GPIOE};
 
void ExtiConfig(GPIO_TypeDef* GPIOx,u32 Pin,EXTITrigger_TypeDef Trigger,GPIOPuPd_TypeDef PuPd)
{
	u8 GPIOn;
	byte k = 0,EXTI_PinSource = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	if(GPIOx == GPIOA)
	{
		GPIOn = 0;
	}
	else if(GPIOx == GPIOB)
	{
		GPIOn = 1;
	}
	else if(GPIOx == GPIOC)
	{
		GPIOn = 2;
	}
	else if(GPIOx == GPIOD)
	{
		GPIOn = 3;
	}
	else if(GPIOx == GPIOE)
	{
		GPIOn = 4;
	}

	while(k<16)
	{
		if(1<<k == Pin)
		{
			EXTI_PinSource = k;
			break;
		}
		k++;
	}
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << GPIOn, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//注意要打开SYSCFG时钟

	GPIO_InitStructure.GPIO_Pin  = Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOx, &GPIO_InitStructure);


    SYSCFG_EXTILineConfig(GPIOn , EXTI_PinSource);    //407使用的配置函数
    EXTI_InitStructure.EXTI_Line = Pin;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = Trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
