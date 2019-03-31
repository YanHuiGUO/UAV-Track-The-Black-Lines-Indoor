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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //ʹ��GPIOFʱ��

  //GPIOB0,B1��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;   //LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOA, &GPIO_InitStructure);                   //��ʼ��GPIO
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5 );           //GPIOB0,B1���õͣ�����
}






