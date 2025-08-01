/*
 * flight_management.c
 *
 *  Created on: Jul 24, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/flight_management.h"

FlightMode_t mode = 0;
Cmd_s Command_in;
Cmd_s Command_out;

void FlightTaskAttitude(void)
{
	//
	mode = (FlightMode_t)CurrentMode();

	switch(mode)
	{
	case FBW_MODE:
	case ATTITUDE_HOLD_MODE:
//		AttitudeHold();
		break;
	case RATE_MODE:
		//
		//Command_out =
		break;
	case MANUAL_MODE:
		//
		Command_out = Command_in;
		break;
	default:
		//
		break;
	}

}

void AttitudeHold(void)
{
	//
//	AttitudeActuals();
//	AttitudeSetPoint();


}
