#ifndef __LED_H
#define __LED_H
#include "sys.h"	

//LED�˿ڶ���
#define LED0_PIN    GPIO_Pin_5
#define LED1_PIN    GPIO_Pin_1
	
#define	LED0	    PAout(5)	       						// LED0 = 0,���õͣ�����,LED0 = !LED0;(��ת)
#define	LED1	    PBout(1)	      					  // LED1 = 0,���õͣ�����,LED1 = !LED1;(��ת)

#define	LED0_ON	  GPIOA->BSRRL |= LED0_PIN	  // LED0��
#define	LED1_ON	  GPIOB->BSRRL |= LED1_PIN    // LED1��
#define	LED0_OFF	GPIOA->BSRRH |= LED0_PIN	  // LED0��
#define	LED1_OFF	GPIOB->BSRRH |= LED1_PIN	  // LED1��

void LED_Init(void);           //��ʼ��	

#endif
