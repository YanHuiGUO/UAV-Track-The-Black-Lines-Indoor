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
#include "DealCamera.h"
void SendToMcu(void);
void SendCamera(void);
float LaserDot 	= 0;
float My_X,My_Y = 0;

#define LineWidth 10//50CM�����߿�궨ֵ
#define CrossWidth 30
#define CircleWidth 20
#define LookField   50 //��ʴ�뾶
#define JudgeKind(n,min,max) ((n>=min && n<max) ? 1 : 0)
struct 
{
	//�д���ʱ�ļ�¼������������ÿ������
	int W_Start;//ÿ���߿���ʼ����
	int W_End;//ÿ���߿���ֹ����
	int W_Width[CAMERA_H];//�߿�
	int W_Mid[CAMERA_H];//�м�ֵ
	
	char W_StartFlag;//������ʼ��־
	char W_EndFlag;//������ֹ��־
	
	int H_Start;//ÿ���߿���ʼ����
	int H_End;//ÿ���߿���ֹ����
	int H_Width[CAMERA_W];//�߿�
	int H_Mid[CAMERA_W];//�м�ֵ
	
	char H_StartFlag;//������ʼ��־
	char H_EndFlag;//������ֹ��־
	
	char Line_Flag;
	char Cross_Flag;//ʮ�ֽ����־
	char RightAngle_Flag;//ֱ�Ǳ�־
	char Circle_Flag;//Բ��־
	
	char NoneFlag ;//δʶ���ʲô��־
	
	u8 Flag_Mode;//��ȡ���ĺ�������ʲô����
	
	int Hcnt;
	int Wcnt;
	
	unsigned int H_Temp;
	int My_H_Pos; //����
	int My_H_Pos_Cnt;
	
	unsigned int W_Temp;
	int My_W_Pos;//����
	int My_W_Pos_Cnt;
	
	char W_ALLField;
	char W_Right_HalfField;
	char W_Left_HalfField;
		
