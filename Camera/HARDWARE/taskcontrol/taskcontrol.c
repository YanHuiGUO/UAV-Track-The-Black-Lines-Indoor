#include "taskcontrol.h"

positionPIDstr positionPID;
void positionPID_init()
{    positionPID.Kp=100;
	   positionPID.Ki=50;
	   positionPID.Kd=0;
	   positionPID.set=0;
}

void positionPID_con(float now)
{		
	  positionPID.error=now-positionPID.set;
	  positionPID.d_error=positionPID.error;
	  positionPID.dd_error=positionPID.d_error;
    positionPID.output+=	positionPID.error*positionPID.Kp+positionPID.d_error*positionPID.Ki;
}
