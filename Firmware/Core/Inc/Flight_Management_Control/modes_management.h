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
#include "Libraries/LED.h"
#include "Flight_Management_Control/flight_modes_config.h"

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


extern FlightMode_t mode ;

FlightMode_t ModesManagement_ReadSwitch3Pos(void);

void get_flight_mode(void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_MODES_MANAGEMENT_H_ */
