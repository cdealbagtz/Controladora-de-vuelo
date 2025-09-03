/*
 * filter.h
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

// Estructura del filtro de primer orden
typedef struct {
    float x; // estado (salida)
} FilterState;



// Prototipos

// Derivada dx/dt = f(x,u,par)
float f(float x, float u,float par);

// Paso Runge-Kutta 4
void rk4_step(FilterState *state, float u, float dt, float params);

// Aplicar filtro
float filter_step(FilterState *state, float u, float cut_off, float DT);

#endif /* INC_FILTER_H_ */
