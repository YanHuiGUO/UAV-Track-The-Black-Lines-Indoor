/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��mpu6050.c
 * ����    ��6�ᴫ����mpu6050����
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/

#include "softdrive_mpu6050_2.h"


MPU6050_STRUCT mpu6050_2;

u8 mpu6050_buffer_2[14];
#define FLI_RANGE_2 100.0f
float J_FLI_2[ITEMS];
void MPU6050_Read_2(void)
{
	I2C_FastMode_2 = 1;
	IIC_Read_nByte_2(MPU6050_ADDR,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer_2);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	  �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
ʧ��Ϊ0
*******************************************************************************/ 
void IICwriteBit_2(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	IIC_Read_nByte_2(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	IIC_Write_1Byte_2(dev, reg, b);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
bitStart  Ŀ���ֽڵ���ʼλ
length   λ����
data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
ʧ��Ϊ0
*******************************************************************************/ 
void IICwriteBits_2(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	
	u8 b,mask;
	IIC_Read_nByte_2(dev, reg, 1, &b);
	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b &= mask;
	b |= data;
	IIC_Write_1Byte_2(dev, reg, b);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		
*��������:	    ���� ������
*******************************************************************************/
void MPU6050_set_SMPLRT_DIV_2(uint16_t hz)
{
	IIC_Write_1Byte_2(MPU6050_ADDR, MPU6050_RA_SMPLRT_DIV,1000/hz - 1);
		//I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
* CLK_SEL | Clock Source
* --------+--------------------------------------
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource_2(uint8_t source)
{
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}
/** Set full-scale gyroscope range.
* @param range New full-scale gyroscope range value
* @see getFullScaleRange()
* @see MPU6050_GYRO_FS_250
* @see MPU6050_RA_GYRO_CONFIG
* @see MPU6050_GCONFIG_FS_SEL_BIT
* @see MPU6050_GCONFIG_FS_SEL_LENGTH
*/
void MPU6050_setFullScaleGyroRange_2(uint8_t range) {
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG,7, 3, 0x00);   //���Լ�
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange_2(uint8_t range) {
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG,7, 3, 0x00);   //���Լ�
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
enabled =1   ˯��
enabled =0   ����
*******************************************************************************/
void MPU6050_setSleepEnabled_2(uint8_t enabled) {
	IICwriteBit_2(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled_2(uint8_t enabled) {
	IICwriteBit_2(MPU6050_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled_2(uint8_t enabled) {
	IICwriteBit_2(MPU6050_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		
*��������:	    ���õ�ͨ�˲���ֹƵ��
*******************************************************************************/
void MPU6050_setDLPF_2(uint8_t mode)
{
	IICwriteBits_2(MPU6050_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_Init_2(u16 lpf)
{ 
	u16 default_filter = 1;
	
	//MPU6050_INT_Config();
	
  switch(lpf)
	{
	case 5:
			default_filter = MPU6050_DLPF_BW_5;
			break;
	case 10:
			default_filter = MPU6050_DLPF_BW_10;
			break;
	case 20:
			default_filter = MPU6050_DLPF_BW_20;
			break;
	case 42:
			default_filter = MPU6050_DLPF_BW_42;
			break;
	case 98:
			default_filter = MPU6050_DLPF_BW_98;
			break;
	case 188:
			default_filter = MPU6050_DLPF_BW_188;
			break;
	case 256:
			default_filter = MPU6050_DLPF_BW_256;
			break;
	default:
			default_filter = MPU6050_DLPF_BW_42;
			break;
	}
	I2c_Soft_Init_2();

	//�豸��λ
//	IIC_Write_1Byte(MPU6050_ADDR,MPU6050_RA_PWR_MGMT_1, 0x80);
	
	MPU6050_setSleepEnabled_2(0); //���빤��״̬
	delay_ms(10);
	MPU6050_setClockSource_2(MPU6050_CLOCK_PLL_ZGYRO); //����ʱ��  0x6b   0x03
	delay_ms(10);
	MPU6050_set_SMPLRT_DIV_2(1000);  //1000hz
	delay_ms(10);
	MPU6050_setFullScaleGyroRange_2(MPU6050_GYRO_FS_2000);//������������� +-2000��ÿ��
	delay_ms(10);
	MPU6050_setFullScaleAccelRange_2(MPU6050_ACCEL_FS_8);	//���ٶȶ�������� +-8G
	delay_ms(10);
	MPU6050_setDLPF_2(default_filter);  //42hz
	delay_ms(10);
	MPU6050_setI2CMasterModeEnabled_2(0);	 //����MPU6050 ����AUXI2C
	delay_ms(10);
	MPU6050_setI2CBypassEnabled_2(1);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
	delay_ms(10);
}

s32 sum_temp_2[7]={0,0,0,0,0,0,0};
u16 acc_sum_cnt_2 = 0,gyro_sum_cnt_2 = 0;

void MPU6050_Data_Offset_2()//���ڳ�ʼ�����ֻ���ÿ�����ǰ
{


	if(mpu6050_2.Acc_CALIBRATE == 1)
	{
    acc_sum_cnt_2++;
		sum_temp_2[A_X] += mpu6050_2.Acc_I16.x;
		sum_temp_2[A_Y] += mpu6050_2.Acc_I16.y;
		sum_temp_2[A_Z] += mpu6050_2.Acc_I16.z - 65536/16;   // +-8G
		sum_temp_2[TEM] += mpu6050_2.Tempreature;

    if( acc_sum_cnt_2 >= OFFSET_AV_NUM )
		{
			mpu6050_2.Acc_Offset.x = sum_temp_2[A_X]/OFFSET_AV_NUM;
			mpu6050_2.Acc_Offset.y = sum_temp_2[A_Y]/OFFSET_AV_NUM;
			mpu6050_2.Acc_Offset.z = sum_temp_2[A_Z]/OFFSET_AV_NUM;
			mpu6050_2.Temprea_Offset = sum_temp_2[TEM]/OFFSET_AV_NUM;
			acc_sum_cnt_2 =0;
			mpu6050_2.Acc_CALIBRATE = 0;
			//Param_SaveAccelOffset(&mpu6050.Acc_Offset);
			sum_temp_2[A_X] = sum_temp_2[A_Y] = sum_temp_2[A_Z] = sum_temp_2[TEM] = 0;
		}	
	}


	if(mpu6050_2.Gyro_CALIBRATE)
	{
		gyro_sum_cnt_2++;
		sum_temp_2[G_X] += mpu6050_2.Gyro_I16.x;
		sum_temp_2[G_Y] += mpu6050_2.Gyro_I16.y;
		sum_temp_2[G_Z] += mpu6050_2.Gyro_I16.z;
		sum_temp_2[TEM] += mpu6050_2.Tempreature;

    if( gyro_sum_cnt_2 >= OFFSET_AV_NUM )
		{
			mpu6050_2.Gyro_Offset.x = (float)sum_temp_2[G_X]/OFFSET_AV_NUM;
			mpu6050_2.Gyro_Offset.y = (float)sum_temp_2[G_Y]/OFFSET_AV_NUM;
			mpu6050_2.Gyro_Offset.z = (float)sum_temp_2[G_Z]/OFFSET_AV_NUM;
			mpu6050_2.Temprea_Offset = sum_temp_2[TEM]/OFFSET_AV_NUM;
			gyro_sum_cnt_2 =0;
			if(mpu6050_2.Gyro_CALIBRATE == 1)
			//	Param_SaveGyroOffset(&mpu6050.Gyro_Offset);
			mpu6050_2.Gyro_CALIBRATE = 0;
			sum_temp_2[G_X] = sum_temp_2[G_Y] = sum_temp_2[G_Z] = sum_temp_2[TEM] = 0;
		}
	}
}



s16 FILT_BUF_2[ITEMS][(FILTER_NUM + 1)];
uint8_t filter_cnt_2 = 0,filter_cnt_old_2 = 0;

float mpu6050_tmp_2[ITEMS];
float tmp_2[ITEMS];
float J_FLI_2[ITEMS];
float test_ang_2 =0,test_ang_old_2=0,test_ang_d_2,test_fli_a_2,test_i_2;

void MPU6050_Data_Prepare_2(float T)
{	
	u8 i;
	s32 FILT_TMP[ITEMS] = {0,0,0,0,0,0,0};
//	float auto_offset_temp[3];
  float Gyro_tmp[3];
	

	MPU6050_Data_Offset_2(); //У׼����

	/*��ȡbufferԭʼ����*/
	mpu6050_2.Acc_I16.x = ((((int16_t)mpu6050_buffer_2[0]) << 8) | mpu6050_buffer_2[1]) ;
	mpu6050_2.Acc_I16.y = ((((int16_t)mpu6050_buffer_2[2]) << 8) | mpu6050_buffer_2[3]) ;
	mpu6050_2.Acc_I16.z = ((((int16_t)mpu6050_buffer_2[4]) << 8) | mpu6050_buffer_2[5]) ;
 
	mpu6050_2.Gyro_I16.x = ((((int16_t)mpu6050_buffer_2[ 8]) << 8) | mpu6050_buffer_2[ 9]) ;
	mpu6050_2.Gyro_I16.y = ((((int16_t)mpu6050_buffer_2[10]) << 8) | mpu6050_buffer_2[11]) ;
	mpu6050_2.Gyro_I16.z = ((((int16_t)mpu6050_buffer_2[12]) << 8) | mpu6050_buffer_2[13]) ;

	Gyro_tmp[0] = mpu6050_2.Gyro_I16.x ;//
  Gyro_tmp[1] = mpu6050_2.Gyro_I16.y ;//
	Gyro_tmp[2] = mpu6050_2.Gyro_I16.z ;//

	mpu6050_2.Tempreature = ((((int16_t)mpu6050_buffer_2[6]) << 8) | mpu6050_buffer_2[7]); //tempreature
	mpu6050_2.TEM_LPF += 2 *3.14f *T *(mpu6050_2.Tempreature - mpu6050_2.TEM_LPF);
	mpu6050_2.Ftempreature = mpu6050_2.TEM_LPF/340.0f + 36.5f;

//======================================================================
	if( ++filter_cnt_2 > FILTER_NUM )	
	{
		filter_cnt_2 = 0;
		
	}
	else
	{
		filter_cnt_old_2 = (filter_cnt_2 == FILTER_NUM)? 0 : (filter_cnt_2 + 1);
	}
//10 170 4056
	/* �ó�У׼������� */
	mpu6050_tmp_2[A_X] = (mpu6050_2.Acc_I16.x - mpu6050_2.Acc_Offset.x) ;
	mpu6050_tmp_2[A_Y] = (mpu6050_2.Acc_I16.y - mpu6050_2.Acc_Offset.y) ;
	mpu6050_tmp_2[A_Z] = (mpu6050_2.Acc_I16.z - mpu6050_2.Acc_Offset.z) ;
	mpu6050_tmp_2[G_X] = Gyro_tmp[0] - mpu6050_2.Gyro_Offset.x ;//
	mpu6050_tmp_2[G_Y] = Gyro_tmp[1] - mpu6050_2.Gyro_Offset.y ;//
	mpu6050_tmp_2[G_Z] = Gyro_tmp[2] - mpu6050_2.Gyro_Offset.z ;//
	

	/* �����˲������������� */
	FILT_BUF_2[A_X][filter_cnt_2] = mpu6050_tmp_2[A_X];
	FILT_BUF_2[A_Y][filter_cnt_2] = mpu6050_tmp_2[A_Y];
	FILT_BUF_2[A_Z][filter_cnt_2] = mpu6050_tmp_2[A_Z];
	FILT_BUF_2[G_X][filter_cnt_2] = mpu6050_tmp_2[G_X]; 
	FILT_BUF_2[G_Y][filter_cnt_2] = mpu6050_tmp_2[G_Y];
	FILT_BUF_2[G_Z][filter_cnt_2] = mpu6050_tmp_2[G_Z];

	for(i=0;i<FILTER_NUM;i++)
	{
		FILT_TMP[A_X] += FILT_BUF_2[A_X][i];
		FILT_TMP[A_Y] += FILT_BUF_2[A_Y][i];
		FILT_TMP[A_Z] += FILT_BUF_2[A_Z][i];
		FILT_TMP[G_X] += FILT_BUF_2[G_X][i];
		FILT_TMP[G_Y] += FILT_BUF_2[G_Y][i];
		FILT_TMP[G_Z] += FILT_BUF_2[G_Z][i];
	}


	tmp_2[A_X] = (float)( FILT_TMP[A_X] )/(float)FILTER_NUM;
	tmp_2[A_Y] = (float)( FILT_TMP[A_Y] )/(float)FILTER_NUM;
	tmp_2[A_Z] = (float)( FILT_TMP[A_Z] )/(float)FILTER_NUM;


	tmp_2[G_X] = (float)( FILT_TMP[G_X] )/(float)FILTER_NUM;
	tmp_2[G_Y] = (float)( FILT_TMP[G_Y] )/(float)FILTER_NUM;
	tmp_2[G_Z] = (float)( FILT_TMP[G_Z] )/(float)FILTER_NUM;
	
//	J_FLI[A_X] +=  5 *3.14f *T *(mpu6050_tmp[A_X] - tmp[A_X] - J_FLI[A_X]);
// 	J_FLI[A_X] = LIMIT(J_FLI[A_X], -FLI_RANGE, FLI_RANGE );	
// 	tmp[A_X] += 3.14f *(5 + 15*ABS(J_FLI[A_X])/FLI_RANGE) *T  *( mpu6050_tmp[A_X] - tmp[A_X] );
// 	
// 	J_FLI[A_Y] +=  5 *3.14f *T *(mpu6050_tmp[A_Y] - tmp[A_Y] - J_FLI[A_Y]);
// 	J_FLI[A_Y] = LIMIT(J_FLI[A_Y], -FLI_RANGE, FLI_RANGE );	
// 	tmp[A_Y] += 3.14f *(5 + 15*ABS(J_FLI[A_Y])/FLI_RANGE) *T  *( mpu6050_tmp[A_Y] - tmp[A_Y] );
// 	
// 	J_FLI[A_Z] +=  5 *3.14f *T *(mpu6050_tmp[A_Z] - tmp[A_Z] - J_FLI[A_Z] );
// 	J_FLI[A_Z] = LIMIT(J_FLI[A_Z], -FLI_RANGE, FLI_RANGE );	
// 	tmp[A_Z] += 3.14f *(5 + 15*ABS(J_FLI[A_Z])/FLI_RANGE) *T  *( mpu6050_tmp[A_Z] - tmp[A_Z] );
// 	
// 	J_FLI[G_X] +=  10 *3.14f *T *(mpu6050_tmp[G_X] - tmp[G_X] - J_FLI[G_X]);
// 	J_FLI[G_X] = LIMIT(J_FLI[G_X], -FLI_RANGE, FLI_RANGE );	
// 	tmp[G_X] += 3.14f *(10 + 10*ABS(J_FLI[G_X])/FLI_RANGE) *T  *( mpu6050_tmp[G_X] - tmp[G_X] );
// 	
// 	J_FLI[G_Y] +=  10 *3.14f *T *(mpu6050_tmp[G_Y] - tmp[G_Y] - J_FLI[G_Y]);
// 	J_FLI[G_Y] = LIMIT(J_FLI[G_Y], -FLI_RANGE, FLI_RANGE );	
// 	tmp[G_Y] += 3.14f *(10 + 10*ABS(J_FLI[G_Y])/FLI_RANGE) *T  *( mpu6050_tmp[G_Y] - tmp[G_Y] );
// 	
// 	J_FLI[G_Z] +=  10 *3.14f *T *(mpu6050_tmp[G_Z] - tmp[G_Z] - J_FLI[G_Z]);
// 	J_FLI[G_Z] = LIMIT(J_FLI[G_Z], -FLI_RANGE, FLI_RANGE );	
// 	tmp[G_Z] += 3.14f *(10 + 10*ABS(J_FLI[G_Z])/FLI_RANGE) *T  *( mpu6050_tmp[G_Z] - tmp[G_Z] );
//	
	
	/*����ת��*/
	Transform(tmp_2[A_X],tmp_2[A_Y],tmp_2[A_Z],&mpu6050_2.Acc.x,&mpu6050_2.Acc.y,&mpu6050_2.Acc.z);
	Transform(tmp_2[G_X],tmp_2[G_Y],tmp_2[G_Z],&mpu6050_2.Gyro.x,&mpu6050_2.Gyro.y,&mpu6050_2.Gyro.z);

	mpu6050_2.Gyro_deg.x = mpu6050_2.Gyro.x *TO_ANGLE;
	mpu6050_2.Gyro_deg.y = mpu6050_2.Gyro.y *TO_ANGLE;
	mpu6050_2.Gyro_deg.z = mpu6050_2.Gyro.z *TO_ANGLE;
	



	
//======================================================================
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
