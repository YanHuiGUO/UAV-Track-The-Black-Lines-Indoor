#include "isr.h"
#include "UartLink.h"
//#include "LedBuz.h"
//#include "key_matrix.h"
//#include "LCDEvent.h"
//#include "rtc.h"
#include "QEI.h"
//#include "Track.h"
#include "OLED.H"
#include "mpu6050.h"
//#include "DCMI.h"

Rx_Port  Rx_PORT;
Union    uni;
double UartData[12] = {0};
unsigned char Start_mark = 0;
float  UART_WL[10] = {0};
int WL_d[4] = {0};
#define UartRxBufferLen 100
#define UartRxDataLen   41
#define UartRxCmdLen    7
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据
typedef struct 
{
  int Stack;
  Byte Data;
  Byte PreData;
  Byte Buffer[UartRxBufferLen];
  Byte Enable;
  Byte Check;
}SerialPortType;
SerialPortType SerialPortRx;
#define 	CONTROL_PERIOD 	5 
u8   Cir_time;
u32  SysRunSecond;
bool Key_Start;
u32  Now_Time = 0;
u32  SysRunSecond;
void TIM7_IRQHandler(void)
{
	if (TIM7->SR&TIM_IT_Update && TIM7->DIER&TIM_IT_Update)
    {
	    SysRunSecond++;
	    if(Key_Start == true && Now_Time == 0)
	    {
//	        Buzzer_GetOffSetTmOver();   //获取偏置时间结束，蜂鸣器提醒
	        Now_Time = SysRunSecond;
	    }
	    {          
	        if( Cir_time >= CONTROL_PERIOD)
	        {
	            Cir_time = 0;
	        }
	        else if (Cir_time == 1)
	        { 
						 
						//	Left_SpeedControl();
						//	Right_SpeedControl();
						Left_Angle_Control();
							
							
	        }
	        else if (Cir_time == 2)
	        {
						//SpeedControl();
							MotorOutput(LeftAngleCtr.AngleCtrOut,RightAngleCtr.AngleCtrOut);
						  
	        }
	        else if (Cir_time == 3)
	        {
						UartDebug();
	        }
	        else if (Cir_time == 4)
	        {    
	        } 
			Cir_time++;       
	    }
		TIM7->SR = (UInt16)~TIM_IT_Update;
	}
}

//static double  RecPara(u8 channel)
//{
//    assert_param(channel<12);

//    return ((s16)( Rx_PORT.Buffer[Rx_PORT.Stack - 40 + channel * 3] << 8 | 
//                     Rx_PORT.Buffer[Rx_PORT.Stack - 40 + channel * 3 + 1])
//    +(((s8)(Rx_PORT.Buffer[Rx_PORT.Stack - 40 + channel * 3 + 2]) / (float)100)));
//}


void Page0_debug()
{ 
   
		
		//RightSpeedCtr.Speed
}
/*
    Tracking_PID.tracking_Kp = 5;
		Tracking_PID.tracking_Kd = 3;
		Tracking_PID.tracking_Ki = 0;
		Tracking_PID.speed_setting = 3500;
		LeftSpeedCtr.Kp = 4;
    LeftSpeedCtr.Ki = 0;
    LeftSpeedCtr.Kd = 0;
		RightSpeedCtr.Kp = 4;
    RightSpeedCtr.Ki = 0;
    RightSpeedCtr.Kd = 0;
*/
void UartDebug(void)
{
	  LeftAngleCtr.Kp  = UartData[0];
		LeftAngleCtr.Ki  = UartData[1]; 
		LeftAngleCtr.Kd  = UartData[2]; 
		//RightAngleCtr.Kp = UartData[2];
		RightAngleCtr.Ki = UartData[3]; 
		LeftAngleCtr.AngleSet  = UartData[4] ;  
    RightAngleCtr.AngleSet = UartData[5] ;		 
//	  LeftSpeedCtr.Kp  = UartData[0];
//		LeftSpeedCtr.Ki  = UartData[1]; 
//		RightSpeedCtr.Kp = UartData[2];
//		RightSpeedCtr.Ki = UartData[3]; 
//		LeftSpeedCtr.SpeedSet  = UartData[4]/100;  
//    RightSpeedCtr.SpeedSet = UartData[5]/100;		 
//   DCMI_temp = UartData[0];
////	 Tracking_PID.tracking_Kp =   UartData[1];
//   DCMI_grayscale = UartData[1];
//	 Tracking_PID.tracking_Ki =   UartData[2];
//	 Tracking_PID.tracking_Kd =   UartData[3];
//	 LeftSpeedCtr.Kp =            UartData[4]; 
//	 LeftSpeedCtr.Ki =            UartData[5];   
//	 RightSpeedCtr.Kp =           UartData[6];
//	 RightSpeedCtr.Ki =           UartData[7];
//   Start_mark ++;              
}
 

 
void UartCmd(Byte CmdNum,Byte Data)
{
     
    switch(CmdNum)//cmd number
  {
       case 2:  
       {
             switch(Data)
            {
               case 102:  ; break;
               case 103:   ; break;
               default:break; 
             }
        } break;
       case 1:  
       {
            switch(Data)
            {
                  case 6:   ; break;     //F6
                  case 7:   ; break;           //F7
          //      case 8:  huotui=1;break;        
               default:break;
             }
                
        }break;
       default:break;

  }	
}
void USART2_IRQHandler(void)  //????
{
  int i,b,d1 ;
  u32 d;
  
    if(USART_GetITStatus(USART2 , USART_IT_RXNE) != RESET)
    {
        SerialPortRx.Data = USART_ReceiveData(USART2);
    if( SerialPortRx.Stack < UartRxBufferLen )
    {
      SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
      
      if(   SerialPortRx.Stack >= UartRxDataLen  //UartRxDataLen?????
         && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  ==0xff //????
           && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1]==0x55
             && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2]==0xaa
               && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3]==0x10 )
      {                                                           //double data 9???????
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxDataLen;  
        for(i=b; i<SerialPortRx.Stack-1; i++)  //???????????
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//??
        }
        
        if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
        {   //????,??????
           
          for(i = 0; i<9; i++)
          {
						WL_d[i] = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
            d = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
            if(d>0x7FFFFFFF)
            {
              d1 = 0x7FFFFFFF - d;
            }
            else
            {
              d1 = d;
            }
            UartData[i]=d1;
            UartData[i]/=65536.0;
          }
          
          UartDebug();  //????,??????????
//					Buzzer_RecOk();
        }
        SerialPortRx.Stack = 0;
      }
      else if(   SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen?????
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  ==0xff
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+1]==0x55
                  && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+2]==0xaa
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+3]==0x77 )//cmd
      {
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxCmdLen; //???
        for(i=b; i<SerialPortRx.Stack-1; i++)  //???????????
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//??
        }
        if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
        {   //????
          UartCmd(UartCmdNum,UartCmdData);//????????,??MCU????
        }
        SerialPortRx.Stack = 0;
      }
    }
    else
    {
      SerialPortRx.Stack = 0;
    } 
  }
}
