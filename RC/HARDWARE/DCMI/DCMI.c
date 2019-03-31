#include "DCMI.h"
#include "delay.h"
#include "MyNVIC.h"
#include "OV7620_REG.h"
#include "UartLink.h"

CamCtrlClass ImgControl;

u8 Image[CAMERA_H][CAMERA_W]= {0};

/************************************************************************
* @ �������ƣ�sccb_wait
* @ ����˵����SCCB��ʱ����Ӧ̫С
* @ ����˵������
* @ �������أ���
*************************************************************************/
void sccb_wait(void)
{
	delay_us(10);
}
/************************************************************************
* @ �������ƣ�sccb_start
* @ ����˵����SCCB����λ
* @ ����˵������
* @ �������أ���
*************************************************************************/
void sccb_start(void)
{
  SCL_OUT;
  SDA_OUT;
 
  SDA_HIGH;
  //sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_LOW;
  sccb_wait();
  SCL_LOW;
}

/************************************************************************
* @ �������ƣ�sccb_stop
* @ ����˵����SCCBֹͣλ
* @ ����˵������
* @ �������أ���
*************************************************************************/
void sccb_stop(void)
{
  SCL_OUT;
  SDA_OUT;
  
  SDA_LOW;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_HIGH;
  sccb_wait();
}

/************************************************************************
* @ �������ƣ�sccb_sendByte
* @ ����˵������SCCB�����Ϸ���һ���ֽ�
* @ ����˵����data Ҫ���͵��ֽ�����
* @ �������أ���
*************************************************************************/
u8 sccb_sendByte(u8 data)
{
  u8 i;
   u8 ack;
  SDA_OUT;
  for( i=0; i<8; i++)
  {
    if(data & 0x80)
      SDA_HIGH;
    else 
      SDA_LOW;
    data <<= 1;
    sccb_wait();
    SCL_HIGH;
    sccb_wait();
    SCL_LOW;
    sccb_wait();
  }
  SDA_HIGH;
  SDA_IN;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  ack = SDA_DATA;
  SCL_LOW;
  sccb_wait();
  return ack;
}


/************************************************************************
* @ �������ƣ�sccb_regWrite
* @ ����˵����ͨ��SCCB������ָ���豸��ָ����ַ����ָ������
* @ ����˵����device---�豸��  ��д������
*            address---д���ݵļĴ���
*            data---д������
* @ �������أ�ack=1δ�յ�Ӧ��(ʧ��)    ack=0�յ�Ӧ��(�ɹ�)
*************************************************************************/
u8 sccb_regWrite(u8 device,u8 address,u8 data)
{
  u8 i;
  u8 ack;
  for( i=0; i<200; i++)
  {
    sccb_start();
    ack = sccb_sendByte(device);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(address);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(data);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    sccb_stop();
    if( ack == 0 ) break;
  }
  return ack;
}
/************************************************************************
* @ �������ƣ� IICInit(void)
* @ ����˵���� IIC���ų�ʼ��
* @ ����˵���� ��
* @ �������أ� ��
*************************************************************************/

void IICInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = (1<<SCLPin) | (1<<SDAPin);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  SDA_HIGH;
	  SCL_HIGH;
}

/************************************************************************
* @ �������ƣ� ov7620_reg[]
* @ ����˵���� ov7620��ʼ������
* @ ����˵���� ��
* @ �������أ� ��
*************************************************************************/
Register_Info ov7620_reg[] =
{
#if 1  //QVGA
    //�Ĵ������Ĵ���ֵ��
    {CLKRC     ,0x00},  //��Ƶ
    {COMC      ,0x24},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7A-CAMERA_W/2/2)},//��ȡͼ��ͼ��У��
    {HSIZE     ,(0x7A+CAMERA_W/2/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)},
    {VEEE      ,0x42},  //��ֱ��Ե��ǿ
    {COMH      ,0x00},     //ɨ��ģʽ
