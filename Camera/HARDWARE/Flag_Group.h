#ifndef _FLAG_GROUP_H
#define _FLAG_GROUP_H


typedef struct 
{
	char 	_1;
	char 	_2;
	char 	_3;
	char 	_4;
	char 	_5;
	char 	_6;
	char  Fly_ready;
	char  Fly_Start;//����������Ʊ�־λ���ܿ����������������
	char  Fly_Stop;
	char  Urgency_Stop;
}	Flag_Group;

extern Flag_Group Flag_ThrControl;
#endif


