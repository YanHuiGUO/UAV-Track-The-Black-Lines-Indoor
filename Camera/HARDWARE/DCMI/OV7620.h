#ifndef __OV7620_H
#define __OV7620_H 

#include "common.h"

#define	CAMERA_DMA_CH 	    DMA_CH0		    //��������ͷ��DMA�ɼ�ͨ��
#define CAMERA_W            120            	//��������ͷͼ����
#define CAMERA_H            120             //��������ͷͼ��߶�
#define CAMERA_INTERLACE    1              	//����ͷ����ɼ����� n - 1,����1��ʾ�����вɼ���2��ʾ���вɼ�

#define CAMERA_DMA_LINE     (CAMERA_H / CAMERA_INTERLACE)     //ʵ�ʲɼ�����
#define CAMERA_DMA_NUM      (CAMERA_W)                        //DMA�ɼ�����
#define CAMERA_SIZE         (CAMERA_W * CAMERA_DMA_LINE)             //ͼ��ռ�ÿռ��С
#define CAMERA_TRA_SIZE     ((CAMERA_W - 6)* CAMERA_DMA_LINE/8)             //ͼ��ռ�ÿռ��С

typedef struct
{
    bool ImageTxOK;
    bool ImageDealOK;
    bool ImageOK;
}ImageDeal;

//����ͼ��ɼ�״̬
typedef enum 
{
    IMG_NOTINIT=0,
	IMG_FINISH,			    //ͼ��ɼ����
	IMG_FAIL,				//ͼ��ɼ�ʧ��(�ɼ���������)
	IMG_GATHER,				//ͼ��ɼ���
	IMG_START,				//��ʼ�ɼ�ͼ��
	IMG_STOP,				//��ֹͼ��ɼ�
}IMG_STATE;

typedef struct
{
	u8 Address;			       /*�Ĵ�����ַ*/
	u8 Value;		           /*�Ĵ���ֵ*/
}Register_Info;


extern	u8      Ov7620_Init(u8 *imgaddr);
extern	void    Ov7620_exti_Init();
extern	void    ov7620_get_img();

#endif























