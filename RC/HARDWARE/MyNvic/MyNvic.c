#include "MyNvic.h"

/*
系统已经使用的中断程序	                   重要程度
1.FlyTool USART3 接收中断			   	        2
2.Multy Communication USART2 接收中断		    2
3.DCMI  捕获中断					   		    0
4.QEI定时，TIMER7 定时溢出中断				1
5.ADC采样完成中断							    3
6.TIMER12 系统任务调配						    4
*/

/*
        @arg NVIC_PriorityGroup_0: 0 bits for pre-emption priority
  *                                4 bits for subpriority
  *     @arg NVIC_PriorityGroup_1: 1 bits for pre-emption priority	  2
  *                                3 bits for subpriority
  *     @arg NVIC_PriorityGroup_2: 2 bits for pre-emption priority	  4
  *                                2 bits for subpriority
  *     @arg NVIC_PriorityGroup_3: 3 bits for pre-emption priority	  8
  *                                1 bits for subpriority
  *     @arg NVIC_PriorityGroup_4: 4 bits for pre-emption priority	  16
  *                                0 bits for subpriority
  ENorDis: ENABLE
           DISABLE
  
*/
void MyNVIC_Init(UInt32 group,UInt32 PreempPriority,UInt32 SubPriority,Byte IRQCha,FunctionalState ENorDis)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(group);
    NVIC_InitStructure.NVIC_IRQChannel = IRQCha;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreempPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENorDis;
    NVIC_Init(&NVIC_InitStructure);
}
