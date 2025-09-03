/*
 * filter.c
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */
/* Includes ------------------------------------------------------------------*/
#include "filter.h"
#include <math.h>

// Derivada dx/dt = f(x,u)
float f(float x, float u,float par) {
    return (-x * par ) + (u * par);
}

// Paso Runge-Kutta 4
void rk4_step(FilterState *state, float u, float dt, float params) {
    float k1, k2, k3, k4;

    k1 = f(state->x, u,params);
    k2 = f(state->x + 0.5f*dt*k1, u,params);
    k3 = f(state->x + 0.5f*dt*k2, u,params);
    k4 = f(state->x + dt*k3, u,params);

    state->x += (dt/6.0f)*(k1 + 2*k2 + 2*k3 + k4);
}

// Aplicar filtro
float filter_step(FilterState *state, float u, float cut_off, float DT) {
    rk4_step(state, u, DT, cut_off);
    return state->x;
}
