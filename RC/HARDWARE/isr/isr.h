//V1.0.0
#ifndef __isr_H
#define __isr_H


#include "Common.h"
extern u8   Cir_time;
extern float  UART_WL[10];

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TIM5_IRQHandler(void);
void TIM7_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
extern bool Key_Start;
void UartDebug(void);
 

#endif 




