#include "Ultra.h"
//串口超声波初始化，使用串口3，TX:PD8 RX:PD9
void Ultra_Init(u32 br_num)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);   //TX   管脚复用
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);   //RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	//配置PD6作为USART2　Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		//串口中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

   USART_InitStructure.USART_BaudRate = br_num;   //波特率设置
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART3, &USART_InitStructure);
   USART_Cmd(USART3, ENABLE);      //串口使能
   USART_ClearFlag(USART3, USART_FLAG_TC);	 //清除标志位
   USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//开启接收中断
		

}

float MyUltraHeight  =0;
char  Ultra_Ok = 0;
u8 Ultra_TxBuffer[5];
u8 Ultra_TxCounter=0;
u8 Ultra_count=0; 
u8 US_100[1]={0x55};
u8 Ultra_Rx_Buf[2];	//串口接收缓存

void USART3_IRQ(void)
{
	u8 com_data;
	float UltraTemp=0;
	static u8 ultra_cnt =0;
	if(USART3->SR & USART_SR_ORE)//ORE中断
	{
		com_data = USART3->DR;
	}

  //接收中断
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除中断标志
		com_data = USART3->DR;
		Ultra_Rx_Buf[ultra_cnt]	= com_data;
		ultra_cnt ++ ;
		if(ultra_cnt == 2)
		{
			ultra_cnt = 0;	
			UltraTemp = (int)(Ultra_Rx_Buf[0]<<8)+Ultra_Rx_Buf[1];
			UltraTemp = UltraTemp/10;
			MyUltraHeight	=	Moving_Average(0,10,UltraTemp);
			Ultra_Ok = 1;
		}
	}
	//发送（进入移位）中断
	if( USART_GetITStatus(USART3,USART_IT_TXE ) )
	{		
		USART3->DR = Ultra_TxBuffer[Ultra_TxCounter++]; //写DR清除中断标志          
		if(Ultra_TxCounter == Ultra_count)
		{
			Ultra_count = 0;
			Ultra_TxCounter =0;
			USART3->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE（发送中断）中断
		}
	}
}

void Usart3_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
	for(i=0;i<data_num;i++)
	{
		Ultra_TxBuffer[Ultra_count++] = *(DataToSend+i);
	}
	if(!(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
	}
}