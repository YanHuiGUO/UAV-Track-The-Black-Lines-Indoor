/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��gpio_cfg.h
 * ����         ��gpio�Ķ˿ڶ���ͷ�ļ�
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/
#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

//�˿ں궨��
typedef enum PORTx
{
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
}PORTx;


typedef enum PTxn
{
    /*  PTA�˿�    */ //0~31
    PTA0=0,           //���� JLINK  
    PTA1,           //���� ͬ��
    PTA2,           //���� ͬ��
    PTA3,           //���� ͬ��
    PTA4,           //���� ͬ��
    PTA5,
    PTA6,
    PTA7,
    PTA8,           //QEI
    PTA9,           //
    PTA10,          //QEI
    PTA11,          //
    PTA12,
    PTA13,
    PTA14,          //����
    PTA15,          //ͬ��


    /*  PTB�˿�    */   //32~63
    PTB0,               //B0~B7���� ����ͷ���������
    PTB1,               //
    PTB2,               //
    PTB3,               //
    PTB4,               //
    PTB5,               //
    PTB6,               //
    PTB7,               //
    PTB8,               //PCLK
    PTB9,               
    PTB10,              //SCCB 
    PTB11,              //SCCB
    PTB12,
    PTB13,
    PTB14,
    PTB15,


    /*  PTC�˿�    */
    PTC0,       
    PTC1,       // C1~C4 ���Ƶ�� PWM
    PTC2,       //  
    PTC3,       //
    PTC4,       //
    PTC5,       // PTC5~PTC12 �������
    PTC6,       // 
    PTC7,       //
    PTC8,       //
    PTC9,       //
    PTC10,      //
    PTC11,      //
    PTC12,      //led
    PTC13,      // 
    PTC14,      //    
    PTC15,      //


    /*  PTD�˿�    */
    PTD0,       //����������3
    PTD1,
    PTD2,
    PTD3,       //5110
    PTD4,       //�ɿؿ���һ
    PTD5,       //5110
    PTD6,       //�ɿؿ��ض�
    PTD7,       //5110
    PTD8,       //�ɿؿ�����
    PTD9,       //5110
    PTD10,      //�ɿؿ�����
    PTD11,      //5110 
    PTD12,      //led D12~D15
    PTD13,      //
    PTD14,      //
    PTD15,      //


    /*  PTE�˿�    */
    PTE0,           //TF��
    PTE1,           //ͬ��
    PTE2,           //ͬ��
    PTE3,           //ͬ��
    PTE4,           //ͬ��
    PTE5,           //ͬ��
    PTE6,           //ͬ��
    PTE7,
    PTE8,           //������
    PTE9,           //����������1
    PTE10,          //�������ж�1
    PTE11,          //����������2
    PTE12,          //�������ж�2
    PTE13,          
    PTE14,          
    PTE15,          
} PTxn;

#endif
