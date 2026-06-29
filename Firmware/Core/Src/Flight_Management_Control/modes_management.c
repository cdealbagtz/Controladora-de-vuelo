/*
 * modes_management.c
 *
 *  Created on: Jul 25, 2025
 *      Author: rtrujillo
 */


#include "Flight_Management_Control/modes_management.h"
#include "Flight_Management_Control/flight_parameters.h"

FlightMode_t mode = MANUAL_MODE;


FlightMode_t ModesManagement_ReadSwitch3Pos(void)
{
	//
	uint16_t pwm_value = FLIGHT_MODE_PWM_INPUT();

	static FlightMode_t last_mode = MANUAL_MODE;

	if (Radio_input.fail_safe == FailSafe)
	    {
	        last_mode = FLIGHT_MODE_FAILSAFE;
	        return last_mode;
	    }


	if (pwm_value < (PWM_MID - PWM_DEADZONE))

	{
		last_mode = FLIGHT_MODE_SW_LOW;
	 }
	else if ((pwm_value >= (PWM_MID - PWM_DEADZONE))&&(pwm_value <= (PWM_MID + PWM_DEADZONE)))
	 {
		last_mode = FLIGHT_MODE_SW_MID;
	  }
	else if (pwm_value > (PWM_MID + PWM_DEADZONE))
	 {
		last_mode = FLIGHT_MODE_SW_HIGH;
	  }
	else
	{
	        // Zona muerta → mantener último modo (evita oscilaciones)
			// Se mantiene el ultimo modo valido

	 }

	return last_mode;
}

void get_flight_mode(void)
{
	//
	mode = ModesManagement_ReadSwitch3Pos();

	switch (mode) {
		case MANUAL_MODE:
			LED_Info.B_LED1.LED_status = 1;
			LED_Info.B_LED2.LED_status = 0;
			LED_Info.B_LED3.LED_status = 0;
			break;
		case RATE_MODE:
			LED_Info.B_LED1.LED_status = 0;
			LED_Info.B_LED2.LED_status = 1;
			LED_Info.B_LED3.LED_status = 0;
			break;
		case ATTITUDE_HOLD_MODE:
			LED_Info.B_LED1.LED_status = 0;
			LED_Info.B_LED2.LED_status = 0;
			LED_Info.B_LED3.LED_status = 1;
			break;

		case FBW_MODE:
			LED_Info.B_LED1.LED_status = 1;
			LED_Info.B_LED2.LED_status = 1;
			LED_Info.B_LED3.LED_status = 0;
			break;
		case FAILSAFE_MODE:
			LED_Info.B_LED1.LED_status = 1;
			LED_Info.B_LED2.LED_status = 1;
			LED_Info.B_LED3.LED_status = 1;
			break;
		default:
			LED_Info.B_LED1.LED_status = 0;
			LED_Info.B_LED2.LED_status = 0;
			LED_Info.B_LED3.LED_status = 0;
			break;
	}
}

