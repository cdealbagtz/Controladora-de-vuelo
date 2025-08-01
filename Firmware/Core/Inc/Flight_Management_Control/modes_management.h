/*
 * modes_management.h
 *
 *  Created on: Jul 25, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_MODES_MANAGEMENT_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_MODES_MANAGEMENT_H_

#include "stdint.h"
#include "Libraries/SBUS.h"

typedef enum {
    MANUAL_MODE = 0,
    RATE_MODE,
    ATTITUDE_HOLD_MODE,
	FBW_MODE,
    FAILSAFE_MODE
} FlightMode_t;

typedef struct {
    uint16_t S[3]; // S[0] a S[2]
} Switch_modes_s;

// Configuración de los límites PWM (ajustar según tu radio)
#define PWM_MIN 1000    // μs (posición baja)
#define PWM_MID 1500    // μs (posición media)
#define PWM_MAX 2000    // μs (posición alta)
#define PWM_DEADZONE 50 // μs (zona muerta para evitar ruido)

uint8_t CurrentMode(void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_MODES_MANAGEMENT_H_ */
