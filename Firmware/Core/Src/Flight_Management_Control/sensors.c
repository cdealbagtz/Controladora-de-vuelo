/*
 * sensors.c
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/sensors.h"
#include "Flight_Management_Control/flight_parameters.h"

float LPF_gyros[3];
float imu_gyr[3];
Gyro_s actual_rates;

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

void refresh_actual_rates(void)
{
	//
	imu_gyr[0] 		= IMU.GYR.x ;
	imu_gyr[1] 		= IMU.GYR.y ;
	imu_gyr[2] 		= IMU.GYR.z ;

	actual_rates.roll  = imu_gyr[0] ;
	actual_rates.pitch = imu_gyr[1] ;
	actual_rates.yaw   = imu_gyr[2] ;
}
