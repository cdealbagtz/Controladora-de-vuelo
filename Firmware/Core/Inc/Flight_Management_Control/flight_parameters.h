/*
 * flight_parameters.h
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_PARAMETERS_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_PARAMETERS_H_

#include <stdio.h>

#define SAMPLE_ATT 0.010f 	// Delta Tiempo de ejecución de la capa ATT.

#define CONFIGURATION 3 //



#define FLIGHT_MODE_PWM_INPUT()     (Radio_input.Canal_5)
#define FLIGHT_SYSIDE_ON()     		(Radio_input.Canal_11)


#define K_P 	0.005f
#define K_Q 	0.005f
#define K_R 	0.005f


#define LPF_gyro_x	50.0f
#define LPF_gyro_y 	50.0f
#define LPF_gyro_z	50.0f

#define CutOffFreq_cmd_ail	10.0f //Frecuancia de conrte (Hertz) comando alerones.
#define CutOffFreq_cmd_ele	10.0f //Frecuancia de conrte (Hertz) comando elevador.
#define CutOffFreq_cmd_rud	10.0f //Frecuancia de conrte (Hertz) comando timon.
#define CutOffFreq_cmd_thr	10.0f //Frecuancia de conrte (Hertz) comando acelerador.

#define pitch_sat 30.0f
#define roll_sat  40.0f

#define KP_roll  0.00000010f
#define KI_roll  0.00000000f
#define KP_pitch 0.01f
#define KI_pitch 0.0000010f

#define IDX_syside 	0u
#define AMP_syside 	0.4f
#define Freq_0 		1.0f
#define Time_start 	2.0f
#define Time_test 	10.0f

#define AMP_syside_wave 	0.4f
#define Freq_wave_Hertz		1.0f


#define Time_pulse_seg 		1.0f
#define AMP_syside_pulse 	0.4f

#define Time_doublet_seg 	2.0f

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_PARAMETERS_H_ */
