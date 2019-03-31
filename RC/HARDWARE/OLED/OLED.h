#ifndef _OLED_H_
#define _OLED_H_

#include "common.h"

#define OLEDPeri 	RCC_AHB1Periph_GPIOE
#define OLEDPort 	GPIOE

#define SCL_PIN  	  pin7
#define SDA_PIN     pin8
#define RST_PIN     pin9
#define DC_PIN      pin10
#define CS_PIN      pin11

#define OLED_SCLH  OLEDPort->BSRRL =  SCL_PIN // ???? 
#define OLED_SCLL  OLEDPort->BSRRH =  SCL_PIN

#define OLED_SDAH  OLEDPort->BSRRL =  SDA_PIN  //???D0
#define OLED_SDAL  OLEDPort->BSRRH =  SDA_PIN

#define OLED_RSTH  OLEDPort->BSRRL =  RST_PIN  //??????
#define OLED_RSTL  OLEDPort->BSRRH =  RST_PIN

#define OLED_DCH   OLEDPort->BSRRL =  DC_PIN
#define OLED_DCL   OLEDPort->BSRRH =  DC_PIN//????

#define OLED_CSH   OLEDPort->BSRRL =  CS_PIN
#define OLED_CSL   OLEDPort->BSRRH =  CS_PIN//????

extern u8 mushroom[];
extern u8 sky1[];
extern u8 sky2[];
extern u8 sky3[];

void OLED_Init(void);
void OLED_CLS(void);
void OLED_Set_Pos(u8 x, u8 y);//??????
void OLED_WrDat(u8 data);   //?????
void OLED_Point(u8 x,u8 y);
void OLED_P6x8Char(u8 x,u8 y,u8 ch);
void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
extern void OLED_P14x16Str(u8 x,u8 y,u8 ch[]);
void OLED_PXx16MixStr(u8 x, u8 y, u8 ch[]);
void OLED_PrintBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]); 
void OLED_Full(u8 dat);
void OLED_ShowInt(u16 x,u16 y,s32 num);
void OLED_Showfloat(u16 a,u16 b,double number,u8 decimals_number);
void OLED_Cursor(u8 x,u8 y);
void OLED_Fill(u8 bmp_data);
void OLED_ClearCursor(u8 x,u8 y);
void OLED_Clear(void);
void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);

#endif
