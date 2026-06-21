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
Cmd_s Command ;
float commands_noise[4] ;
float ouput_fltr[4];
float cmds_out[4];
Servo_reverse_s Reverse ;
Servo_mgmt_s servo_outs ;
FilterState filters[4] = {{0.0f}, {0.0f}, {0.0f}, {0.0f}};
float COF_cmds[4] =
{
	CutOffFreq_cmd_ail,
	CutOffFreq_cmd_ele,
	CutOffFreq_cmd_rud,
	CutOffFreq_cmd_thr
};


Cmd_s command_filtering(Cmd_s command_in, FilterSwitch_e filter_enable)
{
	Cmd_s command_out;

	if(filter_enable == FILTER_ON)
	{
		command_out.roll   = filter_step(&filters[0], command_in.roll,   COF_cmds[0], SAMPLE_ATT);
		command_out.pitch  = filter_step(&filters[1], command_in.pitch,  COF_cmds[1], SAMPLE_ATT);
		command_out.yaw    = filter_step(&filters[2], command_in.yaw,    COF_cmds[2], SAMPLE_ATT);
		command_out.thrust = filter_step(&filters[3], command_in.thrust, COF_cmds[3], SAMPLE_ATT);
	}
	else
	{
		/*
		 * Bypass del filtro.
		 * Además sincronizamos los estados internos para evitar saltos
		 * si después se vuelve a activar FILTER_ON.
		 */
		command_out = command_in;

		filters[0].x = command_in.roll;
		filters[1].x = command_in.pitch;
		filters[2].x = command_in.yaw;
		filters[3].x = command_in.thrust;
	}


	return command_out;
}

void control_allocator(Cmd_s control_cmd, Cmd_s trims )
{
	//
	Frame = CONFIGURATION;
	if(Radio_input.Armed){
		LED_Info.G_LED.LED_status = 1;
	}
	else{
		LED_Info.G_LED.LED_status = 0;
		control_cmd.thrust = 0;
	}

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
		case VTAIL:
			servo_outs = VTail_MIXER(control_cmd, trims)  ;
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

	out_norms.thrust 	= ((float)Radio_input.Canal_3 - 1000.0 ) / 1000.0 ;

	out_norms.yaw 		= ((float)Radio_input.Canal_4 - 1500.0 ) / 500.0 ;

	return out_norms;
}

void get_actual_trims(void)
{
	//
	Trims.roll 		=  (float)Radio_input.Canal_5 - 1500.0 ;
	Trims.pitch 	=  (float)Radio_input.Canal_6 - 1500.0 ;
	Trims.yaw 		=  (float)Radio_input.Canal_7 - 1500.0 ;
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
