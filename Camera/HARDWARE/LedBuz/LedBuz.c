
#include "LedBuz.h"
#include "delay.h"
#include "timer.h"

BuzerTimeCtr BuzTmCtr;
BuzzerMode   BuzMode;
TimeSet      SetTime;
Lightmode    BlueMode;
Lightmode    RedMode;

void BuzLed_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(BuzPeri, ENABLE);

  GPIO_InitStructure.GPIO_Pin = BuzPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //配置IO方向
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //输出方式（推挽或者开漏）
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	   //设置上下拉
  GPIO_Init(BuzPort, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(LedPeri, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LedGreenPin|LedRedPin;
	GPIO_Init(LedPort, &GPIO_InitStructure);

	GREEN_OFF;
	RED_OFF;
  Buzzer_OFF;	
}

//系统正常检测
void SysNormalTest()
{
    Buzzer_Select();  //蜂鸣器正常显示 
    LedFun_Select();
    //Power_Check();
}
/*******************电源检测**********************************/
//void Power_Check()
//{
//    PowerValue = (u16)(ad_once(ADC0,ADC_POWER,ADC_12bit)*330/(4095*0.29));
//    
//    if(PowerValue < 720 && PowerLowFlag == false && FirstEnterSpeedTm == 0)  //低电压报警
//    {
//        Buzzer_PowerLow();
//        RedMode.Alarm = true;
//        RedMode.AlarmTime = setdelay_ms(50000);
//        PowerLowFlag = true;
//        BuzPowLow.Enabled = true; 
//    }
//    else
//    {
//        BuzPowLow.Enabled = false;
//        RedMode.Alarm = false;
//    }
//}
/*******************Led功能***********************************/
void LedFun_Select()
{
    if(checkdelay_ms(SetTime.LedGreTm))
    {
        SetTime.LedGreTm = setdelay_ms(1500);
        GREEN_TURN;
    }
    if(RedMode.Alarm == true)
    {
        Red_Alarm();
        if(checkdelay_ms(RedMode.AlarmTime))
        {
            RedMode.Alarm  = false;
        }
    }
}
/*******************多频率蜂鸣器选择**************************/
 void Buzzer_Select()
{
  if(checkdelay_ms(BuzTmCtr.Time2Normal)) //正常
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(1450);
      SetTime.BuzFreL = setdelay_ms(2000);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(10);
      SetTime.BuzFreH = setdelay_ms(2000);
      Buzzer_ON;
    }
  }
  else if(BuzMode == BuzDataRecOk)
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(50);
      SetTime.BuzFreL = setdelay_ms(100);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(50);
      SetTime.BuzFreH = setdelay_ms(100);
      Buzzer_ON;
    } 
  }  
  else if(BuzMode == BuzGetOffsetTmOver)//陀螺仪偏置值已获
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(25);
      SetTime.BuzFreL = setdelay_ms(50);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(25);
      SetTime.BuzFreH = setdelay_ms(50);
      Buzzer_ON;
    } 
  }
  else if(BuzMode == BuzPowerLow)
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(50);
      SetTime.BuzFreL = setdelay_ms(100);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(50);
      SetTime.BuzFreH = setdelay_ms(100);
      Buzzer_ON;
    } 
  }
  else if(BuzMode == BuzCarDanger)
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(100);
      SetTime.BuzFreL = setdelay_ms(200);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(100);
      SetTime.BuzFreH = setdelay_ms(200);
      Buzzer_ON;
    } 
  }
  else if(BuzMode == BuzKey)
  {
    if(checkdelay_ms(SetTime.BuzFreL))
    {
      SetTime.BuzFreH = setdelay_ms(50);
      SetTime.BuzFreL = setdelay_ms(100);
      Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
      SetTime.BuzFreL = setdelay_ms(50);
      SetTime.BuzFreH = setdelay_ms(100);
      Buzzer_ON;
    } 
  }
}

void Buzzer_RecOk()
{
   BuzTmCtr.Time2Normal = setdelay_ms(500);
   SetTime.BuzFreH = setdelay_ms(0);
   SetTime.BuzFreL = setdelay_ms(0);
   BuzMode = BuzDataRecOk; 
}

void Buzzer_GetOffSetTmOver()
{
   BuzTmCtr.Time2Normal = setdelay_ms(500);
   SetTime.BuzFreH = setdelay_ms(0);
   SetTime.BuzFreL = setdelay_ms(0);
   BuzMode = BuzGetOffsetTmOver;
}

void Buzzer_Key()
{
   BuzTmCtr.Time2Normal = setdelay_ms(100);
   SetTime.BuzFreH = setdelay_ms(0);
   SetTime.BuzFreL = setdelay_ms(0);
   BuzMode = BuzKey;
}

void Buzzer_CarSafeDet()
{
   BuzTmCtr.Time2Normal = setdelay_ms(2000);
   SetTime.BuzFreH = setdelay_ms(0);
   SetTime.BuzFreL = setdelay_ms(0);
   BuzMode = BuzCarDanger;
}

void Buzzer_PowerLow()
{
   BuzTmCtr.Time2Normal = setdelay_ms(500);
   SetTime.BuzFreH = setdelay_ms(0);
   SetTime.BuzFreL = setdelay_ms(0);
   BuzMode = BuzPowerLow;
}
/**************************LED、Buzzer报警***************************/
void Red_Alarm()
{
    if(checkdelay_ms(SetTime.RedFreL))
    {
        SetTime.RedFreH = setdelay_ms(50);
        SetTime.RedFreL = setdelay_ms(100);
        RED_OFF;
    }
    if(checkdelay_ms(SetTime.RedFreH))
    {
        SetTime.RedFreL = setdelay_ms(50);
        SetTime.RedFreH = setdelay_ms(100);
        RED_ON;
    }    
}

//系统蜂鸣器处理
void  Buzzer_Alarm()
{
    if(checkdelay_ms(SetTime.BuzFreL))
    {
        SetTime.BuzFreH = setdelay_ms(20);
        SetTime.BuzFreL = setdelay_ms(100);
        Buzzer_OFF;
    }
    if(checkdelay_ms(SetTime.BuzFreH))
    {
        SetTime.BuzFreL = setdelay_ms(80);
        SetTime.BuzFreH = setdelay_ms(100);
        Buzzer_ON;
    }   
}

