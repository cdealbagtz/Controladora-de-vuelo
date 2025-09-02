/*
 * filter.h
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

typedef struct {
    float Y_n;        // Valor actual de salida
    float Y_nm1;      // Valor anterior de salida
    float Y_0;        // Valor inicial de salida
    float U_n;        // Valor actual de entrada
    float U_nm1;      // Valor anterior de entrada
    float U_0;        // Valor inicial de entrada
    float f_cutoff;   // Frecuencia de corte del filtro (Hz) Siempre positivo!
    float t_sample;   // Periodo de muestreo
    float t_n;        // Tiempo actual
    float t_nm1;      // Tiempo anterior
    int   inicio;     // Bandera de inicialización
} LPF_s;

// Estructura del filtro de primer orden
typedef struct {
    float x; // estado (salida)
} FilterState;

// Prototipos
LPF_s filtering_lpf(LPF_s *filtro);

// Derivada dx/dt = f(x,u,par)
float f(float x, float u,float par);

// Paso Runge-Kutta 4
void rk4_step(FilterState *state, float u, float dt, float params);

// Aplicar filtro
float filter_step(FilterState *state, float u, float cut_off, float DT);

#endif /* INC_FILTER_H_ */
