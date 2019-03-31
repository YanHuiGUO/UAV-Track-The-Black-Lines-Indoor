#include "led.h" 

/***************************************************************************************
@f_name: void LED_Init(void)
@brief:	 ��ʼ��PB0��PB1ΪLED�����
@param:	 None
@return: None
****************************************************************************************/ 

void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //ʹ��GPIOFʱ��

  //GPIOB0,B1��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   //LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);                   //��ʼ��GPIO
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);           //GPIOB0,B1���õͣ�����
}






