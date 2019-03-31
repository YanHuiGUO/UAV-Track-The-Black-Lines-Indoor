#ifndef _ANO_I2C__2_H
#define	_ANO_I2C__2_H

#include "stm32f4xx.h"


#define SCL_H_2        ANO_GPIO_I2C_2->BSRRL = I2C_Pin_SCL_2
#define SCL_L_2         ANO_GPIO_I2C_2->BSRRH = I2C_Pin_SCL_2
#define SDA_H_2        ANO_GPIO_I2C_2->BSRRL = I2C_Pin_SDA_2
#define SDA_L_2         ANO_GPIO_I2C_2->BSRRH = I2C_Pin_SDA_2
#define SCL_read_2      ANO_GPIO_I2C_2->IDR  & I2C_Pin_SCL_2
#define SDA_read_2      ANO_GPIO_I2C_2->IDR  & I2C_Pin_SDA_2

/***************I2C GPIO∂®“Â******************/
#define ANO_GPIO_I2C_2	GPIOD
#define I2C_Pin_SCL_2		GPIO_Pin_13
#define I2C_Pin_SDA_2		GPIO_Pin_12
#define ANO_RCC_I2C_2		RCC_AHB1Periph_GPIOD
/*********************************************/
extern volatile u8 I2C_FastMode_2;

void I2c_Soft_Init_2(void);
void I2c_Soft_SendByte_2(u8 SendByte);
u8 I2c_Soft_ReadByte_2(u8);

//int I2c_Soft_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
//int I2c_Soft_Single_Read(u8 SlaveAddress,u8 REG_Address);
//int I2c_Soft_Mult_Read(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size);

u8 IIC_Write_1Byte_2(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
u8 IIC_Read_1Byte_2(u8 SlaveAddress,u8 REG_Address,u8 *REG_data);
u8 IIC_Write_nByte_2(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);
u8 IIC_Read_nByte_2(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);

#endif
