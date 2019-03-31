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

u8 *	    IMG_BUFF_1;       //图像缓冲区指针  
u8 *	    IMG_BUFF_2;       //图像缓冲区指针  


static u8 Ov7620_reg_Init(void);

void Ov7620_exti_Init()
{
	//DMA通道0初始化，PTB8上升沿触发DMA传输，源地址为PTB_BYTE0_IN，目的地址为：BUFF ，每次传输1Byte，传输CAMERA_SIZE次后停止传输
//	DMA_PORTx2BUFF_Init(CAMERA_DMA_CH, (void *)&PTB_BYTE0_IN, (void *)IMG_BUFF_1, PTB8, DMA_BYTE1, CAMERA_DMA_NUM, DMA_rising_keepon);
//	DMA_IRQ_DIS(DMA_CH0);	            //不需要使用DMA中断,由行中断来控制DMA传输
//	exti_init(PORTA,29,rising_down);	//场中断，内部下拉，上升沿触发中断
//    exti_init(PORTA,28,rising_down); 	//行中断，内部下拉，上升沿触发中断
//    disable_irq(87); 				    //关闭PTA的中断
}

void ov7620_get_img()
{
    img_flag = IMG_START;					//开始采集图像
    PORTA_ISFR=~0;							//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    enable_irq(87); 						//允许PTA的中断
    while(img_flag != IMG_FINISH)           //等待图像采集完毕
    {
        if(img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
        {
            img_flag = IMG_START;			//开始采集图像
           // Send_Str("Get image error!\n");
            PORTA_ISFR=~0;					//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
            enable_irq(87); 				//允许PTA的中断
        }
    }
}

    
/*OV7620初始化配置表*/
Register_Info ov7620_reg[] =
{
#if 1  //QVGA
    //寄存器，寄存器值次
    {CLKRC     ,0x00},  //分频
    {COMC      ,0x24},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/2/2)},
    {HSIZE     ,(0x7F+CAMERA_W/2/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    {VEEE      ,0x42},  //垂直边缘增强
    {COMH      ,0x00},     //扫描模式
#else //VGA
    //寄存器，寄存器值次
    {CLKRC     ,0x00}, 
    //{COMC      ,0x04},  //0x24 QVGA 0x04 VGA 
    {HSTART    ,(0x7F-CAMERA_W/4/2)},
    {HSIZE     ,(0x7F+CAMERA_W/4/2)},   
    {VSTRT     ,(0X7E-CAMERA_H/2)},
    {VSIZE     ,(0X7E+CAMERA_H/2)-1},
    //{VEEE      ,0x42},  //垂直边缘增强
    //{COMH      ,0x20},  //扫描模式
#endif

};

u8 cfgnum = sizeof(ov7620_reg)/sizeof(ov7620_reg[0]);   /*结构体数组成员数目*/

/************************************************
 * 函数名：Ov7620_Init
 * 描述  ：Sensor初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
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
 * 函数名：Ov7620_reg_Init
 * 描述  ：Sensor 寄存器 初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
 ************************************************/
u8 Ov7620_reg_Init(void)
{
    u16 i = 0;
    while( 1 == sccb_regWrite ( 0x42, 0x12, 0xA4 ) ) /*复位sensor */
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
            Send_DataPlusStr("Warming：write register ",ov7620_reg[i].Address," error!\n" );
            return 0;
         }
    }
    Send_Str("OV7620 Register Config Success!\n");    //time_delay_ms(1000);
    //Buzzer_OFF;
    return 1;
}


