/*
 * pid.c
 *
 *  Created on: Sep 5, 2025
 *      Author: rtrujillo
 */
#include "Flight_Management_Control/pid.h"

void PI_Init(PI_Controller *pi, float kp, float ki, float out_min, float out_max) {
    pi->Kp = kp; pi->Ki = ki;
    pi->integrator = 0.0f;
    pi->out_min = out_min; pi->out_max = out_max;
}

float PI_Compute(PI_Controller *pi, float error, float dt) {
    pi->integrator += 0.0f ; //error * pi->Ki * dt;

    // Anti-windup
    if (pi->integrator > pi->out_max) pi->integrator = pi->out_max;
    else if (pi->integrator < pi->out_min) pi->integrator = pi->out_min;

    float output = pi->Kp * error ; //+ pi->integrator;

    // Saturación final
    output = SatComando(output, pi->out_min, pi->out_max);

    return output;
}

float SatComando(float rc_comando,float sat_min,float sat_max)
{
    float rc_comando_out;
    if (rc_comando < sat_min)
    {
        rc_comando_out = sat_min;
    }
    else if (rc_comando > sat_max)
    {
        rc_comando_out = sat_max;
    }
    else
    {
        rc_comando_out = rc_comando;
    }
    return rc_comando_out;
}
