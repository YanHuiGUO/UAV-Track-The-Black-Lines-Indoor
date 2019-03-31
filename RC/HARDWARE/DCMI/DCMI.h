#ifndef __DCMI_H
#define __DCMI_H

#include "Common.h"
/*图像处理结构体*/		 
typedef struct
{
 bool  ImgDealOk;
 bool  ImgOk;
 bool  InitStatu;
 s16   lines;
}CamCtrlClass;
/*寄存器*/
typedef struct
{
	u8 Address;			       /*寄存器地址*/
	u8 Value;		           /*寄存器值*/
}Register_Info;
/*图像尺寸设置*/
#define	CAMERA_DMA_CH 	    DMA_CH0		    //定义摄像头的DMA采集通道
#define CAMERA_W            240           	//定义摄像头图像宽度
#define CAMERA_H            200           //定义摄像头图像高度
#define CAMERA_SIZE         (CAMERA_W * CAMERA_H)
#define false 0
#define true  1

#define DCMI_DR_ADDRESS     0x50050028
/****************摄像头IIC宏定义---->*********************/

#define   SCLPin  8
#define   SDAPin  7

#define SDA_IN         GPIOA->MODER &=~(GPIO_MODER_MODER0<<(2*SDAPin)); GPIOA->MODER |= (GPIO_Mode_IN  << (SDAPin * 2));
#define SDA_OUT        GPIOA->MODER &=~(GPIO_MODER_MODER0<<(2*SDAPin)); GPIOA->MODER |= (GPIO_Mode_OUT << (SDAPin * 2));	         
#define SCL_OUT        GPIOA->MODER &=~(GPIO_MODER_MODER0<<(2*SCLPin)); GPIOA->MODER |= (GPIO_Mode_OUT << (SCLPin * 2));

#define SCL_HIGH       GPIOA->BSRRL = 1<<SCLPin  
#define SCL_LOW        GPIOA->BSRRH = 1<<SCLPin  

#define SDA_HIGH       GPIOA->BSRRL = 1<<SDAPin 
#define SDA_LOW        GPIOA->BSRRH = 1<<SDAPin 

#define SDA_DATA       (GPIOA->IDR&(1<<SDAPin))

/***************<---------------*************************/

extern unsigned char Image[CAMERA_H][CAMERA_W];
extern CamCtrlClass ImgControl;

extern	u8 Ov7620_Init(void);
void DCMIInit(void);
void MCO1_Init(void);
void DCMI_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
u8   Ov7620_reg_Init(void);
void sccb_init(void);                           //初始化SCCB端口为GPIO
void sccb_wait(void);                           //SCCB时序延时
void sccb_start(void);                          //起始标志
void sccb_stop(void);                           //停止标志
u8 sccb_sendByte(u8 data);
u8 sccb_regWrite(u8 device,u8 address,u8 data);
extern double DCMI_temp;

#endif
