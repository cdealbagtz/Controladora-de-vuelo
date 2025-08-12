/*
 * rates_mode.c
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/rates_mode.h"

Cmd_s rates_control_law(Cmd_s inputs_norms, Gyro_s actual_gyro, Cmd_s K)
{
	//
	Cmd_s out_puts;

	out_puts.roll   = inputs_norms.roll  -  actual_gyro.roll * K.roll ;
	out_puts.pitch  = inputs_norms.pitch -  actual_gyro.pitch* K.pitch ;
	out_puts.yaw    = inputs_norms.yaw   -  actual_gyro.yaw  * K.yaw ;
	out_puts.thrust = inputs_norms.thrust ;

	return out_puts ;
}
