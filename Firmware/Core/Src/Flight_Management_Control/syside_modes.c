/*
 * syside_modes.c
 *
 *  Created on: Jun 27, 2026
 *      Author: rafa
 */


#include "Flight_Management_Control/syside_modes.h"
#include <math.h>  // Necesario para sinf()

// Variable estática para mantener el tiempo entre llamadas
static float t = 0.0f;
static SysideTest_t test ;
// Función para reiniciar el timer externamente
void reset_syside_timer(void)
{
    t = 0.0f;
}

SysideTest_t Get_SystemIdentificationTest_Type(void)
{
	//
	uint16_t pwm_value = FLIGHT_SYSIDE_TEST();
	SysideTest_t  syside_test_type;

	if (pwm_value < (PWM_MID - PWM_DEADZONE))
	{
		syside_test_type = SYSIDE_TEST_SW_LOW;
	 }
	else if ((pwm_value >= (PWM_MID - PWM_DEADZONE))&&(pwm_value <= (PWM_MID + PWM_DEADZONE)))
	 {
		syside_test_type = SYSIDE_TEST_SW_MID;
	  }
	else if (pwm_value > (PWM_MID + PWM_DEADZONE))
	 {
		syside_test_type = SYSIDE_TEST_SW_HIGH;
	  }
	else
	{
		//
		syside_test_type = 0;
	}

	return syside_test_type;
}

Cmd_s sine_wave_axis(int8_t axis,float time_start, float time_test, Cmd_s inputs_norms, float freq_Hz, float Amplitude, float Period_t)
{
	//
	Cmd_s commands_out = {0.0f, 0.0f, 0.0f, 0.0f};

	commands_out = inputs_norms;

	// Incrementar el tiempo por el período
	t += Period_t;

	float value = 0.0f;  // Inicializar por defecto

	if(t >= time_start && t < time_test)
	{
	    value = Amplitude * sinf(2.0f * M_PI * freq_Hz * (t - time_start));
	}

	switch(axis)
	{
	case 0:
		commands_out.roll = value;
		break;
	case 1:
		commands_out.pitch = value;
		break;
	case 2:
		commands_out.yaw = value;
		break;
	case 3:
		commands_out.thrust = value;
		break;
	default:
		break;
	}

	return commands_out;
}

Cmd_s System_Identification_Test(int8_t axis, Cmd_s inputs_norms, float sample_time)
{
	//
	Cmd_s commands_out = {0.0f, 0.0f, 0.0f, 0.0f};

	commands_out = inputs_norms;

	// Incrementar el tiempo por el período
	t += sample_time;

	float value = 0.0f;

	test = Get_SystemIdentificationTest_Type();

	switch(test)
	{
	case SINE_MODE:

		if(t >= Time_start && t < Time_test)
		{
			value = AMP_syside_wave * sinf(2.0f * M_PI * Freq_wave_Hertz * (t - Time_start));
		}
		break;
	case PULSE_MODE:
		if(t >= Time_start && t < (Time_start + Time_pulse_seg))
		{
			value = AMP_syside_pulse;
		}
		break;
	case DOUBLET_MODE:
		if(t >= Time_start && t < (Time_start + Time_doublet_seg ) )
		{
			value = AMP_syside_pulse;
		}
		if(t >= (Time_start + Time_doublet_seg ) && t < (Time_start + 2 * Time_doublet_seg))
		{
			value = - AMP_syside_pulse;
		}
		break;
	case CHIRP_MODE:
		if(t >= Time_start && t < Time_test)
		{
			value = AMP_syside_wave * sinf(2.0f * M_PI * Freq_wave_Hertz * (t - Time_start));
		}
		break;
	default:
		value = 0.0f;
		break;
	}

	switch(axis)
		{
		case 0:
			commands_out.roll = value + commands_out.roll;
			break;
		case 1:
			commands_out.pitch = value + commands_out.pitch;
			break;
		case 2:
			commands_out.yaw = value + commands_out.yaw ;
			break;
		case 3:
			commands_out.thrust = value + commands_out.thrust ;
			break;
		default:
			break;
		}

	return commands_out;
}
