#ifndef __KEY2OLED_H
#define __KEY2OLED_H
#include "common.h"
#define ParaLenth 5
#define ParaQuatity 3 
void ParaDeal(char key2num);
void Key_deal(void);
void A_Show(void);
void B_Show(void);
void C_Show(void);
void D_Show(void);
void E_Show(void);
void F_Show(void);
void G_Show(void);
void H_Show(void);
void I_Show(void);
void J_Show(void);
void K_Show(void);
void L_Show(void);
void M_Show(void);
void N_Show(void);
void O_Show(void);
void P_Show(void);
enum 
{ 
	Mode1,
	Mode2,
	Mode3,
	Mode4,
	Mode5,
	Mode6,
};
extern u8 NRFSend ;
extern char MyModeChoose;
extern unsigned int KeySetPara[ParaQuatity];
#endif

