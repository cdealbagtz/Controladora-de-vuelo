/*
 * flight_management.c
 *
 *  Created on: Jul 24, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/flight_management.h"
#include "Flight_Management_Control/parameters_manager.h"

Cmd_s Command_in;

Cmd_s rates_Gains;



void FlightTaskAttitude(void)
{
	//
	attitude_parameters_refresh();

	refresh_actual_rates();

	get_flight_mode();

	Command_in = get_commands_rc();


	switch(mode)
	{
	case FBW_MODE:
	case ATTITUDE_HOLD_MODE:
//		AttitudeHold();
		break;
	case RATE_MODE:
		//
		Command_out = rates_control_law(Command_in , actual_rates, rates_Gains);
		break;
	case MANUAL_MODE:
		//
		Command_out = Command_in;
		break;
	default:
		//
		Command_out = Command_in;
		break;
	}

	command_filtering();

	get_actual_trims();

	init_Reverse_Servos(&Reverse);

	control_allocator(Commands , Trims );

}


void AttitudeHold(void)
{
	//
//	AttitudeActuals();
//	AttitudeSetPoint();


}
