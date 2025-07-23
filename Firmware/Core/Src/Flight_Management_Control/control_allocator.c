/*
 * control_allocator.c
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/control_allocator.h"

Servo_outputs_s control_allocator(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude ,Servo_reverse_s reverse, uint8_t frame_type)
{
	//
	Servo_outputs_s servo_outputs;
	switch(frame_type){
	case 0:
		servo_outputs = FIX_WING_MIXER(control_cmd, trims, Amplitude);
		break;
	case 1:
		servo_outputs = FLYING_WING_MIXER(control_cmd, trims, Amplitude);
		break;
	case 2:
		servo_outputs = TANDEM_WING_MIXER(control_cmd, trims, Amplitude);
		break;
	case 3:
		servo_outputs = CUSTOM_FRAME_MIXER(control_cmd, trims, Amplitude);
		break;
	default:
		servo_outputs.S_1 = 1500;
		servo_outputs.S_2 = 1500;
		servo_outputs.S_3 = 1500;
		servo_outputs.S_4 = 1500;
		servo_outputs.S_5 = 1500;
		servo_outputs.S_6 = 1500;
		servo_outputs.S_7 = 1500;
		servo_outputs.S_8 = 1500;
		servo_outputs.S_9 = 1500;
		servo_outputs.S_10 = 1500;
	}



	return servo_outputs;
}
