#include "nrf24l01.h"
#include "delay.h"
#include "DealCamera.h"
unsigned char  sta;   //状态标志
unsigned char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //本地地址
unsigned char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //接收地址
unsigned char  Tx_Buf[TX_PLOAD_WIDTH]= {3,2}; //发送数据
unsigned char  Rx_Buf[RX_PLOAD_WIDTH]= {0}; //接收数据

void Nrf_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(Nrf24l01Sys, ENABLE);
    RCC_AHB1PeriphClockCmd(Nrf24l01CESys, ENABLE);

    GPIO_InitStructure.GPIO_Pin = Nrf24l01SCK | Nrf24l01SS | Nrf24l01MOSI;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(Nrf24l01PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Nrf24l01CE ;
    GPIO_Init(Nrf24l01CEPORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Nrf24l01MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(Nrf24l01PORT, &GPIO_InitStructure);

    High_24L01_CSN;
    Low_24L01_CE;
    Low_24L01_SCK;
    delay_ms(250);
}
/************************************IO 口模拟SPI总线 代码************************************************/
unsigned char SPI_RW(unsigned char byte)
{
    unsigned char bit_ctr;
    for(bit_ctr=0; bit_ctr<8; bit_ctr++)
    {
        // MOSI=(byte&0x80);
        if(byte&0x80)
        {
            High_24L01_MOSI;
        }
        else
        {
            Low_24L01_MOSI;
        }
        //delayus(10);
        byte=(byte<<1);
        High_24L01_SCK;        //上升沿
        //delayus(10);
        if(Read_24L01_MISO)        //MISO
            byte+=1;          //同时用一个变量来完成收和发。
        Low_24L01_SCK;      //下降沿
        //delayus(10);
    }
    return(byte);         //返回收到的数据
}

unsigned char SPI_RW_Reg  (unsigned char  reg,unsigned char value) // 向寄存器REG写一个字节，同时返回状态字节
{
    unsigned char status;
// CSN=0;
    Low_24L01_CSN;
    status=SPI_RW(reg);
    SPI_RW(value);
// CSN=1;
    High_24L01_CSN;
    return(status);
}
unsigned char SPI_Read (unsigned char  reg )
{
    unsigned char reg_val;
    //CSN=0;
    Low_24L01_CSN;
    SPI_RW(reg);
    reg_val=SPI_RW(0);
    //CSN=1;
    High_24L01_CSN;
    return(reg_val);
}
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
    unsigned char status,byte_ctr;
    //CSN = 0;                   // Set CSN low, init SPI tranaction
    Low_24L01_CSN;
    status = SPI_RW(reg);    // Select register to write to and read status byte
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
        SPI_RW(*pBuf++);
    //CSN = 1;
    High_24L01_CSN;              // Set CSN high again
    return(status);          // return nRF24L01 status byte
}

/*******************************接*****收*****模*****式*****代*****码*************************************/
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char uchars)
{
    unsigned char status,uchar_ctr;

//CSN = 0;                      // Set CSN low, init SPI tranaction
    Low_24L01_CSN;
    status = SPI_RW(reg);         // Select register to write to and read status unsigned char

    for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++)
        pBuf[uchar_ctr] = SPI_RW(0);    //

//CSN = 1;
    High_24L01_CSN;
    return(status);                    // return nRF24L01 status uchar
}
/******************************************************************************************************/
//函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//功能：数据读取后放如rx_buf接收缓冲区中
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;
    sta=SPI_Read(STATUS); // 读取状态寄存其来判断数据接收状况
    if(RX_DR)    // 判断是否接收到数据
    {
        //CE = 0;    //SPI使能
        SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
        revale =1;   //读取数据完成标志
        //delay(100);
    }
    SPI_RW_Reg(NRF24L01WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
    return revale;
}
void TX_Mode(void)
{
//CE=0;
    Low_24L01_CE;
    SPI_RW_Reg(FLUSH_TX,0x00);
    SPI_Write_Buf(NRF24L01WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
    SPI_Write_Buf(NRF24L01WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
    SPI_RW_Reg(NRF24L01WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
    SPI_RW_Reg(NRF24L01WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
    SPI_RW_Reg(NRF24L01WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...1a
    SPI_RW_Reg(NRF24L01WRITE_REG + RF_CH, 40);        // Select RF channel 40
    SPI_RW_Reg(NRF24L01WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
    SPI_RW_Reg(NRF24L01WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为2字节
    SPI_RW_Reg(NRF24L01WRITE_REG + CONFIG, 0x0e);
//CE=1;
    High_24L01_CE;
    delay_ms(100);
}
void Transmit(unsigned char * tx_buf)
{
//CE=0;   //StandBy I模式
    Low_24L01_CE;
    SPI_Write_Buf(NRF24L01WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
    SPI_RW_Reg(FLUSH_TX,0x00);
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);     // 装载数据
    SPI_RW_Reg(NRF24L01WRITE_REG + CONFIG, 0x0e);      // IRQ收发完成中断响应，16位CRC，主发送
//CE=1;   //置高CE，激发数据发送
    High_24L01_CE;
//delay_ms(10);
}
/****************************************************************************************************/
//函数：void RX_Mode(void)
//功能：数据接收配置
/****************************************************************************************************/
void RX_Mode(void)
{
//CE=0;
    Low_24L01_CE;
    SPI_RW_Reg(FLUSH_RX,0x00);
//SPI_Write_Buf(NRF24L01WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
    SPI_Write_Buf(NRF24L01WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack

    SPI_RW_Reg(NRF24L01WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
    SPI_RW_Reg(NRF24L01WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
    //SPI_RW_Reg(NRF24L01WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...1a
    SPI_RW_Reg(NRF24L01WRITE_REG + RF_CH, 40);        // Select RF channel 40
    SPI_RW_Reg(NRF24L01WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为2字节
    SPI_RW_Reg(NRF24L01WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
    SPI_RW_Reg(NRF24L01WRITE_REG + CONFIG, 0x0F);
    High_24L01_CE;
    delay_ms(130);   //好像没有也可以，AVR 22.1184M
}
void NrfTransmit(void)
{
    Transmit(Tx_Buf);
    sta=SPI_Read(NRF24L01READ_REG +  STATUS);
    if(TX_DS)
    {
        SPI_RW_Reg(NRF24L01WRITE_REG + STATUS,sta);
    }
    if(MAX_RT)  //如果是发送超时
    {
        SPI_RW_Reg(NRF24L01WRITE_REG + STATUS,sta);
    }
}
void NrfSend_MyData(int *p,int p_Len)
{
	int i ,j= 0;
	unsigned char Send_Temp =0;
	Tx_Buf[0]=0xff;
	for(i=1,j=0;i<p_Len*2;j++)
	{
		Send_Temp = p[j] >>8;
		if(Send_Temp==0xff)
			Send_Temp = 0xfe;
		Tx_Buf[i] = Send_Temp;
		i++;
		Send_Temp = p[j]&0xff;
		if(Send_Temp==0xff)
		Send_Temp = 0xfe;
		Tx_Buf[i] = Send_Temp;
		i++;
	}
	Tx_Buf[i] = Tx_Buf[1]+Tx_Buf[i-1];
	NrfTransmit();
}


