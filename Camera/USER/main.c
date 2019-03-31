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

#define LineWidth 10//50CM处，线宽标定值
#define CrossWidth 30
#define CircleWidth 20
#define LookField   50 //腐蚀半径
#define JudgeKind(n,min,max) ((n>=min && n<max) ? 1 : 0)
struct 
{
	//行处理时的记录参数，处理完每行清零
	int W_Start;//每行线宽起始坐标
	int W_End;//每行线宽终止坐标
	int W_Width[CAMERA_H];//线宽
	int W_Mid[CAMERA_H];//中间值
	
	char W_StartFlag;//横向起始标志
	char W_EndFlag;//横向终止标志
	
	int H_Start;//每列线宽起始坐标
	int H_End;//每列线宽终止坐标
	int H_Width[CAMERA_W];//线宽
	int H_Mid[CAMERA_W];//中间值
	
	char H_StartFlag;//纵向起始标志
	char H_EndFlag;//纵向终止标志
	
	char Line_Flag;
	char Cross_Flag;//十字交叉标志
	char RightAngle_Flag;//直角标志
	char Circle_Flag;//圆标志
	
	char NoneFlag ;//未识别出什么标志
	
	u8 Flag_Mode;//提取出的黑线属于什么类型
	
	int Hcnt;
	int Wcnt;
	
	unsigned int H_Temp;
	int My_H_Pos; //竖线
	int My_H_Pos_Cnt;
	
	unsigned int W_Temp;
	int My_W_Pos;//横线
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
	None,//无
	Line,//直线
	Cross,//十字
	RightAngle,//直角
	Circle,//圆
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
		UartInit(MYUART);//FreeCars上位机串口初始化
//	RC_Init();  //遥控器初始化
	
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

//Usart2_Init(115200);//匿名上位机初始化
	while(!Ov7620_Init());//判断OV7620初始化是否成功 
//MPU6050DMP_Init();

	/*设置中断优先级*/
//	Buzzer_OFF; 
	MyNVIC_Init(NVIC_PriorityGroup_2,1,1,USART2_IRQn,ENABLE);	 
	EnableInterrupts;
	//PWM通道，3>4顺时针转
	//ref.g.x  逆正，顺负
	//面对杆子,左起为负，右起为正
	while(1)
	{
		int i,j,k=0;
		float Laser_tempx,Laser_tempy=0;
		Laser_tempx = 0;Laser_tempy = 0;LaserDot =0;
		
		if(ImgControl.ImgOk == 1)
		{	
//			//*****************动态阈值*******************//
			threshold=OTSU_threshold((u8 *)Image,CAMERA_H*CAMERA_W);
		
			//*****************图像处理部分*******************//
			for(i=0;i<CAMERA_H;i++)
				for(j=0;j<CAMERA_W;j++)
			{
					if(Image[i][j]<threshold)
					{
						Image_Dis[i][j]=1;//OLED上，1为亮，0为灭
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
			//*****************竖线扫描***************************//
			//上半边
			 for(i=0;i<CAMERA_H/2;i++)
				{					
					//列记录//左半边
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
							//列记录//右半边
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
 							//清零暂存值
							Camera_Data.W_StartFlag = 0;
							Camera_Data.W_EndFlag   = 0;
							Camera_Data.W_Start = 0;
							Camera_Data.W_End 	= 0;
						}		
				}
			//下半边
			for(i=CAMERA_H-1;i>=0;i--)
				{					
					//列记录//左半边
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
							//列记录//右半边
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
 							//清零暂存值
							Camera_Data.W_StartFlag = 0;
							Camera_Data.W_EndFlag   = 0;
							Camera_Data.W_Start = 0;
							Camera_Data.W_End 	= 0;
						}		
				}
				//**************横线扫描***************************//
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
							//清零暂存值
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
							//清零暂存值
							Camera_Data.H_StartFlag = 0;
							Camera_Data.H_EndFlag   = 0;
							Camera_Data.H_Start = 0;
							Camera_Data.H_End 	= 0;
						}	
				}
					
				
				//清零像素计数值
				Camera_Data.H_Temp =0;
				Camera_Data.W_Temp =0;
				Camera_Data.My_H_Pos_Cnt=0;
				Camera_Data.My_W_Pos_Cnt=0;
				
