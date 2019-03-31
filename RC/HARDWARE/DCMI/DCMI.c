#include "DCMI.h"
#include "delay.h"
#include "MyNVIC.h"
#include "OV7620_REG.h"
#include "UartLink.h"

CamCtrlClass ImgControl;

u8 Image[CAMERA_H][CAMERA_W]= {0};

/************************************************************************
* @ 函数名称：sccb_wait
* @ 功能说明：SCCB延时，不应太小
* @ 参数说明：无
* @ 函数返回：无
*************************************************************************/
void sccb_wait(void)
{
	delay_us(10);
}
/************************************************************************
* @ 函数名称：sccb_start
* @ 功能说明：SCCB启动位
* @ 参数说明：无
* @ 函数返回：无
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
* @ 函数名称：sccb_stop
* @ 功能说明：SCCB停止位
* @ 参数说明：无
* @ 函数返回：无
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
* @ 函数名称：sccb_sendByte
* @ 功能说明：在SCCB总线上发送一个字节
* @ 参数说明：data 要发送的字节内容
* @ 函数返回：无
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
* @ 函数名称：sccb_regWrite
* @ 功能说明：通过SCCB总线向指定设备的指定地址发送指定内容
* @ 参数说明：device---设备号  读写有区别
*            address---写数据的寄存器
*            data---写的内容
* @ 函数返回：ack=1未收到应答(失败)    ack=0收到应答(成功)
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
* @ 函数名称： IICInit(void)
* @ 功能说明： IIC引脚初始化
* @ 参数说明： 无
* @ 函数返回： 无
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
* @ 函数名称： ov7620_reg[]
* @ 功能说明： ov7620初始化配置
* @ 参数说明： 无
* @ 函数返回： 无
*************************************************************************/
Register_Info ov7620_reg[] =
{
#if 1  //QVGA
    //寄存器，寄存器值次
    {CLKRC     ,0x00},  //分频
    {COMC      ,0x24},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7A-CAMERA_W/2/2)},//截取图像，图像校正
    {HSIZE     ,(0x7A+CAMERA_W/2/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)},
    {VEEE      ,0x42},  //垂直边缘增强
    {COMH      ,0x00},     //扫描模式
#else //VGA
    //寄存器，寄存器值次
    {CLKRC     ,0x00}, 
    //{COMC      ,0x04},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/4/2)},
    {HSIZE     ,(0x7F+CAMERA_W/4/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    //{VEEE      ,0x42},  //垂直边缘增强
    //{COMH      ,0x20},  //扫描模式
#endif

};

u8 cfgnum = sizeof(ov7620_reg)/sizeof(ov7620_reg[0]);   /*结构体数组成员数目*/

/************************************************
 * @ 函数名：Ov7620_Init
 * @ 描述  ：Sensor初始化
 * @ 输入  ：无
 * @ 输出  ：返回1成功，返回0失败
 * @ 注意  ：无
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
 * @  函数名：Ov7620_reg_Init
 * @ 描述  ：Sensor 寄存器 初始化
 * @ 输入  ：无
 * @ 输出  ：返回1成功，返回0失败
 * @ 注意  ：无
 ************************************************/
u8 Ov7620_reg_Init(void)
{
    u16 i = 0;
    while( 1 == sccb_regWrite ( 0x42, 0x12, 0xA4 ) ) /*复位sensor */
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
  * @函数名称   DCMI_Config
  * @函数说明   DCMI接口配置
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void DCMIInit(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    DCMI_InitTypeDef DCMI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);   /* 使能 DCMI 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   /* 使能 DMA2 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE); /* 使能 DCMI GPIOs 时钟 */

    /* DCMI 引脚初始化 */
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

    DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //连续捕获模式
    DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;     //硬件同步模式
    DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;       //像素时钟上沿Falling
    DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;           //帧同步上沿
    DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;            //水平同步下沿//上沿High
    DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;    //整个帧图像
    DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //每次数据8位

    DCMI_Init(&DCMI_InitStructure);
		
    DCMI_ITConfig(DCMI_IT_VSYNC,ENABLE);
    DCMI_ITConfig(DCMI_IT_LINE,ENABLE);

    MyNVIC_Init(NVIC_PriorityGroup_2,0,1,DCMI_IRQn,ENABLE);	//设置中断优先级
   
    DCMI_Cmd(ENABLE);
    DCMI_CaptureCmd(ENABLE);

    /* Configures the DMA2 to transfer Data from DCMI */
    DMA_DeInit(DMA2_Stream1);
    while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
    {
    }
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;                 //照相机接口数据
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&(Image[0]);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //设备到内存
    DMA_InitStructure.DMA_BufferSize = CAMERA_W;                                //每次传输数据大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;         //word设备端一次传送4个字节32位
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
  * @函数名称   MCO1_Init(void)
  * @函数说明   输出一个时钟信号到PTA8
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
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
  * @函数名称   DCMI_IRQHandler(void)
  * @函数说明   利用DMA进行图像采集
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
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
				        DMA2_Stream1->CR  &= (uint32_t)~DMA_SxCR_EN; //DMA采集图像；每改变一次地址是需要将DMA失能后才能改变		 
                DMA2_Stream1->M0AR = (uint32_t)&(Image[ImgControl.lines]);	 //新的一行的地址
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
