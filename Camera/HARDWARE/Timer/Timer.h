#ifndef _Timer_H_
#define _Timer_H_
#include "common.h"
extern void TIM5_Init(u32 TIM_scale, u32 TIM_Period);//TIM_PeriodΪ16λ����

extern void TIM7_Init(u32 TIM_scale, u32 TIM_Period);
void TIM7_IRQHandler(void);
#endif
