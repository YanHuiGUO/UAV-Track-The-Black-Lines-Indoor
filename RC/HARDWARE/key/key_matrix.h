#ifndef _Key_matrix_
#define _Key_matrix_

#include "common.h"

#define KEY_DOWN_TIME  10

//最大存储的键盘信息
#define   KEY_MSG_FIFO_MAX     16          

#define  KEYIN_PIN     	(pin0|pin1|pin2|pin3)
#define  KEYOUTC_PIN    (pin4|pin5)
#define  KEYOUTD_PIN    (pin0|pin1)

#define  KEY_IN        	(uint16_t)(GPIOC->IDR & 0x0f)

#define  Key_Out_3      (GPIOC->BSRRH = GPIO_Pin_4);(GPIOC->BSRRL = GPIO_Pin_5);(GPIOD->BSRRL = GPIO_Pin_0);(GPIOD->BSRRL = GPIO_Pin_1)
#define  Key_Out_2      (GPIOC->BSRRH = GPIO_Pin_5);(GPIOC->BSRRL = GPIO_Pin_4);(GPIOD->BSRRL = GPIO_Pin_0);(GPIOD->BSRRL = GPIO_Pin_1)
#define  Key_Out_1      (GPIOD->BSRRH = GPIO_Pin_0);(GPIOD->BSRRL = GPIO_Pin_1);(GPIOC->BSRRL = GPIO_Pin_4);(GPIOC->BSRRL = GPIO_Pin_5)
#define  Key_Out_0      (GPIOD->BSRRH = GPIO_Pin_1);(GPIOD->BSRRL = GPIO_Pin_0);(GPIOC->BSRRL = GPIO_Pin_4);(GPIOC->BSRRL = GPIO_Pin_5)

//KEY的种类
typedef enum 
{ 
	KEY_ONE = 0,
	KEY_TWO,  
	KEY_THREE,
	KEY_MODE_A,
	
	KEY_FOUR,
	KEY_FIVE,
	KEY_SIX,
	KEY_MODE_B,
	
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
	KEY_MODE_C,
		 
	KEY_SUB,
	KEY_ZERO, 
	KEY_ADD,
	kEY_RUN,
    
  KEY_MAX,
}KEY_e;

typedef enum
{
  KEY_UP,
  KEY_DOWN,
}KEY_STATUS;

typedef struct
{
  KEY_e   key;
  KEY_STATUS status;
}KEY_MSG;

extern KEY_MSG   KEY_MSG_FIFO[KEY_MSG_FIFO_MAX];

void  KeyPin_Out(u16 Number);
void  Key_Init(void);
void  Send_Key_msg(KEY_MSG key_msg);
void  Check_Key(void);

#endif
