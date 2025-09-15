/*
 * pid.h
 *
 *  Created on: Sep 5, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_PID_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_PID_H_

typedef struct {
    float Kp;
    float Ki;
    float integrator;
    float out_min;
    float out_max;
} PI_Controller;

void PI_Init(PI_Controller *pi, float kp, float ki, float out_min, float out_max);

float PI_Compute(PI_Controller *pi, float error, float dt);

float SatComando(float rc_comando,float sat_min,float sat_max);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_PID_H_ */
