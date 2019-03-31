#include "UartLink.h"
#include "DCMI.h"
#include "QEI.h"

Uartbuff    UartBuff;
UartTraCtr	UartTractr = {0,0,0,0,0,0,0};//0:ͨ���ر� 0xff:ͨ����;
double DCMI_temp = 0x00;
unsigned char DCMI_grayscale = 0x00;

double UartData[9] = {0};
SerialPortType SerialPortRx;

/*************************************************************************************
�������ƣ�void UartInit(USART_TypeDef* USARTx)
��    �ܣ����ڳ�ʼ��
��    ����USARTx�����ں�
����ֵ  ��None
**************************************************************************************/

void UartInit(USART_TypeDef* USARTx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);   //TX   �ܽŸ���
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);   //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = SerialBaud;   //����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);      //����ʹ��
    USART_ClearFlag(USARTx, USART_FLAG_TC);	 //�����־λ
    USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);	//���������ж�

    //DMA���ã�
    //����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ
    //�����ڴ�-->����
    //ÿ�δ���λ��8bit
    //�����СDMA_BufferSize=SENDBUFF_SIZE
    //��ַ����ģʽ�������ַ�������ڴ��ַ����1
    //DMAģʽ��һ�δ��䣬��ѭ��
    //���ȼ�����
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); /* ʹ�� DMA1 ʱ�� */
    DMA_DeInit(DMA1_Stream6);  
    while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
    {}
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->DR;	        //Ŀ�ĵ�ַ
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)UartBuff.DMASendData;			
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                     //�ڴ浽�豸
    DMA_InitStructure.DMA_BufferSize = TotalDataNum+3;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;         //�豸��һ�δ���һ���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ�һ�δ���һ���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode =  DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);

    //DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);   //ʹ���жϲ�����DMA_IT_TCIF4,Ҫ���ݿ⺯���ṩ����ȷ��

    DMA_Cmd(DMA1_Stream6, ENABLE);			//ʹ��һ��DMA������һ�����ݣ�ǰ�����жϱ�־λҪ���
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
}

/*************************************************************************************
�������ƣ�void uart_putchar(USART_TypeDef* USARTx,uint8_t sendData)
��    �ܣ����ڷ���
��    ����USARTx�����ںţ�sendData��Ҫ���͵�����
����ֵ  ��None
**************************************************************************************/

void uart_putchar(USART_TypeDef* USARTx,uint8_t sendData)
{
#if(true)
    USART_ClearFlag(USARTx , USART_FLAG_TXE);
    USART_SendData(USARTx , sendData);								   // �����ֽ�
    while(USART_GetFlagStatus(USARTx , USART_FLAG_TXE) == RESET);	   // �ȴ����ͽ���
    USART_ClearFlag(USARTx , USART_FLAG_TXE);
#else
    while (!(USARTx->SR & USART_FLAG_TXE));	  // �ȴ����ͽ���
    // USART_ClearFlag(USARTx , USART_FLAG_TXE);
    //  USART_SendData(USARTx , sendData);								   // �����ֽ�
    USARTx->DR = sendData;
#endif
}

/*************************************************************************************
�������ƣ�u16 uart_getchar(USART_TypeDef* USARTx)
��    �ܣ����ڽ���
��    ����USARTx�����ں�
����ֵ  ��None
**************************************************************************************/

u16 uart_getchar(USART_TypeDef* USARTx)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Receive Data */
  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

/*************************************************************************************
�������ƣ�void push(u8 index, s16 number)
��    �ܣ����͸���λ�����ݵĻ�����
��    ����None
����ֵ  ��None
**************************************************************************************/

void push(u8 index, s16 number)
{
    UartBuff.SendData[index*2]=(u8)(number>>8);
    UartBuff.SendData[index*2+1]=(u8)(number&0xff);
}

/*************************************************************************************
�������ƣ�void Send_Data(USART_TypeDef* USARTx,u8 *data)
��    �ܣ�����һ������
��    ����data�����ݵ�ַ
����ֵ  ��None
**************************************************************************************/

void Send_Data(USART_TypeDef* USARTx,u8 *data)
{
    int i;
    for(i=0;i<1;i++)
    {
        uart_putchar(USARTx,*data);
        data++;
    }
}

/*************************************************************************************
�������ƣ�void uart_sendN (USART_TypeDef* USARTx, u8 *buff, u32 len)
��    �ܣ�����һ������
��    ����USARTx:���ںţ�buff�����ݵ�ַ��len�����ݳ���
����ֵ  ��None
**************************************************************************************/

void uart_sendN (USART_TypeDef* USARTx, u8 *buff, u32 len)
{
    int i;
    for(i = 0; i < len; i=i+4)
    {      
		//	buff[i] = (buff[i]>DCMI_grayscale?0xf0:0x00);
        uart_putchar(USARTx, buff[i]);
    }
}

/*************************************************************************************
�������ƣ�void Send_NData()
��    �ܣ�����һ�����ݸ���λ��
��    ����None
����ֵ  ��None
**************************************************************************************/
 
