#ifndef __LED_H
#define __LED_H
#include "sys.h"	

//LED端口定义
#define LED0_PIN    GPIO_Pin_0
#define LED1_PIN    GPIO_Pin_1
	
#define	LED0	    PBout(0)	       						// LED0 = 0,设置低，灯灭,LED0 = !LED0;(翻转)
#define	LED1	    PBout(1)	      					  // LED1 = 0,设置低，灯灭,LED1 = !LED1;(翻转)

#define	LED0_ON	  GPIOB->BSRRL |= LED0_PIN	  // LED0亮
#define	LED1_ON	  GPIOB->BSRRL |= LED1_PIN    // LED1亮
#define	LED0_OFF	GPIOB->BSRRH |= LED0_PIN	  // LED0灭
#define	LED1_OFF	GPIOB->BSRRH |= LED1_PIN	  // LED1灭

void LED_Init(void);           //初始化	

#endif
