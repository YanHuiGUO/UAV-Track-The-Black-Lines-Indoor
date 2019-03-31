#include "key2oled.h"
#include "OLED.h"
#include "key_matrix.h"
#include "MyTask.h"
typedef void (* Task_Show)(void);

//函数指针数组，按键与OLED显示
Task_Show Task_ShowCarry[] = {A_Show,B_Show,C_Show,D_Show,
															E_Show,F_Show,G_Show,H_Show,
															I_Show,J_Show,K_Show,L_Show,
															M_Show,N_Show,O_Show,P_Show};

/*************************************************************************************
函数名称：void Key_deal(void)
功    能：按键处理
参    数：None
返回值  ：None
**************************************************************************************/
															
void Key_deal(void)
{
	u8 i = 0;
	Check_Key();
	for(i = 0;i < 16;i++)
	{
		if(KEY_MSG_FIFO[i].status == KEY_DOWN)
		{	
			(*Task_ShowCarry[KEY_MSG_FIFO[i].key])();
		}
	}
}

/*************************************************************************************
函数名称：void A_Show(void)
功    能：0值按键按下后OLED显示
参    数：None
返回值  ：None
**************************************************************************************/

void A_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"7");
}

void B_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"8");
}

void C_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"9");
}

void D_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"C");
}

void E_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"*");
}

void F_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"0");
}

void G_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,".");
}

void H_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"D");
}

void I_Show(void)
{
	OLED_Clear();
	MyModeChoose = Mode1;
	OLED_P6x8Str(0,0,"MODE1");
}

void J_Show(void)
{
	OLED_Clear();
	MyModeChoose = Mode2;
	OLED_P6x8Str(0,0,"MODE2");
}

void K_Show(void)
{
	OLED_Clear();
	MyModeChoose = Mode3;
	OLED_P6x8Str(0,0,"Mode3");
}

void L_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"A");
}

void M_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"4");
}

void N_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"5");
}

void O_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"6");
}

void P_Show(void)
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"B");
}

