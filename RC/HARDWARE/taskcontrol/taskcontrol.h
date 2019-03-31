#ifndef _taskcontrol_H
#define _taskcontrol_H
#include "pwm.h"
#include "Balence.h"
typedef struct
{  float Kp;
	 float Ki;
	 float Kd;
	 float error;
	 float d_error;
	 float dd_error;
	 float set;
	 float output;
	
} positionPIDstr;
extern positionPIDstr positionPID;
extern void positionPID_init(void);
extern void positionPID_con(float now);

#endif
