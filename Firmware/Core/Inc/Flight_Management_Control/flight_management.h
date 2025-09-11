/*
 * flight_management.h
 *
 *  Created on: Jul 24, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MANAGEMENT_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MANAGEMENT_H_

#include "Flight_Management_Control/flight_parameters.h"
#include "Flight_Management_Control/modes_management.h"
#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/sensors.h"


#include "Flight_Management_Control/rates_mode.h"
#include "Flight_Management_Control/attitude_hold_control.h"

extern Cmd_s rates_Gains;

void FlightControl_inits(void);

void FlightTaskAttitude(void);

void FlightTaskARSP(void);

void AttitudeHold(void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_FLIGHT_MANAGEMENT_H_ */
