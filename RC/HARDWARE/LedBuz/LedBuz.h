#ifndef _LEDBUZ_H_
#define _LEDBUZ_H_

#include "common.h"
#include "rtc.h"

#define BuzPeri 	RCC_AHB1Periph_GPIOB
#define BuzPort 	GPIOB

#define LedPeri 	RCC_AHB1Periph_GPIOB
#define LedPort 	GPIOB

#define LedGreenPin	GPIO_Pin_1
#define LedRedPin   GPIO_Pin_0

#define BuzPin      GPIO_Pin_5

#define GREEN_ON      LedPort->BSRRL =  LedGreenPin
#define GREEN_OFF     LedPort->BSRRH =  LedGreenPin
#define GREEN_TURN    LedPort->ODR  ^=  LedGreenPin

#define RED_ON        LedPort->BSRRL =  LedRedPin
#define RED_OFF       LedPort->BSRRH =  LedRedPin
#define RED_TURN      LedPort->ODR  ^=  LedRedPin

#define Buzzer_ON     BuzPort->BSRRL =  BuzPin
#define Buzzer_OFF    BuzPort->BSRRH =  BuzPin
#define Buzzer_TURN   BuzPort->ODR  ^=  BuzPin


typedef enum
{
    BuzKey,
    BuzDataRecOk,
    BuzGetOffsetTmOver,
    BuzPowerLow,
    BuzCarDanger,    
}BuzzerMode;


typedef struct
{
    bool Alarm;
    u32  AlarmTime;
}Lightmode;


typedef struct
{
   u32 Time2Normal;
}BuzerTimeCtr;

typedef struct 
{
  u32 StrTm; 
  u32 StrNumTm;
  u32 CameraTm; 
  u32 DataTm;
  u32 DmaTm;
  //ºìµÆ
  u32 LedRedTm;
  u32 RedFreL;
  u32 RedFreH;
  //ÂÌµÆ
  u32 LedGreTm;
  u32 LedGreL;
  u32 LedGreH;
  //·äÃùÆ÷
  u32 BuzFreL;
  u32 BuzFreH;
  //Òº¾§-
  u32 LCDTm;
  u32 SysTime;
  
  u32 LCDFreL;
  u32 LCDFreH;

  u32 TestTm;
  
  u32 CheckStLine;

  u32 DealKeyTm;
	
	u32 DmpNorTm;
	u32 ReSetTime;
  
  // CheckBlock;
}TimeSet;

extern BuzerTimeCtr BuzTmCtr;
extern BuzzerMode   BuzMode;
extern TimeSet      SetTime;
extern Lightmode    BlueMode;
extern Lightmode    RedMode;

extern  void    LedFun_Select(void);
extern  void    BuzLed_Init(void);
//extern  void    Power_Check();   
extern  void    Red_Alarm(void);

extern  void    water_lights(void);                                               // Á÷Ë®µÆ
extern  void    Buzzer_Alarm(void);
extern  void    Buzzer_PowerLow(void);
extern  void    Buzzer_Select(void);
extern  void    Buzzer_Key(void);
extern  void    Buzzer_CarSafeDet(void);
extern  void    SysNormalTest(void);
extern  void    Buzzer_RecOk(void);
extern  void    ReadSysPara(void);
extern  void    Buzzer_GetOffSetTmOver(void);
#endif

