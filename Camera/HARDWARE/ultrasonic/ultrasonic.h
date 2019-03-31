#ifndef _ultrasonic_H
#define _ultrasonic_H

#include "common.h"
#include "imu.h"
#define ULTRA_PORT			GPIOE
#define ULTRA_CLK       RCC_AHB1Periph_GPIOE
#define ULTRA_TRIG			GPIO_Pin_2
#define ULTRA_ECHO			GPIO_Pin_3

#define TRIG_Send  PEout(2)
#define ECHO_Reci  PEin(3)

void TIM13_Init(u32 TIM_scale, u32 TIM_Period);
void Ultran_Init(void);
void Ultra_Ranging(void);
void Ultra_Distance(void);
float KalmanFilter( float);
extern int distance ;
extern int ultra_count ;
#endif