	char H_ALLField;
	char H_Up_HalfField;
	char H_Down_HalfField;
}Camera_Data;
enum
{
	None,//��
	Line,//ֱ��
	Cross,//ʮ��
	RightAngle,//ֱ��
	Circle,//Բ
};
float MYtesttime=0;
int main(void)
 {  
	DisableInterrupts;

	delay_init(168);
	TIM5_Init(84,10000);
//	TIM7_Init(84,1000); 
	TIM13_Init(84,0xffff);
	//TIM14_Init(84,0xffff);
// LED_Init();
//	BuzLed_Init();
	//Buzzer_ON;
//	Key_Init();
		UartInit(MYUART);//FreeCars��λ�����ڳ�ʼ��
//	RC_Init();  //ң������ʼ��
	
//		PWM_Init(17000);
//  	PID_Init();
////	Balance_PID_Init();
//	  QEI_Init();
//	 
	
//		
//    I2c_Soft_Init();	
//  	MPU6050_Init(20);
//		abEncoder_Init();
		OLED_Init();
//		Key_Start_Init();
//	I2c_Soft_Init_2();	
//  MPU6050_Init_2(20); 
//  Ultran_Init();	

//	Nrf_Init();
//	TX_Mode();

//Usart2_Init(115200);//������λ����ʼ��
	while(!Ov7620_Init());//�ж�OV7620��ʼ���Ƿ�ɹ� 
//MPU6050DMP_Init();

	/*�����ж����ȼ�*/
//	Buzzer_OFF; 
	MyNVIC_Init(NVIC_PriorityGroup_2,1,1,USART2_IRQn,ENABLE);	 
	EnableInterrupts;
	//PWMͨ����3>4˳ʱ��ת
	//ref.g.x  ������˳��
	//��Ը��ӣz,����Ϊ��������Ϊ��
	while(1)
	{
		int i,j,k=0;
		float Laser_tempx,Laser_tempy=0;
		Laser_tempx = 0;Laser_tempy = 0;LaserDot =0;
		
		if(ImgControl.ImgOk == 1)
		{	
//			//*****************��̬��ֵ*******************//
			threshold=OTSU_threshold((u8 *)Image,CAMERA_H*CAMERA_W);
		
			//*****************ͼ������*******************//
			for(i=0;i<CAMERA_H;i++)
				for(j=0;j<CAMERA_W;j++)
			{
					if(Image[i][j]<threshold)
					{
						Image_Dis[i][j]=1;//OLED�ϣ�1Ϊ����0Ϊ��
						Image_Label[i][j] = 0;
						LaserDot ++;
						Laser_tempx +=j;
						Laser_tempy +=i;
					}
					else 
					{
						Image_Dis[i][j]=0;
						Image_Label[i][j] = 0xff;
					}						
			}		
			//*****************����ɨ��***************************//
			//�ϰ��
			 for(i=0;i<CAMERA_H/2;i++)
				{					
					//�м�¼//����
						for(j=0;j<CAMERA_W;j++) 
							{		
								if(0 == Image_Label[i][j])
								{
										if(0 == Camera_Data.W_StartFlag)
									{
										Camera_Data.W_StartFlag = 1;
										Camera_Data.W_Start = j;
										break	;	
									}
								}
							
							}
							//�м�¼//�Ұ��
							for(j=CAMERA_W-1;j>=0;j--) 
							{		
								if(0 == Image_Label[i][j])
							{
								if(1 == Camera_Data.W_StartFlag)
									{
										Camera_Data.W_EndFlag = 1;
									}
								if(1==Camera_Data.W_EndFlag)
									{
										Camera_Data.W_End = j;
										break	;	
									}		
							}		
			
						} 
						
						if(1 == Camera_Data.W_StartFlag && 1==Camera_Data.W_EndFlag)
						{
							Camera_Data.W_Width[i] =  Camera_Data.W_End - Camera_Data.W_Start;
							Camera_Data.W_Mid[i] = (Camera_Data.W_End + Camera_Data.W_Start)/2;
 							//�����ݴ�ֵ
							Camera_Data.W_StartFlag = 0;
							Camera_Data.W_EndFlag   = 0;
							Camera_Data.W_Start = 0;
							Camera_Data.W_End 	= 0;
						}		
				}
			//�°��
			for(i=CAMERA_H-1;i>=0;i--)
				{					
					//�м�¼//����
						for(j=0;j<CAMERA_W;j++) 
							{		
									if(0 == Image_Label[i][j])
								{
										if(0 == Camera_Data.W_StartFlag)
									{
										Camera_Data.W_StartFlag = 1;
										Camera_Data.W_Start = j;
										break	;	
									}
								}
				
							}
							//�м�¼//�Ұ��
							for(j=CAMERA_W-1;j>=0;j--) 
							{		
								if(0 == Image_Label[i][j])
									{
										if(1 == Camera_Data.W_StartFlag)
											{
												Camera_Data.W_EndFlag = 1;
											}
										if(1==Camera_Data.W_EndFlag)
											{
												Camera_Data.W_End = j;
												break	;	
											}		
									}		
							} 
						
						if(1 == Camera_Data.W_StartFlag && 1==Camera_Data.W_EndFlag)
						{
							Camera_Data.W_Width[i] =  Camera_Data.W_End - Camera_Data.W_Start;
							Camera_Data.W_Mid[i] = (Camera_Data.W_End + Camera_Data.W_Start)/2;
 							//�����ݴ�ֵ
							Camera_Data.W_StartFlag = 0;
							Camera_Data.W_EndFlag   = 0;
							Camera_Data.W_Start = 0;
							Camera_Data.W_End 	= 0;
						}		
				}
				//**************����ɨ��***************************//
				for(i=0;i<CAMERA_W/2;i++)
				{
					for(j=0;j<CAMERA_H;j++)
					{
						if(0 == Image_Label[j][i])
						{
							if(0 == Camera_Data.H_StartFlag)
								{
									Camera_Data.H_StartFlag = 1;
									Camera_Data.H_Start = j;
									break	;	
								}
						}						
					}
					for(j=CAMERA_H-1;j>=0;j--)
					{
						if(0 == Image_Label[j][i])
						{
							if(1 == Camera_Data.H_StartFlag)
								{
									Camera_Data.H_EndFlag = 1;
								}
							if(1==Camera_Data.H_EndFlag)
								{
									Camera_Data.H_End = j;
									break	;	
								}		
						}						
					}
					if(1 == Camera_Data.H_StartFlag && 1==Camera_Data.H_EndFlag)
						{
							Camera_Data.H_Width[i] =  Camera_Data.H_End - Camera_Data.H_Start;
							Camera_Data.H_Mid[i] = Camera_Data.H_End ;
							//�����ݴ�ֵ
							Camera_Data.H_StartFlag = 0;
							Camera_Data.H_EndFlag   = 0;
							Camera_Data.H_Start = 0;
							Camera_Data.H_End 	= 0;
						}	
				}
				for(i=CAMERA_W-1;i>=CAMERA_W/2;i--)
				{
					for(j=0;j<CAMERA_H;j++)
					{
						if(0 == Image_Label[j][i])
						{
							if(0 == Camera_Data.H_StartFlag)
								{
									Camera_Data.H_StartFlag = 1;
									Camera_Data.H_Start = j;
									break	;		
								}
								
						}									
					}
					for(j=CAMERA_H-1;j>=0;j--)
					{
						if(0 == Image_Label[j][i])
						{
								if(1 == Camera_Data.H_StartFlag)
								{
									Camera_Data.H_EndFlag = 1;
								}
								if(1==Camera_Data.H_EndFlag)
								{
									Camera_Data.H_End = j;
									break	;	
								}
									
						}									
					}
					if(1 == Camera_Data.H_StartFlag && 1==Camera_Data.H_EndFlag)
						{
							Camera_Data.H_Width[i] =  Camera_Data.H_End - Camera_Data.H_Start;
							Camera_Data.H_Mid[i] = Camera_Data.H_End ;
							//�����ݴ�ֵ
							Camera_Data.H_StartFlag = 0;
							Camera_Data.H_EndFlag   = 0;
							Camera_Data.H_Start = 0;
							Camera_Data.H_End 	= 0;
						}	
				}
					
				
				//�������ؼ���ֵ
				Camera_Data.H_Temp =0;
				Camera_Data.W_Temp =0;
				Camera_Data.My_H_Pos_Cnt=0;
				Camera_Data.My_W_Pos_Cnt=0;
				
				//�жϺ�������
				for(k=0;k<CAMERA_H;k++)
				{
					if(Camera_Data.H_Width[k] > (CAMERA_H-100))
					{
						Camera_Data.H_Width[k] = 0;
						Camera_Data.H_Mid[k] = -1;//-1������
					}
					if(Camera_Data.W_Width[k] > (CAMERA_H-100))
					{
						Camera_Data.W_Width[k] = 0;
						Camera_Data.W_Mid[k] = -1;
					}
					if(Camera_Data.H_Width[k] > CrossWidth)
					{
								Camera_Data.Hcnt ++;
								//����
								if(k <10)
									   Camera_Data.H_Mid[k] = Camera_Data.H_Mid[CAMERA_H/2];
								else Camera_Data.H_Mid[k] = Camera_Data.H_Mid[k-1];
					}
					if(Camera_Data.W_Width[k] > CrossWidth)
					{
								Camera_Data.Wcnt ++;
								//����
								if(k <10)
										 Camera_Data.W_Mid[k] = Camera_Data.W_Mid[CAMERA_H/2];
								else Camera_Data.W_Mid[k] = Camera_Data.W_Mid[k-1];
						
					}
						if(Camera_Data.H_Mid[k] != -1)
						{
							Camera_Data.My_H_Pos_Cnt ++;
							Camera_Data.H_Temp += Camera_Data.H_Mid[k];
						}
						if(Camera_Data.W_Mid[k] != -1)
						{
							Camera_Data.My_W_Pos_Cnt ++;
							Camera_Data.W_Temp += Camera_Data.W_Mid[k];	
						}
				}
				//�������λ��
						if(Camera_Data.My_H_Pos_Cnt>=50)
						Camera_Data.My_H_Pos = Camera_Data.H_Temp/Camera_Data.My_H_Pos_Cnt;
						else Camera_Data.My_H_Pos = 0;
						if(Camera_Data.My_W_Pos_Cnt>=50)
						Camera_Data.My_W_Pos = Camera_Data.W_Temp/Camera_Data.My_W_Pos_Cnt;
						else Camera_Data.My_W_Pos = 0;
				//ʵ��Բ���ߵ��������ں����ص��С��������
				//�����ж� JudgeKind,�����еȺţ������޵Ⱥ�
				//��������������һ���������������ж�ͼ��,���߿�Ϊ������
				//            *
				//            *
				//            *
				//         -  *  -
				//        |   *	  |				
				//************ ************
				//         -  *  -
				//            *
				//            *
				//            *
				//            *		
       //Camera_Data.My_H_Pos , Camera_Data.My_W_Pos
			 //				Pitch									Roll
							Camera_Data.RightAngle_Flag = 0;
							Camera_Data.Circle_Flag = 0;
							Camera_Data.NoneFlag = 0;
							Camera_Data.Line_Flag = 0;
							Camera_Data.Cross_Flag =  0;
							Camera_Data.Wcnt = 0;
							Camera_Data.Hcnt = 0;
						if(JudgeKind(Camera_Data.My_H_Pos_Cnt,0,150) && JudgeKind(Camera_Data.My_W_Pos_Cnt,0,150) &&JudgeKind(LaserDot,10000,20000))
						{
							Camera_Data.Circle_Flag = 1;//Բ
							

						}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,150,201) && JudgeKind(Camera_Data.My_W_Pos_Cnt,150,201) &&JudgeKind(LaserDot,7000,10000))
						{
							Camera_Data.Cross_Flag = 1;//ʮ��
							
						}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,0,150) && JudgeKind(Camera_Data.My_W_Pos_Cnt,180,201) &&JudgeKind(LaserDot,4500,7000))
					{
							Camera_Data.Line_Flag = 1;//ֱ��
							
					}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,100,180) && JudgeKind(Camera_Data.My_W_Pos_Cnt,100,180) &&JudgeKind(LaserDot,5000,7000))
					{
							Camera_Data.RightAngle_Flag = 1;//ֱ��
							
					}
					else Camera_Data.Line_Flag = 1;
