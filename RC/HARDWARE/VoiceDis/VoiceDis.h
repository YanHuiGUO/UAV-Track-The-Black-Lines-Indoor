#ifndef _VOICE_DIS_
#define _VOICE_DIS_

#include "common.h"

#define VoiceDisPer 	RCC_AHB1Periph_GPIOD
#define VoiceDisPort 	GPIOD

#define VoiceDisTrigPin   pin10
#define VoiceDisEchoPin   pin11

#define Trig_ON       VoiceDisPort->BSRRL =  VoiceDisTrigPin
#define Trig_OFF      VoiceDisPort->BSRRH =  VoiceDisTrigPin

#define EchO_IN       (VoiceDisPort->IDR & VoiceDisEchoPin)

typedef struct
{
	volatile float Distance;
	volatile u32   t1;
	volatile u32   t2;
	volatile bool  RecOk;
}VoiceDisClass;

extern VoiceDisClass   VoiceDisCtr;

#endif
