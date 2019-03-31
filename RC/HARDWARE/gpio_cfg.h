/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：gpio_cfg.h
 * 描述         ：gpio的端口定义头文件
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/
#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

//端口宏定义
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
    /*  PTA端口    */ //0~31
    PTA0=0,           //已用 JLINK  
    PTA1,           //已用 同上
    PTA2,           //已用 同上
    PTA3,           //已用 同上
    PTA4,           //已用 同上
    PTA5,
    PTA6,
    PTA7,
    PTA8,           //QEI
    PTA9,           //
    PTA10,          //QEI
    PTA11,          //
    PTA12,
    PTA13,
    PTA14,          //无线
    PTA15,          //同上


    /*  PTB端口    */   //32~63
    PTB0,               //B0~B7已用 摄像头数据输出口
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


    /*  PTC端口    */
    PTC0,       
    PTC1,       // C1~C4 控制电机 PWM
    PTC2,       //  
    PTC3,       //
    PTC4,       //
    PTC5,       // PTC5~PTC12 矩阵键盘
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


    /*  PTD端口    */
    PTD0,       //超声波触发3
    PTD1,
    PTD2,
    PTD3,       //5110
    PTD4,       //飞控开关一
    PTD5,       //5110
    PTD6,       //飞控开关二
    PTD7,       //5110
    PTD8,       //飞控开关三
    PTD9,       //5110
    PTD10,      //飞控开关四
    PTD11,      //5110 
    PTD12,      //led D12~D15
    PTD13,      //
    PTD14,      //
    PTD15,      //


    /*  PTE端口    */
    PTE0,           //TF卡
    PTE1,           //同上
    PTE2,           //同上
    PTE3,           //同上
    PTE4,           //同上
    PTE5,           //同上
    PTE6,           //同上
    PTE7,
    PTE8,           //蜂鸣器
    PTE9,           //超声波触发1
    PTE10,          //超声波中断1
    PTE11,          //超声波触发2
    PTE12,          //超声波中断2
    PTE13,          
    PTE14,          
    PTE15,          
} PTxn;

#endif
