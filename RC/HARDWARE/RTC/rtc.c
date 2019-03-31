#include "rtc.h"

u32 setdelay_ms(u32 t)
{
	return(GetSysMsTime() + t - 1);
}

u32 checkdelay_ms(u32 t)
{
	return(((t - GetSysMsTime()) & 0x80000000) >> 16);
}


