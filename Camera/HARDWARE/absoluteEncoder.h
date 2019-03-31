#ifndef _ABSOLUTE_ENCODER_H
#define _ABSOLUTE_ENCODER_H
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "common.h"
#include "delay.h"
#include "rtc.h"
#define ABEncoder_CLK_H         ABEncoder_GPIO->BSRRL = ABEncoder_Pin_CLK
#define ABEncoder_CLK_L         ABEncoder_GPIO->BSRRH = ABEncoder_Pin_CLK
//#define ABEncoder_D0_H         ABEncoder_GPIO->BSRRL = ABEncoder_Pin_D0
//#define ABEncoder_D0_L         ABEncoder_GPIO->BSRRH = ABEncoder_Pin_D0
#define ABEncoder_CS_H         	ABEncoder_GPIO->BSRRL = ABEncoder_Pin_CS
#define ABEncoder_CS_L         	ABEncoder_GPIO->BSRRH = ABEncoder_Pin_CS
//#define ABEncoder_CLK_read      ABEncoder_GPIO->IDR  & ABEncoder_Pin_CLK
#define ABEncoder_D0_read      	ABEncoder_GPIO->IDR  & ABEncoder_Pin_D0
//#define ABEncoder_CS_read      ABEncoder_GPIO->IDR  & ABEncoder_Pin_CS
/***************I2C GPIO∂®“Â******************/
#define ABEncoder_GPIO				GPIOE
#define ABEncoder_Pin_CLK		GPIO_Pin_5
#define ABEncoder_Pin_D0		GPIO_Pin_2
#define ABEncoder_Pin_CS		GPIO_Pin_6

#define ABEncoder_RCC		RCC_AHB1Periph_GPIOE

extern void abEncoder_Init(void);
extern u16 abEncoder_read(void);

#endif
