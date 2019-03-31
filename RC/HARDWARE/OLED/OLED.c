#include "common.h"
#include "OLED_FONT.h"
#include "OLED.h"

/*************************************************************************************
函数名称：void OLED_WrDat(u8 data)
功    能：OLED写数据
参    数：None
返回值  ：None
**************************************************************************************/

void OLED_WrDat(u8 data)   
{
	u8 i = 8;
	OLED_DCH;
	OLED_SCLL; 
	while (i--)
	{
		if (data & 0x80)
		{
			OLED_SDAH;
		}
		else
		{
			OLED_SDAL;
		}
		OLED_SCLH;;;
		//asm("nop");    
		OLED_SCLL;  
		data <<= 1;    
	}
}

/*************************************************************************************
函数名称：void OLED_WrCmd(u8 cmd)
功    能：OLED写命令
参    数：None
返回值  ：None
**************************************************************************************/

void OLED_WrCmd(u8 cmd) 
{
	u8 i = 8;
	OLED_DCL;
	OLED_SCLL;
	while (i--)
	{
		if (cmd & 0x80)
		{
			OLED_SDAH;
		}
		else
		{
			OLED_SDAL;
		}
		OLED_SCLH;;;
		//asm("nop");           
		OLED_SCLL;    
		cmd <<= 1;   
	} 	
}

/*************************************************************************************
函数名称：void OLED_Set_Pos(u8 x, u8 y)
功    能：OLED设置画点位置
参    数：None
返回值  ：None
**************************************************************************************/
	
void OLED_Set_Pos(u8 x, u8 y)
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01); 
} 

/*************************************************************************************
函数名称：void OLED_CLS(void) 
功    能：OLED清屏
参    数：None
返回值  ：None
**************************************************************************************/

void OLED_CLS(void) 
{
	u8 y,x;	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10); 
		for(x=0;x<128;x++)
			OLED_WrDat(0);
	}
}

void OLED_DLY_ms(u8 ms)
{                         
    u16 a;
    while(ms)
    {
        a=6675;
        while(a--);
        ms--;
    }
    return;
}

void SetStartColumn(unsigned char d)
{
	OLED_WrCmd(0x00+d%16);	// Set Lower Column Start Address for Page Addressing Mode
	OLED_WrCmd(0x10+d/16);	// Set Higher Column Start Address for Page Addressing Mode
}

