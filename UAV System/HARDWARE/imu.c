
#include "imu.h"
#include "include.h"

#define Kp 0.6f                	// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.1f                	// 0.001  integral gain governs rate of convergence of gyroscope biases

#define IMU_INTEGRAL_LIM  ( 2.0f *ANGLE_TO_RADIAN )
#define NORM_ACC_LPF_HZ 10  		//(Hz)
#define REF_ERR_LPF_HZ  1				//(Hz)

float A_gCos[3][3]={0};
float b_g[3],c_g[3],d_angle[3],a_final[3];
float Angle_Cos = 0;
xyz_f_t reference_v;
ref_t 	ref;

xyz_f_t Gravity_Vec;  				//�������������
	
float Roll,Pitch,Yaw;    				//��̬��

float ref_q[4] = {1,0,0,0};
float norm_acc,norm_q;
float norm_acc_lpf;

void IMUupdate(float half_T,float gx, float gy, float gz, float ax, float ay, float az,float *rol,float *pit,float *yaw) 
{	
	int i,j =0;
	float ref_err_lpf_hz;
	
	//=============================================================================
	// �����Ч��������
	reference_v.x = 2*(ref_q[1]*ref_q[3] - ref_q[0]*ref_q[2]);
	reference_v.y = 2*(ref_q[0]*ref_q[1] + ref_q[2]*ref_q[3]);
	reference_v.z = 1 - 2*(ref_q[1]*ref_q[1] + ref_q[2]*ref_q[2]);//ref_q[0]*ref_q[0] - ref_q[1]*ref_q[1] - ref_q[2]*ref_q[2] + ref_q[3]*ref_q[3];

	//���Ҿ���
	 A_gCos[0][0]=ref_q[0]*ref_q[0]+ref_q[1]*ref_q[1]-ref_q[2]*ref_q[2]-ref_q[3]*ref_q[3];
	 A_gCos[0][1]=2*(ref_q[1]*ref_q[2]-ref_q[0]*ref_q[3]);
	 A_gCos[0][2]=2*(ref_q[0]*ref_q[2]+ref_q[1]*ref_q[3]);
	 A_gCos[1][0]=2*(ref_q[1]*ref_q[2]+ref_q[0]*ref_q[3]);
	 A_gCos[1][1]=ref_q[0]*ref_q[0]-ref_q[1]*ref_q[1]+ref_q[2]*ref_q[2]-ref_q[3]*ref_q[3];
	 A_gCos[1][2]=2*(ref_q[2]*ref_q[3]-ref_q[0]*ref_q[1]);
	 A_gCos[2][0]=reference_v.x;
	 A_gCos[2][1]=reference_v.y;
	 A_gCos[2][2]=reference_v.z;	
	//���ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
	//�������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
	//���������vx\y\z����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������������λ������       
	//=============================================================================


	// ������ٶ�������ģ
	norm_acc = my_sqrt(ax*ax + ay*ay + az*az);   
	norm_acc_lpf +=  NORM_ACC_LPF_HZ *(6.28f *half_T) *(norm_acc - norm_acc_lpf);  //10hz *3.14 * 2*0.001


	if(ABS(ax)<4400 && ABS(ay)<4400 && ABS(az)<4400 )
	{	
		//�ѼӼƵ���ά����ת�ɵ�λ������
		ax = ax / norm_acc;//4096.0f;
		ay = ay / norm_acc;//4096.0f;
		az = az / norm_acc;//4096.0f; 
		
		if( 3800 < norm_acc && norm_acc < 4400 )
		{
			/* ��˵õ���� */
			ref.err_tmp.x = ay*reference_v.z - az*reference_v.y;
			ref.err_tmp.y = az*reference_v.x - ax*reference_v.z;
	//       ref.err_tmp.z  = asin(ax*reference_v.y - ay*reference_v.x)/1.57f;
			/* ����ͨ */
			ref_err_lpf_hz = REF_ERR_LPF_HZ *(6.28f *half_T);
			ref.err_lpf.x += ref_err_lpf_hz *( ref.err_tmp.x  - ref.err_lpf.x );
			ref.err_lpf.y += ref_err_lpf_hz *( ref.err_tmp.y  - ref.err_lpf.y );
	//			 ref.err_lpf.z += ref_err_lpf_hz *( ref.err_tmp.z  - ref.err_lpf.z );
			
			ref.err.x = ref.err_lpf.x;//
			ref.err.y = ref.err_lpf.y;//
//				ref.err.z = ref.err_lpf.z ;
		}
	}
	else
	{
		ref.err.x = 0; 
		ref.err.y = 0  ;
//		ref.err.z = 0 ;
	}
	/* ������ */
	ref.err_Int.x += ref.err.x *Ki *2 *half_T ;
	ref.err_Int.y += ref.err.y *Ki *2 *half_T ;
	ref.err_Int.z += ref.err.z *Ki *2 *half_T ;
	
	/* �����޷� */
	ref.err_Int.x = LIMIT(ref.err_Int.x, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	ref.err_Int.y = LIMIT(ref.err_Int.y, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	ref.err_Int.z = LIMIT(ref.err_Int.z, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );

	
	ref.g.x = gx*ANGLE_TO_RADIAN + ( Kp*(ref.err.x + ref.err_Int.x) ) ;     //IN RADIAN
	ref.g.y = gy*ANGLE_TO_RADIAN + ( Kp*(ref.err.y + ref.err_Int.y) ) ;		  //IN RADIAN
	ref.g.z = gz*ANGLE_TO_RADIAN ;//+ ( Kp*(ref.err.z + ref.err_Int.z) )  ;   //IN RADIAN    

	/* �ò���������PI����������ƫ */

	// integrate quaternion rate and normalise
	ref_q[0] = ref_q[0] +(-ref_q[1]*ref.g.x - ref_q[2]*ref.g.y - ref_q[3]*ref.g.z)*half_T;
	ref_q[1] = ref_q[1] + (ref_q[0]*ref.g.x + ref_q[2]*ref.g.z - ref_q[3]*ref.g.y)*half_T;
	ref_q[2] = ref_q[2] + (ref_q[0]*ref.g.y - ref_q[1]*ref.g.z + ref_q[3]*ref.g.x)*half_T;
	ref_q[3] = ref_q[3] + (ref_q[0]*ref.g.z + ref_q[1]*ref.g.y - ref_q[2]*ref.g.x)*half_T;  

	/* ��Ԫ����һ�� normalise quaternion */
	norm_q = my_sqrt(ref_q[0]*ref_q[0] + ref_q[1]*ref_q[1] + ref_q[2]*ref_q[2] + ref_q[3]*ref_q[3]);
	ref_q[0] = ref_q[0] / norm_q;
	ref_q[1] = ref_q[1] / norm_q;
	ref_q[2] = ref_q[2] / norm_q;
	ref_q[3] = ref_q[3] / norm_q;
	

	*rol = fast_atan2(2*(ref_q[0]*ref_q[1] + ref_q[2]*ref_q[3]),1 - 2*(ref_q[1]*ref_q[1] + ref_q[2]*ref_q[2])) *57.3f;
	*pit = asin(2*(ref_q[1]*ref_q[3] - ref_q[0]*ref_q[2])) *57.3f;
// 				//Yaw   = ( - fast_atan2(2*(ref_q[1]*ref_q[2] + ref_q[0]*ref_q[3]),ref_q[0]*ref_q[0] + ref_q[1]*ref_q[1] - ref_q[2]*ref_q[2] - ref_q[3]*ref_q[3]) )* 57.3;
	*yaw = fast_atan2(2*(ref_q[1]*ref_q[2] - ref_q[0]*ref_q[3]), 2*(ref_q[0]*ref_q[0] + ref_q[1]*ref_q[1]) - 1) *57.3f;

		 b_g[0]=ax*norm_acc;                           //ת�������������ٶ�  c_g[3]
	   b_g[1]=ay*norm_acc;
	   b_g[2]=az*norm_acc;
	   c_g[0]=0;
	   c_g[1]=0;
	   c_g[2]=0;
      
	  for(i=0;i<3;i++)
	  {
	     for(j=0;j<3;j++)
		  {
		    c_g[i]=c_g[i]+A_gCos[i][j]*b_g[j];

		  }
	  }

	   d_angle[0]=0;
	   d_angle[1]=0;
	   d_angle[2]=0;
      
	  for(i=0;i<3;i++)
	  {
	     for(j=0;j<3;j++)
		  {
		    d_angle[i]=d_angle[i]+A_gCos[i][j];
		  
		  }
	  }
	  
	  Angle_Cos= -(atan2((double)d_angle[1],(double)d_angle[0]) * (180 / 3.14159265)+180)+270;  //����������н�
	  Angle_Cos=(Angle_Cos*3.14159265/180);
	  a_final[0]=c_g[0]*cos(Angle_Cos)-c_g[1]*sin(Angle_Cos);
	  a_final[1]=c_g[0]*sin(Angle_Cos)+c_g[1]*cos(Angle_Cos);
	  a_final[2]=c_g[2];
}