//					if(JudgeKind(Camera_Data.My_H_Pos_Cnt,175,200) && JudgeKind(Camera_Data.My_W_Pos_Cnt,175,200))
//						{
//							Camera_Data.Cross_Flag = 1;//ʮ��
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.Line_Flag = 0;
//							Camera_Data.Circle_Flag = 0;
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//						}
//					else if(JudgeKind(Camera_Data.Hcnt,0,165) && JudgeKind(Camera_Data.Wcnt,165,200))
//					{
//							Camera_Data.Line_Flag = 1;//ֱ��
//							Camera_Data.Circle_Flag = 0;
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//					}
//					else if(JudgeKind(Camera_Data.Hcnt,0,50) && JudgeKind(Camera_Data.Wcnt,0,50))
//					{
//						 Camera_Data.RightAngle_Flag = 1;//ֱ��
//					  	Camera_Data.Circle_Flag = 0;
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.Line_Flag = 0;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//					}
//					else if(JudgeKind(Camera_Data.Hcnt,50,175) && JudgeKind(Camera_Data.Wcnt,50,175))
//					{
//							Camera_Data.Circle_Flag = 1;//Բ
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.Line_Flag = 0;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//					}
//				
//					else 
//						{
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.Circle_Flag = 0;
//							Camera_Data.Line_Flag = 1;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//						}
					if(1 == Camera_Data.Line_Flag)	Camera_Data.Flag_Mode = Line;//ֱ��
					else if(1 == Camera_Data.Cross_Flag) Camera_Data.Flag_Mode = Cross;//ʮ��
					else if(1 == Camera_Data.RightAngle_Flag) Camera_Data.Flag_Mode = RightAngle;//ֱ��
					else if(1 == Camera_Data.Circle_Flag) Camera_Data.Flag_Mode = Circle;//Բ��
					else if(1 == Camera_Data.NoneFlag) Camera_Data.Flag_Mode = None;
						