void SetAddressingMode(unsigned char d)
{
	OLED_WrCmd(0x20);			// Set Memory Addressing Mode
	OLED_WrCmd(d);			// Default => 0x02
    // 0x00 => Horizontal Addressing Mode
    // 0x01 => Vertical Addressing Mode
    // 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
	OLED_WrCmd(0x21);		// Set Column Address
	OLED_WrCmd(a);			// Default => 0x00 (Column Start Address)
	OLED_WrCmd(b);			// Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char a, unsigned char b)
{
	OLED_WrCmd(0x22);	    // Set Page Address
	OLED_WrCmd(a);			// Default => 0x00 (Page Start Address)
	OLED_WrCmd(b);			// Default => 0x07 (Page End Address)
}

void SetStartLine(unsigned char d)
{
	OLED_WrCmd(0x40|d);		// Set Display Start Line
    // Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char d)
{
	OLED_WrCmd(0x81);			// Set Contrast Control
	OLED_WrCmd(d);			    // Default => 0x7F
}

void Set_Charge_Pump(unsigned char d)
{
	OLED_WrCmd(0x8D);			// Set Charge Pump
	OLED_WrCmd(0x10|d);		// Default => 0x10
    // 0x10 (0x00) => Disable Charge Pump
    // 0x14 (0x04) => Enable Charge Pump
}

void Set_Segment_Remap(unsigned char d)
{
	OLED_WrCmd(0xA0|d);		// Set Segment Re-Map
    // Default => 0xA0
    // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
    // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void Set_Entire_Display(unsigned char d)
{
	OLED_WrCmd(0xA4|d);		// Set Entire Display On / Off
    // Default => 0xA4
    // 0xA4 (0x00) => Normal Display
    // 0xA5 (0x01) => Entire Display On
}

void Set_Inverse_Display(unsigned char d)
{
	OLED_WrCmd(0xA6|d);		// Set Inverse Display On/Off
    // Default => 0xA6
    // 0xA6 (0x00) => Normal Display
    // 0xA7 (0x01) => Inverse Display On
}

void Set_Multiplex_Ratio(unsigned char d)
{
	OLED_WrCmd(0xA8);			// Set Multiplex Ratio
	OLED_WrCmd(d);			// Default => 0x3F (1/64 Duty)
}

void Set_Display_On_Off(unsigned char d)
{
	OLED_WrCmd(0xAE|d);		// Set Display On/Off
    // Default => 0xAE
    // 0xAE (0x00) => Display Off
    // 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char d)
{
	OLED_WrCmd(0xB0|d);		// Set Page Start Address for Page Addressing Mode
    // Default => 0xB0 (0x00)
}

void Set_Common_Remap(unsigned char d)
{
	OLED_WrCmd(0xC0|d);		// Set COM Output Scan Direction
    // Default => 0xC0
    // 0xC0 (0x00) => Scan from COM0 to 63
    // 0xC8 (0x08) => Scan from COM63 to 0
}

void Set_Display_Offset(unsigned char d)
{
	OLED_WrCmd(0xD3);			// Set Display Offset
	OLED_WrCmd(d);			// Default => 0x00
}

void Set_Display_Clock(unsigned char d)
{
	OLED_WrCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	OLED_WrCmd(d);			// Default => 0x80
    // D[3:0] => Display Clock Divider
    // D[7:4] => Oscillator Frequency
}

void Set_Precharge_Period(unsigned char d)
{
	OLED_WrCmd(0xD9);			// Set Pre-Charge Period
	OLED_WrCmd(d);			// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
    // D[3:0] => Phase 1 Period in 1~15 Display Clocks
    // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void Set_Common_Config(unsigned char d)
{
	OLED_WrCmd(0xDA);			// Set COM Pins Hardware Configuration
	OLED_WrCmd(0x02|d);		// Default => 0x12 (0x10)
    // Alternative COM Pin Configuration
    // Disable COM Left/Right Re-Map
}

void Set_VCOMH(unsigned char d)
{
	OLED_WrCmd(0xDB);			// Set VCOMH Deselect Level
	OLED_WrCmd(d);			// Default => 0x20 (0.77*VCC)
}

void Set_NOP(void)
{
	OLED_WrCmd(0xE3);			// Command for No Operation
}

/*************************************************************************************
函数名称：void OLED_Init(void)
功    能：OLED初始化
参    数：None
返回值  ：None
**************************************************************************************/

void OLED_Init(void)
{
	GPIO_InitTypeDef OLED_inittype;
	
	OLED_inittype.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 ;
	OLED_inittype.GPIO_Mode = GPIO_Mode_OUT;
	OLED_inittype.GPIO_Speed = GPIO_Speed_100MHz;
	OLED_inittype.GPIO_OType = GPIO_OType_PP;
	OLED_inittype.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_Init(GPIOE, &OLED_inittype);
		
	OLED_SCLH;	
	OLED_SCLL;
	OLED_RSTL;
  OLED_DLY_ms(50);
	OLED_RSTH;
    
	/*Set_Display_On_Off(0x00);     // Display Off (0x00/0x01)
	Set_Display_Clock(0x80);      // Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);	  // 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);	  // Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			  // Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x04);		  // Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	  // Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0x01);	  // Set SEG/Column Mapping     0x00???? 0x01??
	Set_Common_Remap(0x08);	      // Set COM/Row Scan Direction 0x00???? 0x08??
	Set_Common_Config(0x10);	  // Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xCF);     // Set SEG Output Current
	Set_Precharge_Period(0xF1);	  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x40);			  // Set VCOM Deselect Level
	Set_Entire_Display(0x00);	  // Disable Entire Display On (0x00/0x01)
	Set_Inverse_Display(0x00);	  // Disable Inverse Display On (0x00/0x01)  
	Set_Display_On_Off(0x01);	  // Display On (0x00/0x01)
	OLED_Full(0x00);               //????
	OLED_Set_Pos(0,0); */
	
	OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02);//
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  OLED_WrCmd(0xaf);//--turn on oled panel
  OLED_Fill(0x00);  //????
  OLED_Set_Pos(0,0);

}

/*************************************************************************************
函数名称：void OLED_Point(u8 x,u8 y)
功    能：OLED画点
参    数：x：点的横坐标,y：点的纵坐标
返回值  ：None
**************************************************************************************/

void OLED_Point(u8 x,u8 y)
{
	u8 pos,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=y/8;
	temp=1<<(y%8);
	OLED_Set_Pos(x,pos);
	OLED_WrDat(temp);
}

/*************************************************************************************
函数名称：void OLED_P6x8Char(u8 x,u8 y,u8 ch)
功    能：OLED显示6x8的字符
参    数：x：字符的横坐标,y：字符的纵坐标，ch：要显示的字符
返回值  ：None
**************************************************************************************/

void OLED_P6x8Char(u8 x,u8 y,u8 ch)
{
	u8 c=0,i=0;      
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	OLED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
	{     
		OLED_WrDat(F6x8[c][i]);  
	}
}

/*************************************************************************************
函数名称：void OLED_P6x8Str(u8 x,u8 y,u8 ch[])
功    能：OLED显示6x8的字符串
参    数：x：字符的横坐标,y：字符的纵坐标，ch：要显示的字符串
返回值  ：None
**************************************************************************************/

void OLED_P6x8Str(u8 x,u8 y,u8 ch[])
{
	u8 c=0,i=0,j=0;      
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);    
		for(i=0;i<6;i++)
		{     
			OLED_WrDat(F6x8[c][i]);  
		}
		x+=6;
		j++;
	}
}

/*************************************************************************************
函数名称：void OLED_P8x16Str(u8 x,u8 y,u8 ch[])
功    能：OLED显示8x16的字符串
参    数：x：字符的横坐标,y：字符的纵坐标，ch：要显示的字符串
返回值  ：None
**************************************************************************************/

void OLED_P8x16Str(u8 x,u8 y,u8 ch[])
{
	u8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);    
		for(i=0;i<8;i++) 
		{
			OLED_WrDat(F8X16[(c<<4)+i]);
		}
		OLED_Set_Pos(x,y+1);    
		for(i=0;i<8;i++) 
		{
			OLED_WrDat(F8X16[(c<<4)+i+8]);
		}
		x+=8;
		j++;
	}
}

