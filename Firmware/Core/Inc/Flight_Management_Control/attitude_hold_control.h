/*
 * attitude_hold_control.h
 *
 *  Created on: Sep 4, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_ATTITUDE_HOLD_CONTROL_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_ATTITUDE_HOLD_CONTROL_H_

#include "Flight_Management_Control/flight_parameters.h"
#include "Flight_Management_Control/control_allocator.h"
#include "Flight_Management_Control/sensors.h"
#include "Flight_Management_Control/pid.h"

void init_PIDs(void);

Cmd_s attitude_hold_control(Cmd_s inputs_norms, Att_s actual_atts , Gyro_s actual_gyro, Cmd_s K);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_ATTITUDE_HOLD_CONTROL_H_ */
