/*
 * control_allocator.c
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/servo_mixers.h"

FRAMES_e Frame ;
Cmd_s Command_out ;
Cmd_s Control_out ;
Cmd_s Trims ;
Cmd_s Commands ;
float commands_noise[4] ;
float ouput_fltr[4];
float cmds_out[4];
float COF_cmds[4];
Servo_reverse_s Reverse ;
Servo_mgmt_s servo_outs ;
FilterState filters[4] = {{0.0f}, {0.0f}, {0.0f}, {0.0f}};


void command_filtering(void)
{
	//
	commands_noise[0] = Command_out.roll   ;
	commands_noise[1] = Command_out.pitch  ;
	commands_noise[2] = Command_out.yaw    ;
	commands_noise[3] = Command_out.thrust ;

	for(int idx = 0; idx < 4; idx++)
	{
		//
		ouput_fltr[idx] = filter_step(&filters[idx], commands_noise[idx],COF_cmds[idx],SAMPLE_ATT) ;
	}
	//
	Commands.roll 		= 	ouput_fltr[0] ;
	Commands.pitch 		= 	ouput_fltr[1] ;
	Commands.yaw 		= 	ouput_fltr[2] ;
	Commands.thrust 	= 	ouput_fltr[3] ;


}

void control_allocator(Cmd_s control_cmd, Cmd_s trims )
{
	//
	Frame = CONFIGURATION;


	switch(Frame){
		case FIX_WING:
			servo_outs = FIX_WING_MIXER(control_cmd, trims)     ;
			break;
		case FLYING_WING:
			servo_outs = FLYING_WING_MIXER(control_cmd, trims)  ;
			break;
		case TANDEM_WING:
			servo_outs = TANDEM_WING_MIXER(control_cmd, trims)  ;
			break;
		case CUSTOM_FRAME:
			servo_outs = CUSTOM_FRAME_MIXER(control_cmd, trims) ;
			break;
		default:

			for(int i = 0; i < 10; i++)
			{
				//
				servo_outs.S[i] = 1500 ;
			}

		}

	PWM_Output = reverse_servos(servo_outs );
}

uint16_t reverse_servo_value(uint16_t input)
{
    // Asegurarnos que el valor esté dentro del rango válido
    if (input < 1000) input = 1000;
    if (input > 2000) input = 2000;

    // Calcular el valor invertido
    return 3000 - input;
}


Servo_mgmt_s reverse_servos(Servo_mgmt_s inputs )
{
	//
	Servo_mgmt_s uotputs;

	for(int i = 0; i < 10; i++)
	{
		//
		if (Reverse.S[i])
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

void get_actual_trims(void)
{
	//
	Trims.roll 		=  (float)Radio_input.Canal_10 - 1500.0 ;
	Trims.pitch 	=  (float)Radio_input.Canal_11 - 1500.0 ;
	Trims.yaw 		=  (float)Radio_input.Canal_12 - 1500.0 ;
	Trims.thrust 	=  0.0f ;
}


void init_Reverse_Servos(Servo_reverse_s *servos)
{
    for (int i = 0; i < 10; i++)
    {
    	//
        servos->S[i] = false;
    }
}