/*************************************************************************************
函数名称：void OLED_ShowInt(u16 x,u16 y,s32 num)
功    能：OLED显示整型数
参    数：x：数值的横坐标,y：数值的纵坐标，num：要显示的整型数
返回值  ：None
**************************************************************************************/

void OLED_ShowInt(u16 x,u16 y,s32 num)
{   
  u8 X=0;
  int Number = num;
  u8 s[20] = " ";
  u8 J = 0;
  if(Number==0)
  {
    OLED_P6x8Char(x,y,'0') ; 
  }
  else if(Number<0)
  {
    Number = - Number;
    num = - num;
    while((u32)Number%10!=0||(u32)Number!=0)
    {
        X++;
        Number=Number/10;
    }
    s[0]= '-';
    
    while(num%10!=0||num!=0)
    {
        s[X--]=num%10+48;
        num=num/10;
    }
    s[J+1]='\0';
    OLED_P6x8Str(x,y,s);
  }
  else
  {
    while((u32)Number%10!=0||(u32)Number!=0)
    {
        X++;
        Number=Number/10;
    }
    s[5]='0';
    X-=1;
    J = X;
    while(num%10!=0||num!=0)
    {
        s[X--]=num%10+48;
        num=num/10;
    }
    s[J+1]='\0';
    OLED_P6x8Str(x,y,s);
  }
}

/*************************************************************************************
函数名称：void OLED_Showfloat(u16 a,u16 b,double number,u8 decimals_number)
功    能：OLED显示浮点型
参    数：x：数值的横坐标,y：数值的纵坐标，num：要显示的浮点型数，decimals_number：
返回值  ：None
**************************************************************************************/

void OLED_Showfloat(u16 a,u16 b,double number,u8 decimals_number)
{ 
  u8 s[]=" ";
  int decimals=1,i,j;
  unsigned int x=0;
  float Number=number;

  if(number==0)
  {
    s[0]='0';
    OLED_P6x8Str(a,b,s);
    return;
  }
  
  if(number<0)
  { 
      number = - number;
      Number = - Number;
      for(i=0;i<decimals_number;i++)
      {
         decimals*=10;
      }
      number=number*decimals;
      Number=number;
      while((int)Number%10!=0||(int)Number!=0)
      {
         x++;
         Number=Number/10;
      }
      x+=2;
      j=x;
      while((int)number%10!=0||(int)number!=0)
      {
        if((x-1)==(j-decimals_number-1))
        {
            x--;
        }
        s[--x]=(int)number%10+48;
        number=number/10;
      }
      s[0] = '-';
      if(decimals_number==0)
         s[j-decimals_number-1]='\0';
      else
         s[j-decimals_number-1]='.';
  }
  else
  {
      for(i=0;i<decimals_number;i++)
      {
         decimals*=10;
      }
      number=number*decimals;
      Number=number;
      while((int)Number%10!=0||(int)Number!=0)
      {
         x++;
         Number=Number/10;
      }
      x+=1;
      j=x;
      while((int)number%10!=0||(int)number!=0)
      {
        if((x-1)==(j-decimals_number-1))
        {
            x--;
        }
        s[--x]=(int)number%10+48;
        number=number/10;
      }
      if(decimals_number==0)
         s[j-decimals_number-1]='\0';
      else
         s[j-decimals_number-1]='.';
  }
  OLED_P6x8Str(a,b,s);
}

