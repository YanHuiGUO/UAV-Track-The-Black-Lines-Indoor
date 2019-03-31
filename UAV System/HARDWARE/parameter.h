#ifndef _PARAMETER_H
#define	_PARAMETER_H

#include "stm32f4xx.h"

typedef struct
{
	float kp;
	float kd;
	float ki;
	float kdamp;

}pid_t;

typedef struct 
{
  float x;
	float y;
	float z;
}xyz_f_t;

typedef struct 
{
  s16 x;
	s16 y;
	s16 z;

}xyz_s16_t;


typedef union
{
	uint8_t raw_data[64];
	struct{
  xyz_f_t Accel;
  xyz_f_t Gyro;
  xyz_f_t Mag;
  uint32_t Baro;
	float Temperature;
	}Offset;
}sensor_setup_t;

typedef  struct{
pid_t roll;
pid_t pitch;	
pid_t yaw;	
 }pid_group_t;

typedef union
{
 uint8_t raw_data[128];
 struct{
 pid_group_t ctrl1;
 pid_group_t ctrl2;
 }groups;
}pid_setup_t;

extern xyz_f_t except_A;
void Para_ResetToFactorySetup(void);
void Para_Init(void);


#endif

