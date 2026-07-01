/*
 * flight_modes_config.h
 *
 * Configuración del orden de modos de vuelo.
 *
 *  Created on: Jun 20, 2026
 *      Author: rafa
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MODES_CONFIG_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MODES_CONFIG_H_


/*
 * Canal RC usado para seleccionar modo de vuelo.
 *
 * Cambia aquí el canal según tu configuracion de radio:
 *Ejemplos:
 * * Canal 5  -> Radio_input.Canal_5
 * * Canal 10 -> Radio_input.Canal_10
 */

//#define FLIGHT_MODE_PWM_INPUT()     (Radio_input.Canal_10)

/*
 * Rangos PWM para switch de 3 posiciones.
 */
// Configuración de los límites PWM (ajustar según tu radio)
#define PWM_MIN 		1000U    // μs (posición baja)
#define PWM_MID 		1500U    // μs (posición media)
#define PWM_MAX 		2000U    // μs (posición alta)
#define PWM_DEADZONE 	250U  // μs (zona muerta para evitar ruido)




/*
 * Orden de modos para switch de 3 posiciones.
 *
 * LOW  = posición baja del switch
 * MID  = posición media del switch
 * HIGH = posición alta del switch
 */

#define FLIGHT_MODE_SW_LOW      MANUAL_MODE
#define FLIGHT_MODE_SW_MID      MANUAL_SYSIDE_MODE
#define FLIGHT_MODE_SW_HIGH     ATTITUDE_HOLD_MODE


/*
 * Modo de failsafe.
 */
#define FLIGHT_MODE_FAILSAFE    FAILSAFE_MODE

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MODES_CONFIG_H_ */
