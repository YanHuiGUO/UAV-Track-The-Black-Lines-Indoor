#include "led.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(ANO_RCC_LED,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Pin   = ANO_Pin_LED1;
	GPIO_Init(ANO_GPIO_LED, &GPIO_InitStructure);
	
	GPIO_SetBits(ANO_GPIO_LED, ANO_Pin_LED1);		
}

