/*
 * sensors.c
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/sensors.h"
#include "Flight_Management_Control/flight_parameters.h"

LPF_s gyros_noise[3];
LPF_s gyros_fltrs[3];
float LPF_gyros[3];
float imu_gyr[3];

Gyro_s get_actual_rates(void)
{
	//
	Gyro_s gyros;

	gyros.roll  = IMU.GYR.x ;
	gyros.pitch = IMU.GYR.y ;
	gyros.yaw   = IMU.GYR.z ;

	return gyros;
}

Acc_s get_actual_acc(void)
{
	//
	Acc_s acc;

	acc.X = IMU.ACC.x ;
	acc.Y = IMU.ACC.y ;
	acc.Z = IMU.ACC.z ;

	return acc;
}



Gyro_s actual_rates;

void LPF_gyro_filter_init(void)
{
	// Inicializaciones
	LPF_gyros[0] = LPF_gyro_x ;
	LPF_gyros[1] = LPF_gyro_y ;
	LPF_gyros[2] = LPF_gyro_z ;
	for(int idx = 0;idx < 3; idx++)
	{
		//
		gyros_fltrs[idx].inicio 	= 1;
		gyros_fltrs[idx].f_cutoff 	= LPF_gyros[idx] ;
		gyros_fltrs[idx].t_sample 	= SAMPLE_ATT ;
		gyros_fltrs[idx].Y_0 		= 0.0f ;
		gyros_fltrs[idx].U_n 		= 0.0f;
		gyros_fltrs[idx].Y_n 		= 0.0f;
		gyros_fltrs[idx].Y_nm1 		= 0.0f;
		gyros_fltrs[idx].U_nm1 		= 0.0f;
		gyros_fltrs[idx].t_n 		= 0.0f;
		gyros_fltrs[idx].t_nm1 		= 0.0f;

	}

}

void refresh_actual_rates(void)
{
	//
	imu_gyr[0] 		= IMU.GYR.x ;
	imu_gyr[1] 		= IMU.GYR.y ;
	imu_gyr[2] 		= IMU.GYR.z ;

	//
	for(int idx = 0; idx < 3; idx++)
	{
		//
		gyros_fltrs[idx].U_n = imu_gyr[idx];

		gyros_fltrs[idx] = filtering_lpf(&gyros_fltrs[idx]);
	}

	actual_rates.roll  = gyros_fltrs[0].Y_n ;
	actual_rates.pitch = gyros_fltrs[1].Y_n ;
	actual_rates.yaw   = gyros_fltrs[2].Y_n ;
}
