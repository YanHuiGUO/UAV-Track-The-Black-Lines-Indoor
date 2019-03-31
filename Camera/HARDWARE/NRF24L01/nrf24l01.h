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

//****************************************IO�˿ڶ���***************************************

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

//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define NRF24L01READ_REG        0x00   // ���Ĵ���ָ��
#define NRF24L01WRITE_REG       0x20  // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61   // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0   // д��������ָ��
#define FLUSH_TX        0xE1  // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2   // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3   // �����ظ�װ������ָ��
//#define NOP             0xFF   // ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������


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

