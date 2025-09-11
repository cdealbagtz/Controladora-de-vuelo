/*
 * attitude_hold_control.c
 *
 *  Created on: Sep 4, 2025
 *      Author: rtrujillo
 */

#include "Flight_Management_Control/attitude_hold_control.h"

PI_Controller pid_roll ;
PI_Controller pid_pitch;

float theta_ref , phi_ref;
float error_phi ,  error_theta ;
float u_phi, u_theta ;

void init_PIDs(void)
{
	//para inicializacion del roll

	pid_roll.Kp = KP_roll;
	pid_roll.Ki = KI_roll;
	pid_roll.integrator = 0.0f;
	pid_roll.out_min = -1.0f;
	pid_roll.out_max = 1.0f;

	pid_pitch.Kp = KP_pitch;
	pid_pitch.Ki = KI_pitch;
	pid_pitch.integrator = 0.0f;
	pid_pitch.out_min = -1.0f;
	pid_pitch.out_max = 1.0f;
}

Cmd_s attitude_hold_control(Cmd_s inputs_norms, Att_s actual_atts , Gyro_s actual_gyro, Cmd_s K)
{
	//
	Cmd_s Outs;
	// Attitude reference
	theta_ref = inputs_norms.pitch * pitch_sat ;
	phi_ref   = inputs_norms.roll  * roll_sat  ;

	// Attitude Error
	error_phi 	= phi_ref 	- actual_atts.roll ;
	error_theta = theta_ref - actual_atts.pitch ;

	// Ejecucion del PID
	u_phi 	=  PI_Compute(&pid_roll,  error_phi  , SAMPLE_ATT ) ;
	u_theta =  PI_Compute(&pid_pitch, error_theta, SAMPLE_ATT ) ;

	Outs.roll   = u_phi   -  actual_gyro.roll * K.roll  ;
	Outs.pitch  = u_theta -  actual_gyro.pitch* K.pitch ;

	//Yaw Damping
	Outs.yaw    = inputs_norms.yaw   -  actual_gyro.yaw  * K.yaw ;
	//Feedforwa
	Outs.thrust = inputs_norms.thrust ;

	return Outs;
}
