#include "DealCamera.h"
int threshold=0;
int mylabelnum = 0;
int Centra_X,Centra_Y = 0;
int MyFlashCnt = 0;
//左标记，右标记，上标记，下标记，激光点
int GolaCentra_X[5] = {0,0,0,0,0};
int GolaCentra_Y[5] = {0,0,0,0,0};
//处理得到物理坐标
//									Y
//									^
//									|
//									|
//									|
//									|
//X<----------------
int DealData[3];
#define Model_Num 3
//中值滤波模板数组
int MediaModel[Model_Num][Model_Num]={{0,1,2},{100,101,102},{200,201,202}};
//   0 		1 		2
//0 0000 0001	0002
//1	0100 0101	0102
//2	0200 0201	0202
/***************************************************************************************
@f_name: void MediaFilter(u8 * data,int width,int height)
@brief:	 图像均值滤波
@param:	 None
@return: None
****************************************************************************************/
void MediaFilter(u8 *data,int width,int height)
{
	int i,j=0;
	char m,n=0;
	for(i = 0;i <height;i++ )
		for(j =0;j<width;j++)
	{
		for(m=0;m<Model_Num;m++)
			for (n = 0;n<Model_Num;n++)
					MediaModel[m][n] = i*100 + j;
		
		for(m=0;m<Model_Num;m++)
			for (n = 0;n<Model_Num;n++)
					{
						data[MediaModel[(Model_Num +1) /2][(Model_Num +1) /2]]	=	Moving_Median(0,Model_Num*Model_Num,data[((int)MediaModel[m][n]/100*width)\
																																	+((int)MediaModel[m][n]/10%10+(int)MediaModel[m][n]%10)]);
					}
						
	}
}
/***************************************************************************************
@f_name: u8 OTSU_threshold(u8 *pic,uint16_t num)
@brief:	 动态阈值
@param:	 None
@return: None
****************************************************************************************/
#define Gourd 256
unsigned char OTSU_threshold(unsigned char *pic,unsigned short int num)
{
   unsigned short int i=0;
   unsigned short int Histogram[Gourd];  
	float pt[Gourd],w[Gourd],u[Gourd],o[Gourd],Ut;
	  int maxi=0;
  float maxo=0;
   for (i=0;i<Gourd;i++)Histogram[i]=0;//

   for (i=0;i<num;i++)
   {
       Histogram[(int)pic[i]*Gourd/256]++;
		 
   }

  pt[0]=(float)Histogram[0]/num;
  w[0]=pt[0];
  u[0]=w[0];

  for(i=1;i<Gourd;i++)
  {
    pt[i]=(float)Histogram[i]/num;
    w[i]=w[i-1]+pt[i];
    u[i]=u[i-1]+i*pt[i];
  };
  Ut=u[Gourd-1];

  for(i=0;i<Gourd;i++)
  {
    o[i]=(1-pt[i])*(u[i]*u[i]/w[i]+(u[i]-Ut)*(u[i]-Ut)/(1-w[i]));
  };



  for(i=0;i<Gourd;i++)
  {
    if(o[i]!=0x7FC0000)
    if(o[i]>maxo){maxo=o[i];maxi=i;}

  }
  return maxi*256/Gourd;
}
/***************************************************************************************
@f_name: int labeling(u8 image_in[CAMERA_H][CAMERA_W],u8 image_label[CAMERA_H][CAMERA_W],int *cnt)
@brief:	 贴标签
@param:	 None
@return: None
****************************************************************************************/
#define High_Base 1
int labeling(u8 image_in[CAMERA_H][CAMERA_W],u8 image_label[CAMERA_H][CAMERA_W],int *cnt)
{
  int i,j,label;  
  for(i=0;i<CAMERA_H;i++)//
    for(j=0;j<CAMERA_W;j++) 
        image_label[i][j] = image_in[i][j];
		label = L_BASE;
    for(i=0;i<CAMERA_H;i++)//
      for(j=0;j<CAMERA_W;j++) 
      {
          if(image_label[i][j]==High_Base)
          {
            labelset(image_label,j,i,label);
            label ++ ;
          }
      }
   *cnt = label-L_BASE;
   return 0 ;
}
/***************************************************************************************
@f_name: void labelset(u8 image[CAMERA_H][CAMERA_W],int xs,int ys,int label)
@brief:	 给互联像素贴上标签
@param:	 None
@return: None
****************************************************************************************/
void labelset(u8 image[CAMERA_H][CAMERA_W],int xs,int ys,int label)
{
  int i,j,cnt,im,ip,jm,jp;
  image[ys][xs] = label;
  while(1)
  {
    cnt = 0;
    for(i=0;i<CAMERA_H;i++)
      for(j=0;j<CAMERA_W;j++) 
      {
        if(image[i][j] == label)
        {
          im = i-1;ip = i+1;jm=j-1;jp=j+1;
          if(im<0) im = 0;if(ip>CAMERA_H -1) ip = CAMERA_H -1 ;
          if(jm<0) jm = 0;if(jp>CAMERA_W -1) jp = CAMERA_W -1 ;
          
          if (image[i][jp] == High_Base)
          {
            image[i][jp]  = label ; cnt ++;
          }         
           if (image[im][jp] == High_Base)
          {
            image[im][jp]  = label ; cnt ++;
          }
          
           if (image[im][j] == High_Base)
          {
            image[im][j]  = label ; cnt ++;
          }
           
          if (image[im][jm] == High_Base)
          {
            image[im][jm]  = label ; cnt ++;
          }
           if (image[i][jm] == High_Base)
          {
            image[i][jm]  = label ; cnt ++;
          }
           if (image[ip][jm] == High_Base)
          {
            image[ip][jm]  = label ; cnt ++;
          }
           if (image[ip][j] == High_Base)
          {
            image[ip][j]  = label ; cnt ++;
          }
           if (image[ip][jp] == High_Base)
          {
            image[ip][jp]  = label ; cnt ++;
          }
        }
      
      } 
    if(cnt == 0) break;//????????????????????
  }
}
/***************************************************************************************
@f_name: float calc_size(u8 image_label[CAMERA_H][CAMERA_W],int label,int *cx,int *cy)
@brief:	 计算标签数组的重心，面积
@param:	 None
@return: None
****************************************************************************************/
float calc_size(u8 image_label[CAMERA_H][CAMERA_W],int label,int *cx,int *cy)
{
  int i,j;
  float tx,ty,total;
  int num =0;
  tx = 0;ty = 0;total = 0;
   for(i=0;i<CAMERA_H;i++)
   {
      for(j=0;j<CAMERA_W;j++) 
      {
        if(image_label[i][j] == High_Base)
        {
          tx+=j;ty +=i;total++;
         // myGoalCircle = trace(image_label,j-1,i);
        }
//           if(image_label[i][j] >=threshold)//??
//           {
//             tx+=j;ty +=i;total++;
//               
//          }
        }   
           
   }
   if(total == 0) {  *cx = 0;*cy=0; return 0.0;}
   
   *cx = (int)(tx/total);*cy = (int)(ty/total);
   
   return total;
}
/***************************************************************************************
@f_name: void LookGoal(u8 image_label[CAMERA_H][CAMERA_W],int X[5],int Y[5])
@brief:	 计算标记点坐标
@param:	 None
@return: None
****************************************************************************************/
int My_Vertical =0;//纵向扫描
int My_Horizon  =0;//横向扫描

