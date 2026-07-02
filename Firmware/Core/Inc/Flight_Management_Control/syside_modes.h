/*
 * syside_modes.h
 *
 *  Created on: Jun 27, 2026
 *      Author: rafa
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_SYSIDE_MODES_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_SYSIDE_MODES_H_

#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/flight_parameters.h"

typedef enum {
    SINE_MODE = 0,
	PULSE_MODE,
    DOUBLET_MODE,
	CHIRP_MODE,
    EXTRA_MODE
} SysideTest_t;

/*
 * Rangos PWM para switch de 3 posiciones.
 */
// Configuración de los límites PWM (ajustar según tu radio)
#define PWM_MIN 		1000U    // μs (posición baja)
#define PWM_MID 		1500U    // μs (posición media)
#define PWM_MAX 		2000U    // μs (posición alta)
#define PWM_DEADZONE 	250U  // μs (zona muerta para evitar ruido)


void reset_syside_timer(void);

Cmd_s sine_wave_axis(int8_t axis, float time_start, float time_test, Cmd_s inputs_norms, float freq_Hz, float Amplitude, float Period_t);


Cmd_s System_Identification_Test(int8_t axis, Cmd_s inputs_norms, float sample_time);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_SYSIDE_MODES_H_ */