/*************************************************************************************
函数名称：void OLED_Cursor(u8 x,u8 y)
功    能：OLED显示光标位置
参    数：x：光标的横坐标,y：光标的纵坐标
返回值  ：None
**************************************************************************************/

void OLED_Cursor(u8 x,u8 y)
{
	u8 i = 0;
    OLED_Set_Pos(x,y);        
    for(; i < 3; i++)
    {
        OLED_WrDat(0xFF);
        //x += 1;
    }
}

/*************************************************************************************
函数名称：void OLED_ClearCursor(u8 x,u8 y)
功    能：OLED清楚光标
参    数：x：光标的横坐标,y：光标的纵坐标
返回值  ：None
**************************************************************************************/

void OLED_ClearCursor(u8 x,u8 y)
{
	u8 i = 0;
    OLED_Set_Pos(x,y);
    for(;i < 3; i++)
    {
        OLED_WrDat(0x00);
    }
}

/*************************************************************************************
函数名称：void  OLED_Fill(u8 bmp_data)
功    能：OLED填充
参    数：bmp_data：0：灭，1：亮
返回值  ：None
**************************************************************************************/

void  OLED_Fill(u8 bmp_data)
{
	u8 y,x;

	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<128;x++)
			OLED_WrDat(bmp_data);
	}
}

/*************************************************************************************
函数名称：void OLED_Full(u8 bmp_data)
功    能：OLED满屏
参    数：bmp_data：0：灭，1：亮
返回值  ：None
**************************************************************************************/

void OLED_Full(u8 bmp_data)
{
	u8 y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);     
		OLED_WrCmd(0x00);       
		OLED_WrCmd(0x10);	    
		for(x=0;x<128;x++)
			OLED_WrDat(bmp_data);
	}
} 

/*************************************************************************************
函数名称：void OLED_Clear(void)
功    能：OLED清屏
参    数：None
返回值  ：None
**************************************************************************************/

void OLED_Clear(void)
{
	u8 y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);     
		OLED_WrCmd(0x00);       
		OLED_WrCmd(0x10);	    
		for(x=0;x<128;x++)
		OLED_WrDat(0x00);
	}
} 

/*************************************************************************************
函数名称：void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
功    能：OLED画直线
参    数：x1：起点横坐标,y1：起点纵坐标,x2：终点横坐标, y2：终点纵坐标
返回值  ：None
**************************************************************************************/

void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		OLED_Point(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

/*************************************************************************************
函数名称：void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
功    能：OLED画矩形
参    数：x1：起点横坐标,y1：起点纵坐标,x2：终点横坐标, y2：终点纵坐标
返回值  ：None
**************************************************************************************/

void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	OLED_DrawLine(x1,y1,x2,y1);
	OLED_DrawLine(x1,y1,x1,y2);
	OLED_DrawLine(x1,y2,x2,y2);
	OLED_DrawLine(x2,y1,x2,y2);
}

/*************************************************************************************
函数名称：void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
功    能：OLED画圆
参    数：x1：圆心横坐标,y1：圆心纵坐标,r：半径
返回值  ：None
**************************************************************************************/

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		OLED_Point(x0-b,y0-a);             //3           
		OLED_Point(x0+b,y0-a);             //0           
		OLED_Point(x0-a,y0+b);             //1       
		OLED_Point(x0-b,y0-a);             //7           
		OLED_Point(x0-a,y0-b);             //2             
		OLED_Point(x0+b,y0+a);             //4               
		OLED_Point(x0+a,y0-b);             //5
		OLED_Point(x0+a,y0+b);             //6 
		OLED_Point(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		OLED_Point(x0+a,y0+b);
	}
}	



