#ifndef _COMMON_H_
#define _COMMON_H_
#include "include.h"
#include "stm32f4xx_conf.h"
#include "stdio.h"
#define EnableInterrupts   __enable_irq()     //�����ж�
#define DisableInterrupts  __disable_irq()    //�����ж�

/*****************************������������*****************************/
typedef 	unsigned 	char		u8;	  //�޷�����
typedef     u8                      Byte;
typedef     u8                      bool;
typedef 	unsigned 	short int	u16;
//typedef 	unsigned 	long  int	u32; 

typedef 	signed char			    s8;	  //�з�����
typedef 	short  int				s16;
//typedef 	long   int				s32;
/*****************************��C#����*********************************/
typedef     u8                      byte;
typedef     s16                     Int16;
typedef     double                  Double;
typedef     u16                     UInt16;
typedef     u32                     uint;
typedef     u32                     UInt32;
typedef     s32                     Int32;
/**********************************************************************/

#define ERROR       0
#define SUCCESS     1

#define true        1      //
#define false       0      //
#define none        2      // 




#endif
