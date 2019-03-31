#ifndef _LED_H_
#define	_LED_H_

#include "stm32f4xx.h"

#define LED1_ON         ANO_GPIO_LED->BSRRL = ANO_Pin_LED1   //H
#define LED1_OFF        ANO_GPIO_LED->BSRRH = ANO_Pin_LED1		//L

/***************LED GPIO∂®“Â******************/
#define ANO_RCC_LED			RCC_AHB1Periph_GPIOA
#define ANO_GPIO_LED		GPIOA
#define ANO_Pin_LED1		GPIO_Pin_2
/*********************************************/

void LED_Init(void);

#endif