float H_x[2],H_y[2],H_total[2] = {0};
float V_x[2],V_y[2],V_total[2] = {0};
void LookGoal(u8 image_label[CAMERA_H][CAMERA_W],int X[5],int Y[5])
{
	int i_h,j_h,k_h=0;
	int i_v,j_v,k_v=0;
	i_h=0;j_h=0;k_h=0;
	i_v=0;j_v=0;k_v=0;
	H_x[0]=0;H_x[1]=0;V_x[0]=0;V_x[1]=0;H_total[0]=0;H_total[1]=0;
	H_y[0]=0;H_y[1]=0;V_y[0]=0;V_y[1]=0;V_total[0]=0;V_total[1]=0;
	My_Vertical = CAMERA_H;
	My_Horizon  = CAMERA_W;	
	
	//**************横向扫描*************//
	for(i_h=CAMERA_H/2-20;i_h<CAMERA_H/2+20;i_h++)
	{
		for(j_h=0;j_h<My_Horizon/2;j_h++)
		{
			if(image_label[i_h][j_h] == 0)
					{H_x[0]+=j_h;H_y[0]+=i_h;H_total[0]++;}
		}
		for(k_h=My_Horizon-1;k_h>My_Horizon/2;k_h--)
		{
				if(image_label[i_h][k_h] == 0)
					{H_x[1]+=k_h;H_y[1]+=i_h;H_total[1]++;}
		}

	}	 
	if(H_total[0] == 0) 
	{ 
		X[0] = 0;
		Y[0]=100;
	}//右移出界
	else 
	{
		X[0]= (H_x[0]/H_total[0]);
		Y[0]= (H_y[0]/H_total[0]);
	}
		
	if(H_total[1] == 0) 
	{  
		X[1] = 200;
		Y[1] = 100;
	}//左移出界
	else
	{
		X[1]= (H_x[1]/H_total[1]);
		Y[1]= (H_y[1]/H_total[1]);
	}

	
		//**************纵向扫描*************//
	for(i_v=CAMERA_W/2-20;i_v<CAMERA_W/2+20;i_v++)
	{
		for(j_v=0;j_v<My_Vertical/2;j_v++)
		{
			if(image_label[i_v][j_v] == 0)
					{V_x[0]+=i_v;V_y[0]+=j_v;V_total[0]++;}
		}
		for(k_v=My_Vertical-1;k_v>My_Vertical/2;k_v--)
		{
				if(image_label[i_v][k_v] == 0)
					{V_x[1]+=i_v;V_y[1]+=k_v;V_total[1]++;}
		}
	}	 
		if(V_total[0] == 0)
	 {  
		 X[2] = 100;
		 Y[2] = 0;
	 }
	else 
		{
			X[2] =(V_x[0]/V_total[0]);
			Y[2] =(V_y[0]/V_total[0]);
		}
	if(V_total[1] == 0) 
		{  
			X[3] = 100;
			Y[3] = 200;
		}
	else 
		{
			X[3] = (V_x[1]/V_total[1]);
			Y[3] = (V_y[1]/V_total[1]);
		}
}
/***************************************************************************************
@f_name: void PixToDis(int X[5],int Y[5])
@brief:	 坐标映射到物理世界距离
@param:	 None
@return: None
****************************************************************************************/
//左标记，右标记，上标记，下标记，激光点
//int GolaCentra_X[5] = {0,0,0,0,0};
//int GolaCentra_Y[5] = {0,0,0,0,0};
int RightMaxPix =185;
int LeftMaxPix =12;
int UPMaxPix =182;
int DownMaxPix =22;
int MYDIStest=0;
void PixToDis(int X[5],int Y[5])
{
	LeftMaxPix = X[0];
	RightMaxPix = X[1];
	UPMaxPix = Y[3];//上标记对应的像素值最大，下标记对应的像素值最小
	DownMaxPix = Y[2];
	if(X[4]>=(LeftMaxPix) && X[4]<=(RightMaxPix) && Y[4]>=(DownMaxPix) && Y[4]<=(UPMaxPix))
	{
		DealData[0]=(float)(X[4]-(200-X[1]))*((float)60/(RightMaxPix-LeftMaxPix));
		
		
		DealData[1]=(float)(Y[4]-(200-Y[3]))*((float)60/(UPMaxPix-DownMaxPix));
	}
	else //脱靶时为100,在主机上加入圆轨迹限制，超出圆轨迹限制也视为脱靶
	{
		DealData[0] = 100;
		DealData[1] = 100;
	}
}


