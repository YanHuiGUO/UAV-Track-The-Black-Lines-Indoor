#include "include.h"
void All_Init()
{
	NVIC_PriorityGroupConfig(NVIC_GROUP);		//�ж����ȼ��������
	
	SysTick_Configuration(); 	//�δ�ʱ��
	
	I2c_Soft_Init();					//��ʼ��ģ��I2C

	PWM_Out_Init(400);				//��ʼ������������
	
//	MS5611_Init();						//��ѹ�Ƴ�ʼ��
	
	Delay_ms(200);						//��ʱ
	
	MPU6050_Init(20);   			//���ٶȼơ������ǳ�ʼ��������20hz��ͨ
	
	LED_Init();								//LED���ܳ�ʼ��
	
	Usart2_Init(115200);			//����2��ʼ������������Ϊ������
	
	Usart1_Init(115200);      //����ͷ�崮��
	
	Ultra_Init(9600); 				//��������ʼ��
	
	OLED_Init();
//	
	Nrf_Init();
	RX_Mode();
//	//TIM_INIT();
	
	PID_Para_Init();							//������ʼ��
	
	mpu6050.Acc_CALIBRATE = 1;
	mpu6050.Gyro_CALIBRATE = 1;
	Delay_ms(1000);						//��ʱ
}