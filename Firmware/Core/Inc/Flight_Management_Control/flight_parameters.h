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

#define CONFIGURATION 0 //

#define K_P 	0.005f
#define K_Q 	0.005f
#define K_R 	0.005f


#define LPF_gyro_x	50.0f
#define LPF_gyro_y 	50.0f
#define LPF_gyro_z	50.0f

#define CutOffFreq_cmd_ail	10.0f //Frecuancia de conrte comando alerones.
#define CutOffFreq_cmd_ele	10.0f //Frecuancia de conrte comando alerones.
#define CutOffFreq_cmd_rud	10.0f //Frecuancia de conrte comando alerones.
#define CutOffFreq_cmd_thr	10.0f //Frecuancia de conrte comando alerones.


#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_PARAMETERS_H_ */
