/*
 * filter.c
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */
/* Includes ------------------------------------------------------------------*/
#include "filter.h"
#include <math.h>

/* Private defines -----------------------------------------------------------*/

#ifndef FILTER_TWO_PI
#define FILTER_TWO_PI    (6.28318530718f)
#endif

/*
 * Filtro pasa-bajas de primer orden
 * ---------------------------------
 *
 * En este proyecto, la frecuencia de corte que recibe filter_step()
 * se maneja en Hertz por claridad para electrónica, sensores y control.
 *
 * Sin embargo, el modelo matemático continuo del filtro usa frecuencia
 * angular en rad/s.
 *
 * Conversión:
 *
 *      omega_c = 2*pi*fc_hz
 *
 * Modelo continuo:
 *
 *      dx/dt = omega_c * (u - x)
 *
 * donde:
 *
 *      u       = entrada del filtro
 *      x       = estado interno del filtro
 *              = salida filtrada
 *      fc_hz   = frecuencia de corte en Hertz
 *      omega_c = frecuencia angular de corte en rad/s
 *
 * Función de transferencia equivalente:
 *
 *      G(s) = omega_c / (s + omega_c)
 *
 * Ejemplo:
 *
 *      fc_hz = 5 Hz
 *
 * Entonces:
 *
 *      omega_c = 2*pi*5 = 31.416 rad/s
 */

/**
 * @brief Convierte frecuencia en Hz a frecuencia angular en rad/s.
 *
 * @param frequency_hz Frecuencia en Hertz.
 *
 * @return Frecuencia angular en rad/s.
 */
static float hz_to_rad_s(float frequency_hz)
{
    return FILTER_TWO_PI * frequency_hz;
}

/**
 * @brief Ecuación diferencial ordinaria del filtro pasa-bajas de primer orden.
 *
 * Implementa:
 *
 *      dx/dt = omega_c * (u - x)
 *
 * Importante:
 *      Esta función recibe omega_c en rad/s.
 *      La conversión desde Hz se hace antes de llamarla.
 *
 * @param x        Estado actual del filtro.
 * @param u        Entrada actual del filtro.
 * @param omega_c  Frecuencia angular de corte en rad/s.
 *
 * @return Derivada dx/dt.
 */
static float first_order_filter_ode(float x, float u, float omega_c)
{
    return omega_c * (u - x);
}

// Derivada dx/dt = f(x,u)
float f(float x, float u,float par) {
    return (-x * par ) + (u * par);
}

// Paso Runge-Kutta 4
/**
 * @brief Integra un paso usando Runge-Kutta de cuarto orden.
 *
 * Esta función actualiza el estado interno del filtro.
 *
 * @param state    Estado interno del filtro.
 * @param u        Entrada actual.
 * @param dt       Paso de integración en segundos.
 * @param omega_c  Frecuencia angular de corte en rad/s.
 */
void rk4_step(FilterState *state, float u, float dt, float omega_c)
{
    float k1, k2, k3, k4;

    k1 = first_order_filter_ode(state->x, u, omega_c);
    k2 = first_order_filter_ode(state->x + 0.5f * dt * k1, u, omega_c);
    k3 = first_order_filter_ode(state->x + 0.5f * dt * k2, u, omega_c);
    k4 = first_order_filter_ode(state->x + dt * k3, u, omega_c);

    state->x += (dt / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
}

// Aplicar filtro
/**
 * @brief Calcula un paso del filtro pasa-bajas.
 *
 * Esta función mantiene la interfaz original del proyecto:
 *
 *      filter_step(state, u, cut_off, DT)
 *
 * pero por convención del proyecto:
 *
 *      cut_off está en Hz.
 *
 * Internamente se convierte a rad/s antes de integrar la ecuación
 * diferencial del filtro.
 *
 * @param state    Estado interno del filtro.
 * @param u        Entrada actual.
 * @param cut_off  Frecuencia de corte en Hz.
 * @param DT       Paso de integración en segundos.
 *
 * @return Salida filtrada.
 */
float filter_step(FilterState *state, float u, float cut_off, float DT)
{
	float omega_c;

	omega_c = hz_to_rad_s(cut_off);

    rk4_step(state, u, DT, cut_off);

    return state->x;
}
