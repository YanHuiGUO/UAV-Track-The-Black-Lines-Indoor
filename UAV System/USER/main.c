#include "include.h"
int16_t MyTest[4]={0};
int main(void)
{
	All_Init();	
	while(1)
	{
		Duty_Loop(); 
		//������û�
    //SetPwm(MyTest,0,1000);
	}
}