//			//*****************��ʾ����*******************//
//				//��ʾ�ʹ���Ķ�ֵ����ʱ��������һ������

//				//	OLED_Showfloat(0,0,1,1);

//					
//			//*****************ͨ�Ų���*******************//
//				
				SendToMcu();
				
				OLED_Clear();
				LCD_PrintImage((u8 *)Image_Dis,CAMERA_H,CAMERA_W);
				
				OLED_Showfloat(0,3,Camera_Data.My_H_Pos_Cnt,0);
				OLED_Showfloat(0,4,Camera_Data.My_W_Pos_Cnt,0);
				OLED_Showfloat(0,5,LaserDot,0);
				//OLED_Showfloat(0,6,(Laser_tempx)/LaserDot,2);
				//OLED_Showfloat(0,7,(Laser_tempy)/LaserDot,2);
			//SendCamera();
			//SendOV7620();			//�����������ٴ�����һ֡ͼ��	
		 //	NrfTransmit();
				ImgControl.ImgDealOk = 1;
		}
	}
}
void SendCamera(void)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //????? ?????
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //????? ?????
  	uint32_t i,j;
	  uint8_t d;
		uart_putchar(MYUART,cmdf[0]);
		uart_putchar(MYUART,cmdf[1]);
	for(i = 0; i < CAMERA_H; i++)
	{ 
		for(j = 0; j < CAMERA_W; j++)
		{
			d=Image[i][j];
			if(d == 0xFF) d = 0xFE;         
			uart_putchar(MYUART,d);
		}
	}		
	uart_putchar(MYUART,cmdr[0]);
	uart_putchar(MYUART,cmdr[1]);
}
void SendToMcu(void)
{
	u8 send_1 ,send_2 = 0;
	u8 flagsend_1,flagsend_2=0;
	uart_putchar(MYUART,0xff);
	uart_putchar(MYUART,0xcc);
   send_1 = (u8)(Camera_Data.My_H_Pos>>8);
    if(send_1 == 0xff)
       send_1 = 0xfe;
    uart_putchar(MYUART,send_1);
    send_2=(u8)(Camera_Data.My_H_Pos & 0xff);
     if(send_2 == 0xff)
     send_2 = 0xfe;
    uart_putchar(MYUART,send_2);  
    
    flagsend_1 = (u8)(Camera_Data.My_W_Pos>>8);
    if(flagsend_1 == 0xff)
       flagsend_1 = 0xfe;
    uart_putchar(MYUART,flagsend_1);
		
    flagsend_2=(u8)(Camera_Data.My_W_Pos & 0xff);
     if(flagsend_2 == 0xff)
     flagsend_2 = 0xfe;
		 
  uart_putchar(MYUART,flagsend_2);

	uart_putchar(MYUART,Camera_Data.Flag_Mode);
		 
  uart_putchar(MYUART,send_1+flagsend_2);
}