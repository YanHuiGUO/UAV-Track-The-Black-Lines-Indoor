#include "include.h"
int16_t MyTest[4]={0};
int main(void)
{
	All_Init();	
	while(1)
	{
		Duty_Loop(); 
		//检查电机好坏
    //SetPwm(MyTest,0,1000);
	}
}
