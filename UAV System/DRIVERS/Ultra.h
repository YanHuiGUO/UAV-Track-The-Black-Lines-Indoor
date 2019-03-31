#ifndef _ULTRA_H
#define _ULTRA_H
#include "stm32f4xx.h"
#include "mymath.h"
extern u8 Ultra_Rx_Buf[2];
extern u8 US_100[1];
extern char  Ultra_Ok;
extern float MyUltraHeight;//单位CM，分辨率mm
void Ultra_Init(u32 br_num);
void USART3_IRQ(void);
void Usart3_Send(unsigned char *DataToSend ,u8 data_num);
#endif
