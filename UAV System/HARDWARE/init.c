#include "include.h"
void All_Init()
{
	NVIC_PriorityGroupConfig(NVIC_GROUP);		//中断优先级组别设置
	
	SysTick_Configuration(); 	//滴答时钟
	
	I2c_Soft_Init();					//初始化模拟I2C

	PWM_Out_Init(400);				//初始化电调输出功能
	
//	MS5611_Init();						//气压计初始化
	
	Delay_ms(200);						//延时
	
	MPU6050_Init(20);   			//加速度计、陀螺仪初始化，配置20hz低通
	
	LED_Init();								//LED功能初始化
	
	Usart2_Init(115200);			//串口2初始化，函数参数为波特率
	
	Usart1_Init(115200);      //摄像头板串口
	
	Ultra_Init(9600); 				//超声波初始化
	
	OLED_Init();
//	
	Nrf_Init();
	RX_Mode();
//	//TIM_INIT();
	
	PID_Para_Init();							//参数初始化
	
	mpu6050.Acc_CALIBRATE = 1;
	mpu6050.Gyro_CALIBRATE = 1;
	Delay_ms(1000);						//延时
}