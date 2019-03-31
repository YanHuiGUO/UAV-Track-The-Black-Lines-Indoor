#include "key_matrix.h"
#include "gpio_cfg.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
#include "key2oled.h"
#include "OLED.h"
//���水��״̬
KEY_MSG   KEY_MSG_FIFO[KEY_MSG_FIFO_MAX];

//�������´������ﵽһ����������Ϊ����
u32 Key_Press_Time[KEY_MAX];

/***********************************************************************************
�������ƣ�void Key_Init(void)
��    �ܣ���ʼ�����̵�IO�˿�
��    ������
����ֵ  ����
************************************************************************************/
void Key_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYIN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		   //����IO����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin  = KEYOUTC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //����IO����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //�����ʽ��������߿�©�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYOUTD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //����IO����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //�����ʽ��������߿�©��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //����������
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
}

/**************************************************************************************
�������ƣ�void  KeyPin_Out(u16 Number)
��    �ܣ�����IO�����
��    ����Number����������ڼ���
����ֵ  ����
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
�������ƣ�void Send_Key_msg(KEY_MSG key_msg)
��    �ܣ��洢����IO�ڼ�ֵ��״̬
��    ����key_msg������״̬��Ϣ
����ֵ  ����
**************************************************************************************/

void Send_Key_msg(KEY_MSG key_msg)
{
	KEY_MSG_FIFO[key_msg.key].status=key_msg.status;
	KEY_MSG_FIFO[key_msg.key].key = key_msg.key;		
}

/*************************************************************************************
�������ƣ�void Send_Key_msg(KEY_MSG key_msg)
��    �ܣ��洢����IO�ڼ�ֵ��״̬
��    ����key_msg������״̬��Ϣ
����ֵ  ����
**************************************************************************************/
/*************************************** 
            key_Val ��Ӧ��ֵ 
      �У�[PC3]  [PC2]  [PC1]   [PC0]
            ��      ��      ��      ��
 �У�
[PD1]��      0      1      2      3   
[PD0]��      4      5      6      7    
[PC5]��      8      9     10     11   
[PC4]��     12     13     14     15   
***************************************/

void Check_Key(void)
{   
  s8 j,row,col,tmp2;
	static s8 blackcount = 0;
  KEY_MSG  key_msg;
  for(row = 0;row < 4;row++)            //��ɨ��
  {
    KeyPin_Out(row);          //B(00001111)                                            
    if((KEY_IN & 0x0f)<0x0f)              //�Ƿ�ĳ������һλΪ0
    {                       
      tmp2 = 0x01;                        // tmp2���ڼ�����һλΪ0  
      for(col = 0;col < 4;col++)          // �м�� 
      {    
        if((KEY_IN & tmp2) == 0x00)
        {   
           tmp2 <<= 1;                       // tmp2����1λ 
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
						tmp2 <<= 1;                       // tmp2����1λ 
					 
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
