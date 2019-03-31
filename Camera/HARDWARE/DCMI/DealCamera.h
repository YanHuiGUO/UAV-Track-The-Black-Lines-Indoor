#ifndef _DEALCAMERA_H
#define _DEALCAMERA_H
#include "DCMI.h"
#include "imu.h"
#define L_BASE 100
#define Pi 3.14159265
extern int threshold;
extern int MyFlashCnt;
extern int GolaCentra_X[5];
extern int GolaCentra_Y[5];
extern int DealData[3];
extern unsigned char OTSU_threshold(unsigned char *pic,unsigned short int num);
extern void labelset(u8 image[CAMERA_H][CAMERA_W],int xs,int ys,int label);
extern int labeling(u8 image_in[CAMERA_H][CAMERA_W],u8 image_label[CAMERA_H][CAMERA_W],int *cnt);
extern float calc_size(u8 image_label[CAMERA_H][CAMERA_W],int label,int *cx,int *cy);
extern void LookGoal(u8 image_label[CAMERA_H][CAMERA_W],int X[5],int Y[5]);
extern void PixToDis(int X[5],int Y[5]);
extern void MediaFilter(u8 * data,int width,int height);
#endif