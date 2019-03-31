#include "key2oled.h"
#include "OLED.h"
#include "key_matrix.h"
#include "nrf24l01.h"
typedef void (* Task_Show)(void);
char MyModeChoose = -1;//ģʽѡ���жϴ˱���ѡ��ģʽ
unsigned int KeySetPara[ParaQuatity]={0};//�����洢3��5λ�������˱����������õĲ���
static u8  MyParaTemp[ParaLenth]={'_','_','_','_','_'};
static u8  DisTemp[ParaLenth+1] = {'_','_','_','_','_'};//��һλ��ʾ�Ľ�����\0
char MyFinalNumLen[ParaQuatity] = {0};
static u8 SetTime = 0;
static char  ModeTemp = 0;
static char  ModeChooseKeyflag = 0;
static char  ParaChooseKeyflag = 0;
static char  WhitchPara = 0;
//����ָ�����飬������OLED��ʾ
Task_Show Task_ShowCarry[] = {A_Show,B_Show,C_Show,D_Show,    /*	7,8,9,����������*/
															E_Show,F_Show,G_Show,H_Show,		/*	�л�ģʽ,0,����ѡ��,ȷ��*/
															I_Show,J_Show,K_Show,L_Show,		/*	1,2,3,��һ������			*/
															M_Show,N_Show,O_Show,P_Show};		/*	4,5,6,�ڶ�������			*/

/*************************************************************************************
�������ƣ�void Key_deal(void)
��    �ܣ���������
��    ����None
����ֵ  ��None
**************************************************************************************/
u8 Key_Down_Right=0;	
u8 NRFSend = 0;															
void Key_deal(void)
{
	u8 i ,j= 0;
	Check_Key();
	for(i = 0;i < 16;i++)
	{
		if((KEY_MSG_FIFO[i].status == KEY_DOWN)&&(Key_Down_Right == 1)) //���ּ��
		{	
			Key_Down_Right = 0;
			(*Task_ShowCarry[KEY_MSG_FIFO[i].key])();
			NrfTransmit();
		}
		if(KEY_MSG_FIFO[i].status == KEY_UP)
		{
			j++;
			if(j == 16)
			{
				Tx_Buf[0] = 0;
				Key_Down_Right = 1;
			}
		}
	}
}
/*************************************************************************************
�������ƣ�void A_Show(void)
��    �ܣ�0ֵ�������º�OLED��ʾ
��    ����None
����ֵ  ��None
**************************************************************************************/
//������A��PC0��ͬ��
int TESTMyket = 0;
void A_Show(void)//7
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"7");
}

void B_Show(void)//8
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"8");
}

void C_Show(void)//9
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"9");
}

void D_Show(void)//C,����������
{

	OLED_Clear();
	OLED_P6x8Str(0,0,"C");
}
/*************************************************************************************
�������ƣ�void E_Show(void)
��    �ܣ��л�ģʽ
��    ����None
����ֵ  ��None
**************************************************************************************/

void E_Show(void)//* 
{
		OLED_Clear();
	OLED_P6x8Str(0,0,"*");
}

void F_Show(void)//0
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"0");
}

void G_Show(void)//.,��������ѡ���
{

	OLED_Clear();
	OLED_P6x8Str(0,0,".");
}

void H_Show(void)//D��ȷ�ϼ�
{ 
		OLED_Clear();
	OLED_P6x8Str(0,0,"D");
}

void I_Show(void)//1
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"1_Fly_Ready");
	Tx_Buf[0] = 1;
	
}

void J_Show(void)//2
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"2_Fly_Stop");
	
	Tx_Buf[0] = 2;
}

void K_Show(void)//3
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"3_Rise");
	Tx_Buf[0] = 3;
}

void L_Show(void)//A,��һ������
{
	
	OLED_Clear();
	OLED_P6x8Str(0,0,"A");

}

void M_Show(void)//4
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"4_Land");
	Tx_Buf[0] = 4;
}

void N_Show(void)//5
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"5");
}

void O_Show(void)//6
{
	OLED_Clear();
	OLED_P6x8Str(0,0,"6");
}

void P_Show(void)//B���ڶ�������
{
	
	OLED_Clear();
	OLED_P6x8Str(0,0,"B");
}
void MyOneParaDeal(char Para,char key2num)
{
	u8 Mynumtemp = 0;
	u8 CountLen = 0;
	char i=0;
	OLED_Clear();
	switch(key2num)
	{
		case 0:Mynumtemp = 7;break;//7
		case 1:Mynumtemp = 8;break;//8
		case 2:Mynumtemp = 9;break;//9
		case 5:Mynumtemp = 0;break;//0
		case 8:Mynumtemp = 1;break;//1
		case 9:Mynumtemp = 2;break;//2
		case 10:Mynumtemp = 3;break;//3
		case 12:Mynumtemp = 4;break;//4
		case 13:Mynumtemp = 5;break;//5
		case 14:Mynumtemp = 6;break;//6
		default:return;
	}
	if(SetTime<ParaLenth)
	{
		MyParaTemp[SetTime] = Mynumtemp;	
		SetTime ++;
	}
	else 
	{//�����������Ⱥ�����
		for(i =0;i<ParaLenth;i++)
		{
			DisTemp[i] ='_';
			MyParaTemp[i] ='_';
		}
		SetTime = 0;
		MyFinalNumLen[Para-1]=0;
	}
	for(i=0;i<ParaLenth;i++)
	{
		if(MyParaTemp[i]!='_')
		{
		 DisTemp[i]=MyParaTemp[i] +'0';
	   CountLen ++;
		}
	}
  DisTemp[i] = '\0';
	MyFinalNumLen[Para-1] = CountLen;
	
	OLED_P6x8Str(0,0,DisTemp);

}
/*	7,8,9,����������*/
/*	�л�ģʽ,0,����ѡ��,ȷ��*/
/*	1,2,3,��һ������			*/
/*	4,5,6,�ڶ�������			*/
void ParaDeal(char key2num)
{
	MyOneParaDeal(WhitchPara,key2num);
}