				//判断黑线类型
				for(k=0;k<CAMERA_H;k++)
				{
					if(Camera_Data.H_Width[k] > (CAMERA_H-100))
					{
						Camera_Data.H_Width[k] = 0;
						Camera_Data.H_Mid[k] = -1;//-1代表丢线
					}
					if(Camera_Data.W_Width[k] > (CAMERA_H-100))
					{
						Camera_Data.W_Width[k] = 0;
						Camera_Data.W_Mid[k] = -1;
					}
					if(Camera_Data.H_Width[k] > CrossWidth)
					{
								Camera_Data.Hcnt ++;
								//补线
								if(k <10)
									   Camera_Data.H_Mid[k] = Camera_Data.H_Mid[CAMERA_H/2];
								else Camera_Data.H_Mid[k] = Camera_Data.H_Mid[k-1];
					}
					if(Camera_Data.W_Width[k] > CrossWidth)
					{
								Camera_Data.Wcnt ++;
								//补线
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
				//计算黑线位置
						if(Camera_Data.My_H_Pos_Cnt>=50)
						Camera_Data.My_H_Pos = Camera_Data.H_Temp/Camera_Data.My_H_Pos_Cnt;
						else Camera_Data.My_H_Pos = 0;
						if(Camera_Data.My_W_Pos_Cnt>=50)
						Camera_Data.My_W_Pos = Camera_Data.W_Temp/Camera_Data.My_W_Pos_Cnt;
						else Camera_Data.My_W_Pos = 0;
				//实心圆和线的区分用融合像素点大小进行区分
				//类型判断 JudgeKind,下限有等号，上限无等号
				//用中心向外来搜一个正方形区域来判断图形,虚线框为搜索区
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
							Camera_Data.Circle_Flag = 1;//圆
							

						}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,150,201) && JudgeKind(Camera_Data.My_W_Pos_Cnt,150,201) &&JudgeKind(LaserDot,7000,10000))
						{
							Camera_Data.Cross_Flag = 1;//十字
							
						}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,0,150) && JudgeKind(Camera_Data.My_W_Pos_Cnt,180,201) &&JudgeKind(LaserDot,4500,7000))
					{
							Camera_Data.Line_Flag = 1;//直线
							
					}
						else if(JudgeKind(Camera_Data.My_H_Pos_Cnt,100,180) && JudgeKind(Camera_Data.My_W_Pos_Cnt,100,180) &&JudgeKind(LaserDot,5000,7000))
					{
							Camera_Data.RightAngle_Flag = 1;//直角
							
					}
					else Camera_Data.Line_Flag = 1;
//					if(JudgeKind(Camera_Data.My_H_Pos_Cnt,175,200) && JudgeKind(Camera_Data.My_W_Pos_Cnt,175,200))
//						{
//							Camera_Data.Cross_Flag = 1;//十字
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.Line_Flag = 0;
//							Camera_Data.Circle_Flag = 0;
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//						}
//					else if(JudgeKind(Camera_Data.Hcnt,0,165) && JudgeKind(Camera_Data.Wcnt,165,200))
//					{
//							Camera_Data.Line_Flag = 1;//直线
//							Camera_Data.Circle_Flag = 0;
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.RightAngle_Flag = 0;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//					}
//					else if(JudgeKind(Camera_Data.Hcnt,0,50) && JudgeKind(Camera_Data.Wcnt,0,50))
//					{
//						 Camera_Data.RightAngle_Flag = 1;//直角
//					  	Camera_Data.Circle_Flag = 0;
//							Camera_Data.NoneFlag = 0;
//							Camera_Data.Line_Flag = 0;
//							Camera_Data.Cross_Flag =  0;
//							Camera_Data.Wcnt = 0;
//							Camera_Data.Hcnt = 0;
//					}
//					else if(JudgeKind(Camera_Data.Hcnt,50,175) && JudgeKind(Camera_Data.Wcnt,50,175))
//					{
//							Camera_Data.Circle_Flag = 1;//圆
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
					if(1 == Camera_Data.Line_Flag)	Camera_Data.Flag_Mode = Line;//直线
					else if(1 == Camera_Data.Cross_Flag) Camera_Data.Flag_Mode = Cross;//十字
					else if(1 == Camera_Data.RightAngle_Flag) Camera_Data.Flag_Mode = RightAngle;//直角
					else if(1 == Camera_Data.Circle_Flag) Camera_Data.Flag_Mode = Circle;//圆盘
					else if(1 == Camera_Data.NoneFlag) Camera_Data.Flag_Mode = None;
						

//			//*****************显示部分*******************//
//				//显示和处理的二值化分时，但共用一个数组

//				//	OLED_Showfloat(0,0,1,1);

//					
//			//*****************通信部分*******************//
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
			//SendOV7620();			//发给主机后再处理下一帧图像	
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