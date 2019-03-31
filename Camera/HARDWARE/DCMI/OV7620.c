#include "common.h"
//#include "gpio.h"
//#include "exti.h"
//#include "dma.h"
#include "SCCB.h"
#include "OV7620.h"
#include "OV7620_REG.h"
#include "UartLink.h"

#define     OV7620_Delay_ms(time)  delay_ms(time)

volatile    IMG_STATE  	img_flag=IMG_FINISH;

u8 *	    IMG_BUFF_1;       //ͼ�񻺳���ָ��  
u8 *	    IMG_BUFF_2;       //ͼ�񻺳���ָ��  


static u8 Ov7620_reg_Init(void);

void Ov7620_exti_Init()
{
	//DMAͨ��0��ʼ����PTB8�����ش���DMA���䣬Դ��ַΪPTB_BYTE0_IN��Ŀ�ĵ�ַΪ��BUFF ��ÿ�δ���1Byte������CAMERA_SIZE�κ�ֹͣ����
//	DMA_PORTx2BUFF_Init(CAMERA_DMA_CH, (void *)&PTB_BYTE0_IN, (void *)IMG_BUFF_1, PTB8, DMA_BYTE1, CAMERA_DMA_NUM, DMA_rising_keepon);
//	DMA_IRQ_DIS(DMA_CH0);	            //����Ҫʹ��DMA�ж�,�����ж�������DMA����
//	exti_init(PORTA,29,rising_down);	//���жϣ��ڲ������������ش����ж�
//    exti_init(PORTA,28,rising_down); 	//���жϣ��ڲ������������ش����ж�
//    disable_irq(87); 				    //�ر�PTA���ж�
}

void ov7620_get_img()
{
    img_flag = IMG_START;					//��ʼ�ɼ�ͼ��
    PORTA_ISFR=~0;							//д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
    enable_irq(87); 						//����PTA���ж�
    while(img_flag != IMG_FINISH)           //�ȴ�ͼ��ɼ����
    {
        if(img_flag == IMG_FAIL)            //����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            img_flag = IMG_START;			//��ʼ�ɼ�ͼ��
           // Send_Str("Get image error!\n");
            PORTA_ISFR=~0;					//д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
            enable_irq(87); 				//����PTA���ж�
        }
    }
}

    
/*OV7620��ʼ�����ñ�*/
Register_Info ov7620_reg[] =
{
#if 1  //QVGA
    //�Ĵ������Ĵ���ֵ��
    {CLKRC     ,0x00},  //��Ƶ
    {COMC      ,0x24},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/2/2)},
    {HSIZE     ,(0x7F+CAMERA_W/2/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    {VEEE      ,0x42},  //��ֱ��Ե��ǿ
    {COMH      ,0x00},     //ɨ��ģʽ
#else //VGA
    //�Ĵ������Ĵ���ֵ��
    {CLKRC     ,0x00}, 
    //{COMC      ,0x04},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/4/2)},
    {HSIZE     ,(0x7F+CAMERA_W/4/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    //{VEEE      ,0x42},  //��ֱ��Ե��ǿ
    //{COMH      ,0x20},  //ɨ��ģʽ
#endif

};

u8 cfgnum = sizeof(ov7620_reg)/sizeof(ov7620_reg[0]);   /*�ṹ�������Ա��Ŀ*/

/************************************************
 * ��������Ov7620_Init
 * ����  ��Sensor��ʼ��
 * ����  ����
 * ���  ������1�ɹ�������0ʧ��
 * ע��  ����
 ************************************************/
u8 Ov7620_Init(u8 *imgaddr)
{
    IMG_BUFF_1 = imgaddr;
    sccb_init();
	while(Ov7620_reg_Init() == 0);
	Ov7620_exti_Init();
    return 0;
}

/************************************************
 * ��������Ov7620_reg_Init
 * ����  ��Sensor �Ĵ��� ��ʼ��
 * ����  ����
 * ���  ������1�ɹ�������0ʧ��
 * ע��  ����
 ************************************************/
u8 Ov7620_reg_Init(void)
{
    u16 i = 0;
    while( 1 == sccb_regWrite ( 0x42, 0x12, 0xA4 ) ) /*��λsensor */
    {
        i++;
		if(i == 20)
		{
            Send_Str("Warming:SCCB write date error!\n");
			return 0 ;
		}		
    }
    for( i = 0 ; i < cfgnum ; i++ )
    {
         while( 1 == sccb_regWrite(0x42,ov7620_reg[i].Address, ov7620_reg[i].Value) )
         {
            Send_DataPlusStr("Warming��write register ",ov7620_reg[i].Address," error!\n" );
            return 0;
         }
    }
    Send_Str("OV7620 Register Config Success!\n");    //time_delay_ms(1000);
    //Buzzer_OFF;
    return 1;
}


