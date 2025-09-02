/*
 * filter.c
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */
/* Includes ------------------------------------------------------------------*/
#include "filter.h"
#include <math.h>

LPF_s filtering_lpf(LPF_s *filtro)
{
    /* Filtro pasa-bajas:
     *  Solución numérica a
     *   dy
     *  ---- = 2π*f_cutoff*(u(t) - y(t))
     *   dt
     *   donde:
     *   f_cutoff -> frecuencia de corte
     *   u -> Señal de entrada
     *   y -> Señal de salida
     *   */

    LPF_s resultado = *filtro;
    static float y_n;

    // Validación básica de parámetros
    if(filtro->f_cutoff <= 0.0f || filtro->t_sample <= 0.0f) {
        return resultado;  // O manejar error adecuadamente
    }

    const float w_0 = filtro->f_cutoff * 6.283185307179586f; // 2*PI

    if(filtro->inicio) {
        y_n = resultado.Y_n = resultado.Y_nm1 = filtro->Y_0;
        resultado.inicio = 0;
        return resultado;
    }

    const float u_n = filtro->U_n;
    const float h = filtro->t_sample;
    const float h2 = h * .50f;

    // Runge-Kutta 4to orden
    const float k1 = w_0 * (u_n - y_n);
    const float k2 = w_0 * (u_n - (y_n + k1 * h2));
    const float k3 = w_0 * (u_n - (y_n + k2 * h2));
    const float k4 = w_0 * (u_n - (y_n + k3 * h));

    y_n += h * (0.166666667f * k1 + 0.333333333f * (k2 + k3) + 0.166666667f * k4);

    // Manejo robusto de NaN (protección contra condiciones inválidas)
    y_n = isnan(y_n) ? resultado.Y_nm1 : y_n;

    // Actualizar estructura de resultado
    resultado.Y_n = resultado.Y_nm1 = y_n;
    resultado.U_nm1 = u_n;
    resultado.t_nm1 = resultado.t_n;

    return resultado;
}

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
