#include "absoluteEncoder.h"
void abEncoder_delay(void)
{
		u8 i=100;
		while(i--);
}

void abEncoder_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_AHB1PeriphClockCmd(ABEncoder_RCC , ENABLE );
  GPIO_InitStructure.GPIO_Pin =  ABEncoder_Pin_CLK |ABEncoder_Pin_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(ABEncoder_GPIO, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin=ABEncoder_Pin_D0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(ABEncoder_GPIO, &GPIO_InitStructure);

	ABEncoder_CLK_L;
//	ABEncoder_D0_H;
	ABEncoder_CS_H;
}

u16 abEncoder_read(void)
{ 
		u8 i=14;
    u16 ReceiveByte=0;
		ABEncoder_CS_L;	
		delay_us(1);
		ABEncoder_CLK_H;
		abEncoder_delay();
		ABEncoder_CLK_L;
		ABEncoder_D0_read;
		abEncoder_delay();
		ABEncoder_CLK_H;
		abEncoder_delay();
		ABEncoder_CLK_L;
		ABEncoder_D0_read;
		abEncoder_delay();
	  			
    while(i--)
    {
      ReceiveByte<<=1;      
      ABEncoder_CLK_H;
      abEncoder_delay();
			ABEncoder_CLK_L;
      
      if(ABEncoder_D0_read)
      {
        ReceiveByte|=0x0001;
      }
    }
			abEncoder_delay();
			ABEncoder_CS_H;	

    return ReceiveByte;
		
}
