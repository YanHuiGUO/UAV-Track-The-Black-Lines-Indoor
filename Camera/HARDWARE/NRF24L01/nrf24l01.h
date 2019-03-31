#ifndef __NRF24L01_H_
#define __NRF24L01_H_

#include "Common.h"
//SS PB12 SCK PB13 MISO PB14 MOSI PB15 CE PD3 IRQ PD4
#define Nrf24l01SCK  GPIO_Pin_2
#define Nrf24l01SS   GPIO_Pin_3
#define Nrf24l01MISO GPIO_Pin_0
#define Nrf24l01MOSI GPIO_Pin_1
#define Nrf24l01Sys  RCC_AHB1Periph_GPIOC
#define Nrf24l01PORT GPIOC

#define Nrf24l01CE     GPIO_Pin_0
#define Nrf24l01CESys  RCC_AHB1Periph_GPIOA
#define Nrf24l01CEPORT GPIOA

//****************************************IO端口定义***************************************

#define   Read_24L01_MISO    (Nrf24l01PORT->IDR & Nrf24l01MISO)

#define   High_24L01_MOSI    Nrf24l01PORT->BSRRL = Nrf24l01MOSI
#define   Low_24L01_MOSI     Nrf24l01PORT->BSRRH = Nrf24l01MOSI

#define   High_24L01_CSN     Nrf24l01PORT->BSRRL = Nrf24l01SS
#define   Low_24L01_CSN      Nrf24l01PORT->BSRRH = Nrf24l01SS

#define   High_24L01_SCK     Nrf24l01PORT->BSRRL = Nrf24l01SCK
#define   Low_24L01_SCK      Nrf24l01PORT->BSRRH = Nrf24l01SCK

#define   High_24L01_CE      Nrf24l01CEPORT->BSRRL = Nrf24l01CE
#define   Low_24L01_CE       Nrf24l01CEPORT->BSRRH = Nrf24l01CE

#define RX_DR sta&0x40
#define TX_DS sta&0x20
#define MAX_RT sta&0x10
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5    // 5 uints TX address width
#define RX_ADR_WIDTH    5    // 5 uints RX address width
#define TX_PLOAD_WIDTH  32   // 32 uints TX payload
#define RX_PLOAD_WIDTH  32   // 32 uints TX payload

//***************************************NRF24L01寄存器指令*******************************************************
#define NRF24L01READ_REG        0x00   // 读寄存器指令
#define NRF24L01WRITE_REG       0x20  // 写寄存器指令
#define RD_RX_PLOAD     0x61   // 读取接收数据指令
#define WR_TX_PLOAD     0xA0   // 写待发数据指令
#define FLUSH_TX        0xE1  // 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2   // 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3   // 定义重复装载数据指令
//#define NOP             0xFF   // 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置


void Nrf_Init(void);
unsigned char SPI_RW(unsigned char byte);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void TX_Mode(void);
void Transmit(unsigned char * tx_buf);
void RX_Mode(void);
void NrfTransmit(void);
void NrfSend_MyData(int *p,int p_Len);
extern unsigned char Rx_Buf[RX_PLOAD_WIDTH];
extern unsigned char Tx_Buf[RX_PLOAD_WIDTH];




#endif