void Send_NData(void)
{
  unsigned char i;
	int 	sum=0; 
  uart_putchar(MYUART,251); 
	uart_putchar(MYUART,109);
	uart_putchar(MYUART,37);
   //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�֪���������
  sum+=(251);      //��ż����������У��
  sum+=(109);
	sum+=(37);
  for(i=0;i<34;i++)
  {
    uart_putchar(MYUART,UartBuff.SendData[i]);
    sum+=UartBuff.SendData[i];         //��ż����������У��
  }
  uart_putchar(MYUART,sum);
}

/*************************************************************************************
�������ƣ�void SendOV7620(void)
��    �ܣ�����OV7620����ͷ���ݵ���λ��
��    ����None
����ֵ  ��None
**************************************************************************************/

void SendOV7620(void)
{
  	uint32_t i,j;
	  uint8_t d;
    uart_putchar(MYUART,0xFF);
    uart_putchar(MYUART,0xFE);
    uart_putchar(MYUART,0xFF);
		uart_putchar(MYUART,0xFE);
	for(i = 0; i < CAMERA_H; i++)
	{ 
		for(j = 0; j < CAMERA_W; j++)
		{
	   d=Image[i][j];
			if(d > 0xFD) d = 0xFD;         
    uart_putchar(MYUART,d);
		}
		uart_putchar(MYUART,0xFE);
		uart_putchar(MYUART,0xFE);
	}
//  i=0;
  j=0;  
}

/*************************************************************************************
�������ƣ�void Send_Str(u8 *s)
��    �ܣ������ַ�������λ��
��    ����*s���ַ����׵�ַ
����ֵ  ��None
**************************************************************************************/

void Send_Str(u8 *s)
{
   // while(!checkdelay_ms(SetTime.StrTm));
    //SetTime.StrTm=delay_ms(200);
    uart_putchar(MYUART,0xAB);
    uart_putchar(MYUART,0xCD);
    uart_putchar(MYUART,0xEF);
    uart_sendN  (MYUART,s,40);
}

/*************************************************************************************
�������ƣ�void Send_DataPlusStr(u8 s1[30],s32 data,u8 s2[60])
��    �ܣ��������ݼ��ַ�������λ�� ��ʱ��2.5ms
��    ����u8 s1[30],s32 data,u8 s2[60]
����ֵ  ��None
**************************************************************************************/

void Send_DataPlusStr(u8 s1[30],s32 data,u8 s2[60])
{
	int j=0;

    //while(!(SetTime.StrNumTm));

    //SetTime.StrNumTm=setdelay_ms(200);
    uart_putchar(MYUART,0x12);
    uart_putchar(MYUART,0x23);
    uart_putchar(MYUART,0x34);
    
    while(j<30&&s1[j]!='\0')
    {
        uart_putchar(MYUART,s1[j++]);
    }
    if(data<0)
    {
        uart_putchar(MYUART,'-');
        data=-data;
    }
    if(data>=10000000) uart_putchar(MYUART,(data/10000000)+'0');
    if(data>=1000000)  uart_putchar(MYUART,(data%10000000/1000000)+'0');
    if(data>=100000)   uart_putchar(MYUART,(data%1000000/100000)+'0');
    if(data>=10000)    uart_putchar(MYUART,(data%100000/10000)+'0');
    if(data>=1000)     uart_putchar(MYUART,(data%10000/1000)+'0');
    if(data>=100)      uart_putchar(MYUART,(data%1000)/100+'0');
    if(data>=10)       uart_putchar(MYUART,(data%100)/10+'0');
    uart_putchar(MYUART,(data%10)+'0');
    uart_sendN  (MYUART,s2,60);
}

/*************************************************************************************
�������ƣ�void Received_Succeed(u8 Statu)
��    �ܣ����سɹ����ձ�־
��    ����Statu������״̬
����ֵ  ��None
**************************************************************************************/

void Received_Succeed(u8 Statu)
{
    if(Statu)
    {
        uart_putchar(MYUART,0x89);  //ͨ����
        uart_putchar(MYUART,0x9a);
        uart_putchar(MYUART,0xab);
    }
    else
    {
        uart_putchar(MYUART,0xAB);  //ͨ���ر�
        uart_putchar(MYUART,0xBC);
        uart_putchar(MYUART,0xCD);
    }
}

/*************************************************************************************
�������ƣ�void USART2_IRQHandler(void)
��    �ܣ�����1�жϷ�����򣬴���λ���������ݵ���Ƭ��
��    ����None
����ֵ  ��None
**************************************************************************************/

