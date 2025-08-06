/*
 * parameters_manager.c
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/parameters_manager.h"
#include "Flight_Management_Control/flight_management.h"


void attitude_parameters_refresh(void)
{
	//
	rates_Gains.roll  = K_P * 100;
	rates_Gains.pitch = K_Q * 100;
	rates_Gains.yaw   = K_R * 100;

}


