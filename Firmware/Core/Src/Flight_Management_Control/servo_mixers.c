/*
 * servo_mixers.c
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/servo_mixers.h"



Servo_mgmt_s FIX_WING_MIXER(Cmd_s control_cmd, Trim_s trims)
{
	//
	Servo_mgmt_s mixer_output;

	mixer_output.S[0]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[1]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[2]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[3]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[4]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[5]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[6]  = (uint16_t)(1000.0f + 500.f * control_cmd.thrust+ (1500 - trims.thrust)) ;
	mixer_output.S[7]  = (uint16_t)(1500.0f );
	mixer_output.S[8]  = (uint16_t)(1500.0f );
	mixer_output.S[9]  = (uint16_t)(1500.0f );

	return mixer_output;
}

Servo_mgmt_s FLYING_WING_MIXER(Cmd_s control_cmd, Trim_s trims)
{
	//
	Servo_mgmt_s mixer_output;

	mixer_output.S[0]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[1]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[2]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[3]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[4]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[5]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[6]  = (uint16_t)(1000.0f + 500.f * control_cmd.thrust+ (1500 - trims.thrust)) ;
	mixer_output.S[7]  = (uint16_t)(1500.0f );
	mixer_output.S[8]  = (uint16_t)(1500.0f );
	mixer_output.S[9]  = (uint16_t)(1500.0f );

	return mixer_output;
}

Servo_mgmt_s TANDEM_WING_MIXER(Cmd_s control_cmd, Trim_s trims)
{
	//
	Servo_mgmt_s mixer_output;

	mixer_output.S[0]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[1]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[2]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[3]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[4]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[5]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[6]  = (uint16_t)(1000.0f + 500.f * control_cmd.thrust+ (1500 - trims.thrust)) ;
	mixer_output.S[7]  = (uint16_t)(1500.0f );
	mixer_output.S[8]  = (uint16_t)(1500.0f );
	mixer_output.S[9]  = (uint16_t)(1500.0f );

	return mixer_output;
}

Servo_mgmt_s CUSTOM_FRAME_MIXER(Cmd_s control_cmd, Trim_s trims)
{
	//
	Servo_mgmt_s mixer_output;

	mixer_output.S[0]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[1]  = (uint16_t)(1500.0f + 500.f * control_cmd.roll  + (1500 - trims.roll  )) ;
	mixer_output.S[2]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[3]  = (uint16_t)(1500.0f + 500.f * control_cmd.pitch + (1500 - trims.pitch )) ;
	mixer_output.S[4]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[5]  = (uint16_t)(1500.0f + 500.f * control_cmd.yaw   + (1500 - trims.yaw   )) ;
	mixer_output.S[6]  = (uint16_t)(1000.0f + 500.f * control_cmd.thrust+ (1500 - trims.thrust)) ;
	mixer_output.S[7]  = (uint16_t)(1500.0f );
	mixer_output.S[8]  = (uint16_t)(1500.0f );
	mixer_output.S[9]  = (uint16_t)(1500.0f );

	return mixer_output;
}
