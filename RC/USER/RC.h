#include "common.h"
#ifndef _RC_H
#define _RC_H
#define RC_PORT			GPIOB
#define RC_CLK      RCC_AHB1Periph_GPIOB
#define RC_SINGAL		GPIO_Pin_11
#define IRDA_ID 0
	typedef  struct 
	{
		unsigned long int	count ;
		char Error;
		char first_In_EXit;
		int time ;
		char GOback_Flag1;
		char GOback_Flag2;
	}RC_Struct;
unsigned int IrDa_Process(void);
void RC_Init(void);
void TIM14_Init(unsigned int TIM_scale, unsigned int TIM_Period);
uint8_t Get_Pulse_Time(void);
void	RC_LOOP(void);
extern 	RC_Struct RC;
extern	uint32_t frame_data ;    /* 一帧数据缓存 */
extern uint8_t  frame_cnt ;
extern uint8_t  frame_flag ;    /* 一帧数据接收完成标志 */
#endif

