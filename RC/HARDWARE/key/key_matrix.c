#include "key_matrix.h"
#include "gpio_cfg.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
#include "key2oled.h"
#include "OLED.h"
//±£´æ°´¼ü×´Ì¬
KEY_MSG   KEY_MSG_FIFO[KEY_MSG_FIFO_MAX];

//°´¼ü°´ÏÂ´ÎÊı£¬´ïµ½Ò»¶¨´ÎÊı²ÅÈÏÎª°´ÏÂ
u32 Key_Press_Time[KEY_MAX];

/***********************************************************************************
º¯ÊıÃû³Æ£ºvoid Key_Init(void)
¹¦    ÄÜ£º³õÊ¼»¯¼üÅÌµÄIO¶Ë¿Ú
²Î    Êı£ºÎŞ
·µ»ØÖµ  £ºÎŞ
************************************************************************************/
void Key_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYIN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		   //ÅäÖÃIO·½Ïò
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //ÉèÖÃÉÏÏÂÀ­
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin  = KEYOUTC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //ÅäÖÃIO·½Ïò
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //Êä³ö·½Ê½£¨ÍÆÍì»òÕß¿ªÂ©£
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //ÉèÖÃÉÏÏÂÀ­
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYOUTD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //ÅäÖÃIO·½Ïò
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //Êä³ö·½Ê½£¨ÍÆÍì»òÕß¿ªÂ©£©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //ÉèÖÃÉÏÏÂÀ­
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
}

/**************************************************************************************
º¯ÊıÃû³Æ£ºvoid  KeyPin_Out(u16 Number)
¹¦    ÄÜ£º¼üÅÌIO¿ÚÊä³ö
²Î    Êı£ºNumber£º¼üÅÌÊä³öµÚ¼¸ĞĞ
·µ»ØÖµ  £ºÎŞ
***************************************************************************************/

void  KeyPin_Out(u16 Number)   
{
	switch(Number)
	{
		case 0: Key_Out_0;break;
		case 1: Key_Out_1;break;
		case 2: Key_Out_2;break;
		case 3: Key_Out_3;break;
		default:break;
	}
}

/*************************************************************************************
º¯ÊıÃû³Æ£ºvoid Send_Key_msg(KEY_MSG key_msg)
¹¦    ÄÜ£º´æ´¢¼üÅÌIO¿Ú¼üÖµÓë×´Ì¬
²Î    Êı£ºkey_msg£º¼üÅÌ×´Ì¬ĞÅÏ¢
·µ»ØÖµ  £ºÎŞ
**************************************************************************************/

void Send_Key_msg(KEY_MSG key_msg)
{
	KEY_MSG_FIFO[key_msg.key].status=key_msg.status;
	KEY_MSG_FIFO[key_msg.key].key = key_msg.key;		
}

/*************************************************************************************
º¯ÊıÃû³Æ£ºvoid Send_Key_msg(KEY_MSG key_msg)
¹¦    ÄÜ£º´æ´¢¼üÅÌIO¿Ú¼üÖµÓë×´Ì¬
²Î    Êı£ºkey_msg£º¼üÅÌ×´Ì¬ĞÅÏ¢
·µ»ØÖµ  £ºÎŞ
**************************************************************************************/
/*************************************** 
            key_Val ¶ÔÓ¦¼üÖµ 
      ÁĞ£º[PC3]  [PC2]  [PC1]   [PC0]
            ¡ı      ¡ı      ¡ı      ¡ı
 ĞĞ£º
[PD1]¡ú      0      1      2      3   
[PD0]¡ú      4      5      6      7    
[PC5]¡ú      8      9     10     11   
[PC4]¡ú     12     13     14     15   
***************************************/

void Check_Key(void)
{   
  s8 j,row,col,tmp2;
	static s8 blackcount = 0;
  KEY_MSG  key_msg;
  for(row = 0;row < 4;row++)            //ĞĞÉ¨Ãè
  {
    KeyPin_Out(row);          //B(00001111)                                            
    if((KEY_IN & 0x0f)<0x0f)              //ÊÇ·ñÄ³ÁĞÖĞÓĞÒ»Î»Îª0
    {                       
      tmp2 = 0x01;                        // tmp2ÓÃÓÚ¼ì²â³öÄÄÒ»Î»Îª0  
      for(col = 0;col < 4;col++)          // ÁĞ¼ì²â 
      {    
        if((KEY_IN & tmp2) == 0x00)
        {   
           tmp2 <<= 1;                       // tmp2×óÒÆ1Î» 
           Key_Press_Time[row*4+3-col]++;  
           if(Key_Press_Time[row*4+3-col] <= KEY_DOWN_TIME)
           {
						 col = -1;
						 tmp2 = 0x01; 
						 continue;
           }
           else 
           {
						 for(j = 0;j < 16;j++)
						 KEY_MSG_FIFO[j].status = KEY_UP;
						 key_msg.key=(KEY_e)(row*4+3-col);   
						 key_msg.status=KEY_DOWN;
						 Send_Key_msg(key_msg);
				
           }
         }
         else
         {
						key_msg.key=(KEY_e)(row*4+3-col); 
						key_msg.status=KEY_UP;
						Key_Press_Time[row*4+3-col]=0;
						Send_Key_msg(key_msg);  
						tmp2 <<= 1;                       // tmp2×óÒÆ1Î» 
					 
         }
       }
    }
		else 
		{
			blackcount++;
			if(blackcount >=40)
				{
					blackcount = 0;						
					 for(j = 0;j < 16;j++)
					{
						 KEY_MSG_FIFO[j].status = KEY_UP;
						 Key_Press_Time[j] = 0;
					}
				}
				
		}
  }		
}   
