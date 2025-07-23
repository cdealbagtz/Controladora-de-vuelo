/*
 * servo_mixers.h
 *
 *  Created on: Jul 16, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_SERVO_MIXERS_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_SERVO_MIXERS_H_

#include <stdio.h>
#include "Flight_Management_Control/control_allocator.h"

#define FLYING_WING_MIX_AIL 0.5f
#define FLYING_WING_MIX_AIL 0.5f


Servo_outputs_s FIX_WING_MIXER(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude);
//todo falta configurar ala voladora, tandem y custom.
Servo_outputs_s FLYING_WING_MIXER(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude);

Servo_outputs_s TANDEM_WING_MIXER(Cmd_s control_cmd, Trim_s trims,Amplitude_servo_s Amplitude);

Servo_outputs_s CUSTOM_FRAME_MIXER(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_SERVO_MIXERS_H_ */
