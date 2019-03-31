#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "delay.h"
#include "common.h"
#include "OLED.h"
#include "UartLink.h"
#include "stm32f4xx_it.h"
#include "MyNvic.h"
#include "DCMI.h"
#include "IIC.h"
#include "led.h" 
#include "LedBuz.h"
#include "timer.h"
#include "pwm.h"
#include "QEI.h"
#include "dmp_driver.h"
#include "dmp_mpu6050.h"
#include "TeeterBoard.h"
#include "key_matrix.h"
#include "key2oled.h"
#include "nrf24l01.h"
#include "ultrasonic.h"
#include "RC.h"
#include "Ano_I2c.h"
#include "softdrive_mpu6050.h"
#include "softdrive_mpu6050_2.h"
#include "ANO_Uart.h"
#include "imu.h"
#include "Out_thr.h"
#include "MyTask.h"
extern char flag_30ms;	
char isad = 1;
int PWM_test = 0;
void SendCamera(void);
unsigned char tmp_buf[8];
int main(void)
 {  
	DisableInterrupts;

	delay_init(168);
	TIM5_Init(84,10000);
	TIM7_Init(84,1000); 
	TIM13_Init(84,0xffff);
//	TIM14_Init(84,0xffff);
//	LED_Init();
//	BuzLed_Init();
	//Buzzer_ON;
	Key_Init();
//  UartInit(USART2);//FreeCars上位机串口初始化

//	RC_Init();  //遥控器初始化
	
//		PWM_Init(17000);
//  	PID_Init();
//	Balance_PID_Init();
//	  QEI_Init();
	 
		//Ov7620_Init();
		
//    I2c_Soft_Init();	
//  	MPU6050_Init(20);
//		abEncoder_Init();
		OLED_Init();
//	I2c_Soft_Init_2();	
//  MPU6050_Init_2(20); 
//  Ultran_Init();	
  	Nrf_Init();
		TX_Mode();
	//Usart2_Init(115200);//匿名上位机初始化
	//	while(!Ov7620_Init());//判断OV7620初始化是否成功 
	//MPU6050DMP_Init();

	/*设置中断优先级*/
//	Buzzer_OFF; 
	MyNVIC_Init(NVIC_PriorityGroup_2,1,0,USART2_IRQn,ENABLE);	 
	EnableInterrupts;
	//PWM通道，3>4顺时针转
	//ref.g.x  逆正，顺负
	//面对杆子,左起为负，右起为正
	while(1)
	{	
			Key_deal();

	}
}
void SendCamera(void)
{
  	uint32_t i,j;
	  uint8_t d;
		uart_putchar(MYUART,0xFE);
	for(i = 0; i < CAMERA_H; i++)
	{ 
		for(j = 0; j < CAMERA_W; j++)
		{
	   d=Image[i][j];
			if(d > 0xFD) d = 0xFD;         
    uart_putchar(MYUART,d);
		}
	
	}
}
