/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * 作者   ：匿名科创
 * 文件名  ：imu.c
 * 描述    ：姿态解算
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/

#include "imu_2.h"

#define Kp_2 0.6f                	// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki_2 0.1f                	// 0.001  integral gain governs rate of convergence of gyroscope biases
#define IMU_INTEGRAL_LIM_2  ( 2.0f *ANGLE_TO_RADIAN )
#define NORM_ACC_LPF_HZ_2 10  		//(Hz)
#define REF_ERR_LPF_HZ_2  1				//(Hz)
xyz_f_t reference_v_2;
ref_t_2 	ref_2;

xyz_f_t Gravity_Vec_2;  				//解算的重力向量
	
float SOFT_Roll_2,SOFT_Pitch_2,SOFT_Yaw_2;    				//姿态角

float ref_q_2[4] = {1.0f,0.0f,0.0f,0.0f};
float norm_acc_2,norm_q_2;
float norm_acc_lpf_2;

void  IMUupdate_2(float half_T_2,float gx_2, float gy_2, float gz_2, float ax_2, float ay_2, float az_2)//float *rol_,float *pit_,float *yaw_) 
{	
	
	float ref_err_lpf_hz;
	
	//=============================================================================
	// 计算等效重力向量
	reference_v_2.x = 2*(ref_q_2[1]*ref_q_2[3] - ref_q_2[0]*ref_q_2[2]);
	reference_v_2.y = 2*(ref_q_2[0]*ref_q_2[1] + ref_q_2[2]*ref_q_2[3]);
	reference_v_2.z = 1 - 2*(ref_q_2[1]*ref_q_2[1] + ref_q_2[2]*ref_q_2[2]);//ref_q[0]*ref_q[0] - ref_q[1]*ref_q[1] - ref_q[2]*ref_q[2] + ref_q[3]*ref_q[3];

	
	//这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
	//根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
	//所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。       
	//=============================================================================


	// 计算加速度向量的模
	norm_acc_2 = my_sqrt(ax_2*ax_2 + ay_2*ay_2 + az_2*az_2);   
	norm_acc_lpf_2 +=  NORM_ACC_LPF_HZ_2 *(6.28f *half_T_2) *(norm_acc_2 - norm_acc_lpf_2);  //10hz *3.14 * 2*0.001


	if(ABS(ax_2)<4400 && ABS(ay_2)<4400 && ABS(az_2)<4400 )
	{	
		//把加计的三维向量转成单位向量。
		ax_2 = ax_2 / norm_acc_2;//4096.0f;
		ay_2 = ay_2/ norm_acc_2;//4096.0f;
		az_2 = az_2 / norm_acc_2;//4096.0f; 
		
		if( 3800 < norm_acc_2 && norm_acc_2 < 4400 )
		{
			/* 叉乘得到误差 */
			ref_2.err_tmp.x = ay_2*reference_v_2.z - az_2*reference_v_2.y;
			ref_2.err_tmp.y = az_2*reference_v_2.x - ax_2*reference_v_2.z;
	//       ref.err_tmp.z  = asin(ax*reference_v.y - ay*reference_v.x)/1.57f;
			/* 误差低通 */
			ref_err_lpf_hz = REF_ERR_LPF_HZ_2 *(6.28f *half_T_2);
			ref_2.err_lpf.x += ref_err_lpf_hz *( ref_2.err_tmp.x  - ref_2.err_lpf.x );
			ref_2.err_lpf.y += ref_err_lpf_hz *( ref_2.err_tmp.y  - ref_2.err_lpf.y );
	//			 ref.err_lpf.z += ref_err_lpf_hz *( ref.err_tmp.z  - ref.err_lpf.z );
			
			ref_2.err.x = ref_2.err_lpf.x;//
			ref_2.err.y = ref_2.err_lpf.y;//
//				ref.err.z = ref.err_lpf.z ;
		}
	}
	else
	{
		ref_2.err.x = 0; 
		ref_2.err.y = 0  ;
//		ref.err.z = 0 ;
	}
	/* 误差积分 */
	ref_2.err_Int.x += ref_2.err.x *Ki_2 *2 *half_T_2 ;
	ref_2.err_Int.y += ref_2.err.y *Ki_2 *2 *half_T_2 ;
	ref_2.err_Int.z += ref_2.err.z *Ki_2 *2 *half_T_2 ;
	
	/* 积分限幅 */
	ref_2.err_Int.x = LIMIT(ref_2.err_Int.x, - IMU_INTEGRAL_LIM_2 ,IMU_INTEGRAL_LIM_2 );
	ref_2.err_Int.y = LIMIT(ref_2.err_Int.y, - IMU_INTEGRAL_LIM_2 ,IMU_INTEGRAL_LIM_2);
	ref_2.err_Int.z = LIMIT(ref_2.err_Int.z, - IMU_INTEGRAL_LIM_2 ,IMU_INTEGRAL_LIM_2);

	
	ref_2.g.x = gx_2*ANGLE_TO_RADIAN + ( Kp_2*(ref_2.err.x + ref_2.err_Int.x) ) ;     //IN RADIAN
	ref_2.g.y = gy_2*ANGLE_TO_RADIAN + ( Kp_2*(ref_2.err.y + ref_2.err_Int.y) ) ;		  //IN RADIAN
	ref_2.g.z = gz_2*ANGLE_TO_RADIAN ;//+ ( Kp*(ref.err.z + ref.err_Int.z) )  ;   //IN RADIAN    

	/* 用叉积误差来做PI修正陀螺零偏 */

	// integrate quaternion rate and normalise
	ref_q_2[0] = ref_q_2[0] +(-ref_q_2[1]*ref_2.g.x - ref_q_2[2]*ref_2.g.y - ref_q_2[3]*ref_2.g.z)*half_T_2;
	ref_q_2[1] = ref_q_2[1] + (ref_q_2[0]*ref_2.g.x + ref_q_2[2]*ref_2.g.z - ref_q_2[3]*ref_2.g.y)*half_T_2;
	ref_q_2[2] = ref_q_2[2] + (ref_q_2[0]*ref_2.g.y - ref_q_2[1]*ref_2.g.z + ref_q_2[3]*ref_2.g.x)*half_T_2;
	ref_q_2[3] = ref_q_2[3] + (ref_q_2[0]*ref_2.g.z + ref_q_2[1]*ref_2.g.y - ref_q_2[2]*ref_2.g.x)*half_T_2;  

	/* 四元数规一化 normalise quaternion */
	norm_q_2 = my_sqrt(ref_q_2[0]*ref_q_2[0] + ref_q_2[1]*ref_q_2[1] + ref_q_2[2]*ref_q_2[2] + ref_q_2[3]*ref_q_2[3]);
	ref_q_2[0] = ref_q_2[0] / norm_q_2;
	ref_q_2[1] = ref_q_2[1] / norm_q_2;
	ref_q_2[2] = ref_q_2[2] / norm_q_2;
	ref_q_2[3] = ref_q_2[3] / norm_q_2;
	

	SOFT_Roll_2 = fast_atan2(2*(ref_q_2[0]*ref_q_2[1] + ref_q_2[2]*ref_q_2[3]),1 - 2*(ref_q_2[1]*ref_q_2[1] + ref_q_2[2]*ref_q_2[2])) *57.3f;
	SOFT_Pitch_2 = asin(2*(ref_q_2[1]*ref_q_2[3] - ref_q_2[0]*ref_q_2[2])) *57.3f;
// 				//Yaw   = ( - fast_atan2(2*(ref_q[1]*ref_q[2] + ref_q[0]*ref_q[3]),ref_q[0]*ref_q[0] + ref_q[1]*ref_q[1] - ref_q[2]*ref_q[2] - ref_q[3]*ref_q[3]) )* 57.3;
	SOFT_Yaw_2 = fast_atan2(2*(ref_q_2[1]*ref_q_2[2] - ref_q_2[0]*ref_q_2[3]), 2*(ref_q_2[0]*ref_q_2[0] + ref_q_2[1]*ref_q_2[1]) - 1) *57.3f;


}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

