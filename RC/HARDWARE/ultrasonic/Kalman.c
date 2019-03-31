#include "math.h"
double KalmanGain;         //����������
double EstimateCovariance;  //����Э����
double MeasureCovariance;  //����Э����
double EstimateValue;       //����ֵ

//KalmanGain;         //����������
//EstimateCovariance;  //����Э����
//MeasureCovariance;  //����Э����
//EstimateValue;       //����ֵ
//void KalmanFilterInit( void );
//extern float KalmanFilter( float Measure );

void KalmanFilterInit( void )
{
  EstimateValue = 0;
EstimateCovariance = 0.1;
MeasureCovariance = 0.02;
}

float KalmanFilter( float Measure )
{
if(!EstimateCovariance && !MeasureCovariance)
	KalmanFilterInit();
//���㿨��������
KalmanGain = EstimateCovariance * sqrt( 1 / ( EstimateCovariance * EstimateCovariance + MeasureCovariance * MeasureCovariance ));
//���㱾���˲�����ֵ
EstimateValue = EstimateValue + KalmanGain*( Measure - EstimateValue );
//���¹���Э����
EstimateCovariance = sqrt(1 - KalmanGain) * EstimateCovariance;
//���²�������
MeasureCovariance = sqrt(1 - KalmanGain) * MeasureCovariance;
//���ع���ֵ
return EstimateValue;
}
