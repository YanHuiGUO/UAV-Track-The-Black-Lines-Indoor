#ifndef _EXTI_H_
#define _EXTI_H_

#include "common.h"

void ExtiConfig(GPIO_TypeDef* GPIOx,u32 Pin,EXTITrigger_TypeDef Trigger,GPIOPuPd_TypeDef PuPd);

#endif
