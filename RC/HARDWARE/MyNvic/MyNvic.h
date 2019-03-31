#ifndef _MY_NVIC_H_
#define _MY_NVIC_H_

#include "Common.h"

extern void MyNVIC_Init(UInt32 group,UInt32 PreempPriority,UInt32 SubPriority,Byte IRQCha,FunctionalState ENorDis);

#endif