//void USART2_IRQHandler(void)                	
//{
//	char Rec = 0;
//    int i,b,d1;
//    u32 d;
//    
//	if(USART_GetITStatus(UART_BlueTooth, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		    Rec = uart_getchar(UART_BlueTooth); 
//        SerialPortRx.Data = Rec;    
//    
//        if( SerialPortRx.Stack < UartRxBufferLen )
//        {
//            SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
//      
//            if(    SerialPortRx.Stack >= UartRxDataLen  
//                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  ==0xff 
//                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1]==0x55
//                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2]==0xaa
//                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3]==0x10 )
//            {   
//                SerialPortRx.Check = 0;
//                b = SerialPortRx.Stack - UartRxDataLen; 
//                for(i=b; i<SerialPortRx.Stack-1; i++)  
//                {
//                    SerialPortRx.Check += SerialPortRx.Buffer[i];
//                }
//        
//                if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
//                {   
//                    for(i = 0; i<9; i++)
//                    {
//                        d = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
//                        if(d>0x7FFFFFFF)
//                        {
//                            d1 = 0x7FFFFFFF - d;
//                        }
//                        else
//                        {
//                            d1 = d;
//                        }
//                        UartData[i]=d1;
//                        UartData[i]/=65536.0;     //��λ�����͹��������ݻ�����												
//                    }
//										if(UartData[8] == 0)
//										{
//											Rev_Page0();
//										}
//										else if(UartData[8] == 1)
//										{
//											Rev_Page1();
//										}
//										else if(UartData[8] == 2)
//										{
//											Rev_Page2();
//										}
//										else if(UartData[8] == 3)
//										{
//											Rev_Page3();
//										}
//                }
//                SerialPortRx.Stack = 0;
//            }
//            else if(  SerialPortRx.Stack >= UartRxCmdLen 
//                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  ==0xff
//                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+1]==0x55
//                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+2]==0xaa
//                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+3]==0x77 )//cmd
//                {
//                    SerialPortRx.Check = 0;
//                    b = SerialPortRx.Stack - UartRxCmdLen; 
//                    for(i=b; i<SerialPortRx.Stack-1; i++) 
//                    {
//                        SerialPortRx.Check += SerialPortRx.Buffer[i];
//                    }
//                    if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
//                    {   
//                        UartCmd(UartCmdNum,UartCmdData);
//                    }
//                    SerialPortRx.Stack = 0;                  
//                }
//            }
//        else
//        {
//            SerialPortRx.Stack = 0;
//        } 
//    }	 
//  } 

//void UartDebug(void)
//{
//    push(13,(int)UartData[0]);	
//    push(14,(int)UartData[1]);		
//    PID0Velocity.SetVal = UartData[0];
//    PID1Velocity.SetVal = UartData[1];
//}

void UartCmd(u8 CmdNum,u8 Data)
{
//	push(0,CmdNum);	
//  push(1,Data);
//  switch(CmdNum)
//    {
//        case UART_IMAG_TX:
//        {
//            if(Data)
//                UART_IMAGE_TRANSFER=true;
//            else
//                UART_IMAGE_TRANSFER=false;
//            break;
//        }
//    }		 
}

/***********************************************************************************
�������ƣ�void Rev_Page0(void)
��    �ܣ���λ�������ٶ�λ��ʽ����PID����ֵ ������ͬһPID��
��    ������
����ֵ  ����
************************************************************************************/

void Rev_Page0(void)
{
	SpeedCtr.SpeedSet = UartData[0];
	SpeedCtr.Kp = UartData[1];
	SpeedCtr.Ki = UartData[2];
	SpeedCtr.Kd = UartData[3];
}

/***********************************************************************************
�������ƣ�void Rev_Page1(void)
��    �ܣ���λ�������ٶ�����ʽ����PID����ֵ�����ֶ���PID��
��    ������
����ֵ  ����
************************************************************************************/

void Rev_Page1(void)
{
	pid_left->SetSpeed = UartData[0];
	pid_left->Proportion = UartData[1];
	pid_left->Integral = UartData[2];
	pid_left->Derivative = UartData[3];
	pid_right->SetSpeed = UartData[4];
	pid_right->Proportion = UartData[5];
	pid_right->Integral = UartData[6];
	pid_right->Derivative = UartData[7];
}

/***********************************************************************************
�������ƣ�void Rev_Page2(void)
��    �ܣ���λ�������ٶ�λ��ʽ����PID����ֵ�����ֶ���PID��
��    ������
����ֵ  ����
************************************************************************************/

void Rev_Page2(void)
{
	LeftSpeedCtr.SpeedSet = UartData[0];
	LeftSpeedCtr.Kp = UartData[1];
	LeftSpeedCtr.Ki = UartData[2];
	LeftSpeedCtr.Kd = UartData[3];
	RightSpeedCtr.SpeedSet = UartData[4];
	RightSpeedCtr.Kp = UartData[5];
	RightSpeedCtr.Ki = UartData[6];
	RightSpeedCtr.Kd = UartData[7];
}

/***********************************************************************************
�������ƣ�void Rev_Page3(void)
��    �ܣ���λ�����ͽǶ�λ��ʽ����PID����ֵ�����ֶ���PID��
��    ������
����ֵ  ����
************************************************************************************/

void Rev_Page3(void)
{
	LeftAngleCtr.AngleSet = UartData[0];
	LeftAngleCtr.Kp = UartData[1];
	LeftAngleCtr.Ki = UartData[2];
	LeftAngleCtr.Kd = UartData[3];
	RightAngleCtr.AngleSet = UartData[4];
	RightAngleCtr.Kp = UartData[5];
	RightAngleCtr.Ki = UartData[6];
	RightAngleCtr.Kd = UartData[7];
}


