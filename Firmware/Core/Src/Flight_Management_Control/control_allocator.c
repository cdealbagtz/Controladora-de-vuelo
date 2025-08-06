/*
 * control_allocator.c
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/servo_mixers.h"



void control_allocator(Cmd_s control_cmd, Trim_s trims,Servo_reverse_s reverse, uint8_t frame_type)
{
	//
	Servo_mgmt_s servo_outs;


	switch(frame_type){
		case 0:
			servo_outs = FIX_WING_MIXER(control_cmd, trims)     ;
			break;
		case 1:
			servo_outs = FLYING_WING_MIXER(control_cmd, trims)  ;
			break;
		case 2:
			servo_outs = TANDEM_WING_MIXER(control_cmd, trims)  ;
			break;
		case 3:
			servo_outs = CUSTOM_FRAME_MIXER(control_cmd, trims) ;
			break;
		default:

			for(int i = 0; i < 10; i++)
			{
				//
				servo_outs.S[i] = 1500 ;
			}

		}

	PWM_Output = reverse_servos( servo_outs ,reverse);
}

uint16_t reverse_servo_value(uint16_t input)
{
    // Asegurarnos que el valor esté dentro del rango válido
    if (input < 1000) input = 1000;
    if (input > 2000) input = 2000;

    // Calcular el valor invertido
    return 3000 - input;
}


Servo_mgmt_s reverse_servos(Servo_mgmt_s inputs, Servo_reverse_s reverse)
{
	//
	Servo_mgmt_s uotputs;

	for(int i = 0; i < 10; i++)
	{
		//
		if (reverse.S[i])
		{
			//
			uotputs.S[i] = reverse_servo_value(inputs.S[i]);
		}
		else
		{
			//
			uotputs.S[i] = inputs.S[i];
		}
	}

	return uotputs;
}

float saturation(float signal_in,float sat_min,float sat_max)
{
    float signal_out;
    if (signal_in < sat_min)
    {
    	signal_out = sat_min;
    }
    else if (signal_in > sat_max)
    {
    	signal_out = sat_max;
    }
    else
    {
    	signal_out = signal_in;
    }
    return signal_out;
}

Cmd_s get_commands_rc(void)
{
	//
	Cmd_s out_norms;

	out_norms.roll 		= ((float)Radio_input.Canal_1 - 1500.0 ) / 500.0 ;

	out_norms.pitch 	= ((float)Radio_input.Canal_2 - 1500.0 ) / 500.0 ;

	out_norms.yaw 		= ((float)Radio_input.Canal_3 - 1500.0 ) / 500.0 ;

	out_norms.thrust 	= ((float)Radio_input.Canal_4 - 1000.0 ) / 1000.0 ;

	return out_norms;
}