#else //VGA
    //�Ĵ������Ĵ���ֵ��
    {CLKRC     ,0x00}, 
    //{COMC      ,0x04},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/4/2)},
    {HSIZE     ,(0x7F+CAMERA_W/4/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    //{VEEE      ,0x42},  //��ֱ��Ե��ǿ
    //{COMH      ,0x20},  //ɨ��ģʽ
#endif

};

u8 cfgnum = sizeof(ov7620_reg)/sizeof(ov7620_reg[0]);   /*�ṹ�������Ա��Ŀ*/

/************************************************
 * @ ��������Ov7620_Init
 * @ ����  ��Sensor��ʼ��
 * @ ����  ����
 * @ ���  ������1�ɹ�������0ʧ��
 * @ ע��  ����
 ************************************************/
u8 Ov7620_Init(void)
{	
  IICInit();
	DCMIInit();
  MCO1_Init();
	
	if(Ov7620_reg_Init() == 0)
	{
		return 0;
	}
    return 1;
}

/************************************************
 * @  ��������Ov7620_reg_Init
 * @ ����  ��Sensor �Ĵ��� ��ʼ��
 * @ ����  ����
 * @ ���  ������1�ɹ�������0ʧ��
 * @ ע��  ����
 ************************************************/
u8 Ov7620_reg_Init(void)
{
    u16 i = 0;
    while( 1 == sccb_regWrite ( 0x42, 0x12, 0xA4 ) ) /*��λsensor */
    {
        i++;
		if(i == 20)
		{
			return 0 ;
		}		
    }
    for( i = 0 ; i < cfgnum ; i++ )
    {
         while( 1 == sccb_regWrite(0x42,ov7620_reg[i].Address, ov7620_reg[i].Value) )
         {
            return 0;
         }
    }
    return 1;
}
/*******************************************************************************
  * @��������   DCMI_Config
  * @����˵��   DCMI�ӿ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void DCMIInit(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    DCMI_InitTypeDef DCMI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);   /* ʹ�� DCMI ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   /* ʹ�� DMA2 ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE); /* ʹ�� DCMI GPIOs ʱ�� */

    /* DCMI ���ų�ʼ�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_6|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4  ;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*** Configures the DCMI GPIOs to interface with the OV2640 camera module ***/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);           //DCMI_HSYNC
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);           //DCMI_PIXCLK
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_DCMI);           //DCMI_D0
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_DCMI);           //DCMI_D1
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);          //DCMI_D2
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);          //DCMI_D3
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4,GPIO_AF_DCMI);          //DCMI_D4
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);          //DCMI_D5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);          //DCMI_VSYNC
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI);          //DCMI_D6
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);          //DCMI_D7

    DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //��������ģʽ
    DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;     //Ӳ��ͬ��ģʽ
    DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;       //����ʱ������Falling
    DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;           //֡ͬ������
    DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;            //ˮƽͬ������//����High
    DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;    //����֡ͼ��
    DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //ÿ������8λ

    DCMI_Init(&DCMI_InitStructure);
		
    DCMI_ITConfig(DCMI_IT_VSYNC,ENABLE);
    DCMI_ITConfig(DCMI_IT_LINE,ENABLE);

    MyNVIC_Init(NVIC_PriorityGroup_2,0,1,DCMI_IRQn,ENABLE);	//�����ж����ȼ�
   
    DCMI_Cmd(ENABLE);
    DCMI_CaptureCmd(ENABLE);

    /* Configures the DMA2 to transfer Data from DCMI */
    DMA_DeInit(DMA2_Stream1);
    while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
    {
    }
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;                 //������ӿ�����
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&(Image[0]);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //�豸���ڴ�
    DMA_InitStructure.DMA_BufferSize = CAMERA_W;                                //ÿ�δ������ݴ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;         //word�豸��һ�δ���4���ֽ�32λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode =  DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);

    ImgControl.ImgDealOk = true;
    ImgControl.ImgOk = true;
    ImgControl.lines = 0;
}
/*******************************************************************************
  * @��������   MCO1_Init(void)
  * @����˵��   ���һ��ʱ���źŵ�PTA8
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MCO1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_ClockSecuritySystemCmd(ENABLE);

    /* Enable GPIOs clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);

    /* Configure MCO (PA8) */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_3);
}

/*******************************************************************************
  * @��������   DCMI_IRQHandler(void)
  * @����˵��   ����DMA����ͼ��ɼ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/

void DCMI_IRQHandler(void)
{
    if (DCMI->MISR & DCMI_IT_VSYNC)//VSYNC capture
    {
        DCMI->ICR = DCMI_IT_VSYNC ;//clear flag
        if(ImgControl.ImgDealOk)
        {
            ImgControl.lines = 0;
            ImgControl.ImgOk = false;
        }
    }
    if (DCMI->MISR & DCMI_IT_LINE)
    {
        DCMI->ICR = DCMI_IT_LINE;
		
        if(ImgControl.ImgOk == false)
        {	
            if(ImgControl.lines < CAMERA_H)
            {	
				        DMA2_Stream1->CR  &= (uint32_t)~DMA_SxCR_EN; //DMA�ɼ�ͼ��ÿ�ı�һ�ε�ַ����Ҫ��DMAʧ�ܺ���ܸı�		 
                DMA2_Stream1->M0AR = (uint32_t)&(Image[ImgControl.lines]);	 //�µ�һ�еĵ�ַ
                DMA2_Stream1->CR  |=  DMA_SxCR_EN;					  //DMA2_Stream1 enable
                DCMI->CR |=  DCMI_CR_CAPTURE;						 //capture enable
                ImgControl.lines++;
            }
            else
            {
                ImgControl.ImgOk = true;  
                ImgControl.ImgDealOk = false;
            }
        }
    }

    if (DCMI->MISR & DCMI_IT_FRAME)
    {
        DCMI->ICR = DCMI_IT_FRAME;
    }
    if (DCMI->MISR &DCMI_IT_ERR)
    {
        DCMI->ICR = DCMI_IT_ERR;
    }
}
