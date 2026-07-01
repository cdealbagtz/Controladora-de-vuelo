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

void Init_syside_clock(void)
{
    bool is_valid_mode = (mode == MANUAL_SYSIDE_MODE ||
                          mode == RATE_SYSIDE_MODE ||
                          mode == ATTITUDE_HOLD_SYSIDE_MODE);

    if(!is_valid_mode)
    {
    	//
        reset_syside_timer();
    }
}

void FlightControl_inits(void)
{
	//
	init_PIDs();
}

void FlightTaskAttitude(void)
{
	//
	refresh_actual_rates();
	get_actual_attitude();
	get_flight_mode();

	Command_in = get_commands_rc();

	switch(mode)
	{
	case FBW_MODE:
	case ATTITUDE_HOLD_MODE:
		//
		Command_out = attitude_hold_control(Command_in, actual_attitude , actual_rates, rates_Gains);
		break;
	case RATE_MODE:
		attitude_parameters_refresh();
		Command_out = rates_control_law(Command_in , actual_rates, rates_Gains) ;
		break;
	case MANUAL_SYSIDE_MODE:
		Command_out = System_Identification_Test(SINE_MODE, IDX_syside, Command_in, SAMPLE_ATT);
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

	Init_syside_clock();

	Command = command_filtering(Command_out, FILTER_ON);
	get_actual_trims();
	init_Reverse_Servos(&Reverse);
	control_allocator(Command, Trims );
}



void FlightTaskARSP(void)
{
	//
	switch(mode)
	{
	//
	case FBW_MODE:
		//
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
