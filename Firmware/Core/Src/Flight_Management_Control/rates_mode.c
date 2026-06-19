/*
 * rates_mode.c
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/rates_mode.h"

Gyro_s Saturation = {
		.pitch = 10,
		.roll = 10,
		.yaw = 0
};

Cmd_s rates_control_law(Cmd_s inputs_norms, Gyro_s actual_gyro, Cmd_s K)
{
	//
	Cmd_s Outs;
	Gyro_s Gyro_Error;
	Gyro_s Gyro_SetPoint;

	Gyro_SetPoint.roll = inputs_norms.roll * Saturation.roll;
	Gyro_SetPoint.pitch = inputs_norms.pitch * Saturation.pitch;
	Gyro_SetPoint.yaw = 0; //SetPoint siempre en 0 para Damping

	Gyro_Error.roll  = Gyro_SetPoint.roll  - actual_gyro.roll;
	Gyro_Error.pitch = Gyro_SetPoint.pitch - actual_gyro.pitch;
	Gyro_Error.yaw   = Gyro_SetPoint.yaw   - actual_gyro.yaw;

	Outs.roll   = inputs_norms.roll  -  Gyro_Error.roll * K.roll ;
	Outs.pitch  = inputs_norms.pitch -  Gyro_Error.pitch* K.pitch ;
	Outs.yaw    = inputs_norms.yaw   -  Gyro_Error.yaw  * K.yaw ;
	Outs.thrust = inputs_norms.thrust ;

	return Outs ;
}
