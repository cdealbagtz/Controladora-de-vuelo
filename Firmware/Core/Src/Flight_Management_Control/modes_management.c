/*
 * modes_management.c
 *
 *  Created on: Jul 25, 2025
 *      Author: rtrujillo
 */


#include "Flight_Management_Control/modes_management.h"

FlightMode_t mode ;

uint8_t rc_three_steps_CurrentMode(void)
{
	//
	uint16_t pwm_value = Radio_input.Canal_5;

	if (pwm_value < (PWM_MID - PWM_DEADZONE))
	{
	        return MANUAL_MODE;
	 }
	else if (pwm_value < (PWM_MID + PWM_DEADZONE) && pwm_value > (PWM_MAX - PWM_DEADZONE))
	 {
	        return RATE_MODE;
	  }
	else if (pwm_value >= (PWM_MAX - PWM_DEADZONE))
	 {
	        return ATTITUDE_HOLD_MODE;
	  }
	else {
	        // Zona muerta → mantener último modo (evita oscilaciones)
	        static FlightMode_t last_mode = MANUAL_MODE;
	        return last_mode;
	    }

}

void get_flight_mode(void)
{
	//
	mode = (FlightMode_t)rc_three_steps_CurrentMode();
}

