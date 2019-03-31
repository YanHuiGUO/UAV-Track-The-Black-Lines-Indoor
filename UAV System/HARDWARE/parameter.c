#include "include.h"

static sensor_setup_t sensor_setup;
static pid_setup_t pid_setup;


void Para_ResetToFactorySetup(void)
{
	/* 加速计默认校准值 */
	sensor_setup.Offset.Accel.x = 0;
	sensor_setup.Offset.Accel.y = 0;
	sensor_setup.Offset.Accel.z = 0;
	/* 陀螺仪默认校准值 */
	sensor_setup.Offset.Gyro.x = 0;
	sensor_setup.Offset.Gyro.y = 0;
	sensor_setup.Offset.Gyro.z = 0;
	/* 罗盘默认校准值 */
	sensor_setup.Offset.Mag.x = 0;		
	sensor_setup.Offset.Mag.y = 0;		
	sensor_setup.Offset.Mag.z = 0;	
	/* 气压计默认校准值 */	
	sensor_setup.Offset.Baro = 0;
   /* 温度默认校准值 */	
	sensor_setup.Offset.Temperature = 0;
	
  /* PID 默认值 */
	pid_setup.groups.ctrl1.pitch.kp = 1.0;
	pid_setup.groups.ctrl1.pitch.ki = 0.1;
	pid_setup.groups.ctrl1.pitch.kd = 1.5;
	pid_setup.groups.ctrl1.pitch.kdamp = 1;
	
	pid_setup.groups.ctrl1.roll.kp = 1.0;
	pid_setup.groups.ctrl1.roll.ki = 0.1;
	pid_setup.groups.ctrl1.roll.kd = 1.5;
	pid_setup.groups.ctrl1.roll.kdamp = 1;
	
	pid_setup.groups.ctrl1.yaw.kp = 1.0;
	pid_setup.groups.ctrl1.yaw.ki = 0.1;
	pid_setup.groups.ctrl1.yaw.kd = 1.5;
	pid_setup.groups.ctrl1.yaw.kdamp = 1;

  pid_setup.groups.ctrl2.pitch.kp = 0.8;
	pid_setup.groups.ctrl2.pitch.ki = 0.05;
	pid_setup.groups.ctrl2.pitch.kd = 0.3;

  pid_setup.groups.ctrl2.roll.kp = 0.8;
	pid_setup.groups.ctrl2.roll.ki = 0.05;
	pid_setup.groups.ctrl2.roll.kd = 0.3;
	
	pid_setup.groups.ctrl2.yaw.kp = 0.8;
	pid_setup.groups.ctrl2.yaw.ki = 0.05;
	pid_setup.groups.ctrl2.yaw.kd = 0.3;

}

void Para_Init()
{
	 Para_ResetToFactorySetup();
}
