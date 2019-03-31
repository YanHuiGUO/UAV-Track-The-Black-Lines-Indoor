#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f4xx.h"
#include "timer.h"

#define  GetSysMsTime()      (TIM5->CNT / 1000)
#define  GetSys100UsTime()   (TIM5->CNT / 100)
#define  GetSys10UsTime()	   (TIM5->CNT / 10)
#define  GetSysUsTime()	     (TIM5->CNT) 

extern u32 setdelay_ms(u32 t);
extern u32 checkdelay_ms(u32 t);

#endif

