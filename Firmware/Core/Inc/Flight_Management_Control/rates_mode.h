/*
 * rates_mode.h
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_RATES_MODE_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_RATES_MODE_H_

#include "Flight_Management_Control/flight_parameters.h"
#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/sensors.h"


Cmd_s rates_control_law(Cmd_s inputs_norms, Gyro_s actual_gyro, Cmd_s K);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_RATES_MODE_H_ */
