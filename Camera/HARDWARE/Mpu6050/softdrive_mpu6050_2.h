#ifndef _SOFTDRIVE_MPU6050_2_H
#define _SOFTDRIVE_MPU6050_2_H
#include "stm32f4xx.h"
#include "Ano_I2c_2.h"
#include  "softdrive_mpu6050.h"
#include "common.h"

extern MPU6050_STRUCT mpu6050_2;
extern float mpu6050_tmp_2[ITEMS];
void MPU6050_Init_2(u16);
void MPU6050_Read_2(void);

void MPU6050_Data_Prepare_2(float T);

#endif


