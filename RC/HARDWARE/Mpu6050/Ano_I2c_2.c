/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * 作者   ：匿名科创
 * 文件名  ：i2c_soft.c
 * 描述    ：软件模拟i2c通信
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/
#include "Ano_I2c_2.h"

volatile u8 I2C_FastMode_2;

void I2c_Soft_delay_2()
{ 
	__nop();__nop();__nop();
	__nop();__nop();__nop();
	__nop();__nop();__nop();
	
	if(!I2C_FastMode_2)
	{
		u8 i = 15;
		while(i--);
	}
}

void I2c_Soft_Init_2()
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_AHB1PeriphClockCmd(ANO_RCC_I2C_2 , ENABLE );
  GPIO_InitStructure.GPIO_Pin =  I2C_Pin_SCL_2 | I2C_Pin_SDA_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ANO_GPIO_I2C_2, &GPIO_InitStructure);		
}

int I2c_Soft_Start_2()
{
	SDA_H_2;
	SCL_H_2;
	I2c_Soft_delay_2();
	if(!SDA_read_2)return 0;	//SDA线为低电平则总线忙,退出
	SDA_L_2;
	I2c_Soft_delay_2();
	if(SDA_read_2) return 0;	//SDA线为高电平则总线出错,退出
	SDA_L_2;
	I2c_Soft_delay_2();
	return 1;	

}

void I2c_Soft_Stop_2()
{
	SCL_L_2;
	I2c_Soft_delay_2();
	SDA_L_2;
	I2c_Soft_delay_2();
	SCL_H_2;
	I2c_Soft_delay_2();
	SDA_H_2;
	I2c_Soft_delay_2();
}

void I2c_Soft_Ask_2()
{
	SCL_L_2;
	I2c_Soft_delay_2();
	SDA_L_2;
	I2c_Soft_delay_2();
	SCL_H_2;
	I2c_Soft_delay_2();
	SCL_L_2;
	I2c_Soft_delay_2();
}

void I2c_Soft_NoAsk_2()
{
	SCL_L_2;
	I2c_Soft_delay_2();
	SDA_H_2;
	I2c_Soft_delay_2();
	SCL_H_2;
	I2c_Soft_delay_2();
	SCL_L_2;
	I2c_Soft_delay_2();
}

int I2c_Soft_WaitAsk_2(void) 	 //返回为:=1无ASK,=0有ASK
{
  u8 ErrTime = 0;
	SCL_L_2;
	I2c_Soft_delay_2();
	SDA_H_2;			
	I2c_Soft_delay_2();
	SCL_H_2;
	I2c_Soft_delay_2();
	while(SDA_read_2)
	{
		ErrTime++;
		if(ErrTime>50)
		{
			I2c_Soft_Stop_2();
			return 1;
		}
	}
	SCL_L_2;
	I2c_Soft_delay_2();
	return 0;
}

void I2c_Soft_SendByte_2(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L_2;
        I2c_Soft_delay_2();
      if(SendByte&0x80)
        SDA_H_2;  
      else 
        SDA_L_2;   
        SendByte<<=1;
        I2c_Soft_delay_2();
				SCL_H_2;
				I2c_Soft_delay_2();
    }
    SCL_L_2;
}  

//读1个字节，ack=1时，发送ACK，ack=0，发送NACK
u8 I2c_Soft_ReadByte_2(u8 ask)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H_2;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L_2;
      I2c_Soft_delay_2();
			SCL_H_2;
      I2c_Soft_delay_2();	
      if(SDA_read_2)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L_2;

	if (ask)
		I2c_Soft_Ask_2();
	else
		I2c_Soft_NoAsk_2();  
    return ReceiveByte;
} 


// IIC写一个字节数据
u8 IIC_Write_1Byte_2(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1);   
	if(I2c_Soft_WaitAsk_2())
	{
		I2c_Soft_Stop_2();
		return 1;
	}
	I2c_Soft_SendByte_2(REG_Address);       
	I2c_Soft_WaitAsk_2();	
	I2c_Soft_SendByte_2(REG_data);
	I2c_Soft_WaitAsk_2();   
	I2c_Soft_Stop_2(); 
	return 0;
}

// IIC读1字节数据
u8 IIC_Read_1Byte_2(u8 SlaveAddress,u8 REG_Address,u8 *REG_data)
{      		
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk_2())
	{
		I2c_Soft_Stop_2();
		return 1;
	}
	I2c_Soft_SendByte_2(REG_Address);     
	I2c_Soft_WaitAsk_2();
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1 | 0x01);
	I2c_Soft_WaitAsk_2();
	*REG_data= I2c_Soft_ReadByte_2(0);
	I2c_Soft_Stop_2();
	return 0;
}	

// IIC写n字节数据
u8 IIC_Write_nByte_2(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{	
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk_2())
	{
		I2c_Soft_Stop_2();
		return 1;
	}
	I2c_Soft_SendByte_2(REG_Address); 
	I2c_Soft_WaitAsk_2();
	while(len--) 
	{
		I2c_Soft_SendByte_2(*buf++); 
		I2c_Soft_WaitAsk_2();
	}
	I2c_Soft_Stop_2();
	return 0;
}

// IIC读n字节数据
u8 IIC_Read_nByte_2(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{	
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk_2())
	{
		I2c_Soft_Stop_2();
		return 1;
	}
	I2c_Soft_SendByte_2(REG_Address); 
	I2c_Soft_WaitAsk_2();
	
	I2c_Soft_Start_2();
	I2c_Soft_SendByte_2(SlaveAddress<<1 | 0x01); 
	I2c_Soft_WaitAsk_2();
	while(len) 
	{
		if(len == 1)
		{
			*buf = I2c_Soft_ReadByte_2(0);
		}
		else
		{
			*buf = I2c_Soft_ReadByte_2(1);
		}
		buf++;
		len--;
	}
	I2c_Soft_Stop_2();
	return 0;
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

