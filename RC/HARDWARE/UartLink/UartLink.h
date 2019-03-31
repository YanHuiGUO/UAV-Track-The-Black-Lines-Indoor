#ifndef _Uart_Link_H
#define _Uart_Link_H

#include "common.h"

//串口配置
#define   MYUART    	 USART2
#define   SerialBaud 	 115200
#define   DMA_UART_TRA 	 true
#define   FIFO_UART_TRA  false

#define   RxBuffer_Size    100

#define   UartDataNum     17
#define   TotalDataNum   UartDataNum*2

#define UART_BlueTooth        USART2
#define BaudRate_BlueTooth    115200

#define UartDataNum           17
#define _PI	              3.1415926535897932f
#define true 1
#define false 0

#define UartRxBufferLen       100
#define UartRxDataLen         41        
#define UartRxCmdLen          7	       

#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]

typedef struct 
{
  int Stack;
  u8 Data;
  u8 PreData;
  u8 Buffer[UartRxBufferLen];
  u8 Enable;
  u8 Check;
}SerialPortType;

extern u8 uSendBuf[UartDataNum*2];
extern SerialPortType SerialPortRx;
extern double UartData[9];

typedef struct 
{
  u8    DATActr;
  u8    TraCtr;
  u8    Cameractr;
  u8    RecRoad;
  u8    DMActr;
  u8    Binctr;
  u8    SDctr;
  u8    SysStart;
  u8    IRQMode;
}UartTraCtr; 

typedef struct
{
  u32 Stack;//接收数据字节数
  u32 SlaveStack;
  u8  Data;
	u8  PreData;
  u8  Len;
  u8  Buffer[RxBuffer_Size];
	u8  Enable;
  u8  Check;
  u8  SlaveBuffer[RxBuffer_Size];
}Rx_Port;

//BUFF
typedef struct
{
  u8    SendData[TotalDataNum]; //有符号数据数组
	u8    DMASendData[TotalDataNum + 3];

}Uartbuff;  //UartBuff

typedef union
{
    u32 numbercheck;
    u8  checkbuff[4];
}Union;

extern UartTraCtr	UartTractr;
extern Rx_Port  Rx_PORT;
extern unsigned char DCMI_grayscale;
extern void UartInit(USART_TypeDef* USARTx);
extern void uart_putchar(USART_TypeDef* USARTx,uint8_t sendData);
void push(u8 index, s16 number);
extern u16  uart_getchar(USART_TypeDef* USARTx);
extern void Send2dream_Ctr(void);
extern void Send_Data(USART_TypeDef* USARTx,u8 *data);
extern void Send_NData(void);
extern void Received_Succeed(u8 Statu);
extern void Send_Str(u8 *s);
extern void SendOV7620(void);
extern void Send_DataPlusStr(u8 s1[30],s32 data,u8 s2[60]);
extern void uart_sendN (USART_TypeDef* USARTx, u8 *buff, u32 len);
void UartDebug(void);
void UartCmd(u8 CmdNum,u8 Data);
void Rev_Page0(void);
void Rev_Page1(void);
void Rev_Page2(void);
void Rev_Page3(void);
#endif
