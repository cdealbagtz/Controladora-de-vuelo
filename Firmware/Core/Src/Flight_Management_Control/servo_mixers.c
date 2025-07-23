/*
 * servo_mixers.c
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/servo_mixers.h"



Servo_outputs_s FIX_WING_MIXER(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude)
{
	//
	Servo_outputs_s mixer_output;

	mixer_output.S_1  = 1500 + Amplitude.S_1 * control_cmd.roll  + (1500 - trims.roll  ) ;
	mixer_output.S_2  = 1500 + Amplitude.S_2 * control_cmd.roll  + (1500 - trims.roll  ) ;
	mixer_output.S_3  = 1500 + Amplitude.S_3 * control_cmd.pitch + (1500 - trims.pitch ) ;
	mixer_output.S_4  = 1500 + Amplitude.S_4 * control_cmd.pitch + (1500 - trims.pitch ) ;
	mixer_output.S_5  = 1500 + Amplitude.S_5 * control_cmd.yaw   + (1500 - trims.yaw   ) ;
	mixer_output.S_6  = 1500 + Amplitude.S_6 * control_cmd.yaw   + (1500 - trims.yaw   ) ;
	mixer_output.S_7  = 1000 + Amplitude.S_7 * control_cmd.thrust+ (1500 - trims.thrust) ;
	mixer_output.S_8  = 1000 ;
	mixer_output.S_9  = 1000 ;
	mixer_output.S_10 = 1000 ;

	return mixer_output;
}

Servo_outputs_s FLYING_WING_MIXER(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude)
{
	//
	Servo_outputs_s mixer_output;

	mixer_output.S_1  = 1500 + Amplitude.S_1 * control_cmd.roll  + (1500 + trims.roll  ) ;
	mixer_output.S_2  = 1500 + Amplitude.S_2 * control_cmd.roll  + (1500 + trims.roll  ) ;
	mixer_output.S_3  = 1500 + Amplitude.S_3 * control_cmd.pitch + (1500 + trims.pitch ) ;
	mixer_output.S_4  = 1500 + Amplitude.S_4 * control_cmd.pitch + (1500 + trims.pitch ) ;
	mixer_output.S_5  = 1500 + Amplitude.S_5 * control_cmd.yaw   + (1500 + trims.yaw   ) ;
	mixer_output.S_6  = 1500 + Amplitude.S_6 * control_cmd.yaw   + (1500 + trims.yaw   ) ;
	mixer_output.S_7  = 1000 + Amplitude.S_7 * control_cmd.thrust+ (1500 + trims.thrust) ;
	mixer_output.S_8  = 1000 ;
	mixer_output.S_9  = 1000 ;
	mixer_output.S_10 = 1000 ;

	return mixer_output;
}
