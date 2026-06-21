/*
 * filter.h
 *
 *  Created on: May 27, 2025
 *      Author: Rafael Trujillo
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

/**
 * @struct FilterState
 * @brief Contiene el estado interno del filtro (la salida actual).
 */

typedef struct {
    float x; /**< Estado interno (equivale a la salida actual). */
} FilterState;

/**
 * @brief Calcula un paso de filtrado usando integración Runge-Kutta 4.
 *
 * El filtro implementado es un pasa-bajas de primer orden.
 *
 * A nivel de configuración, la frecuencia de corte se maneja en Hertz:
 *
 *      cut_off = fc_hz
 *
 * Internamente, el filtro convierte la frecuencia de corte a rad/s:
 *
 *      omega_c = 2*pi*fc_hz
 *
 * y usa el modelo continuo:
 *
 *      dx/dt = omega_c * (u - x)
 *
 * donde:
 *
 *      u       = entrada actual del filtro
 *      x       = estado interno del filtro
 *              = salida filtrada
 *      fc_hz   = frecuencia de corte en Hertz
 *      omega_c = frecuencia angular de corte en rad/s
 *
 * Función de transferencia equivalente:
 *
 *      G(s) = omega_c / (s + omega_c)
 *
 * Nota:
 *      La interfaz de esta función se mantiene como:
 *
 *          filter_step(state, u, cut_off, DT)
 *
 *      pero en este proyecto cut_off debe interpretarse como Hertz.
 *
 * Ejemplo:
 *
 *      cut_off = 5.0f;
 *
 * significa:
 *
 *      fc_hz   = 5 Hz
 *      omega_c = 2*pi*5 = 31.416 rad/s
 *
 * @param state    Puntero al estado interno del filtro.
 * @param u        Entrada actual, es decir, la señal a filtrar.
 * @param cut_off  Frecuencia de corte en Hz.
 * @param DT       Paso de integración en segundos.
 *
 * @return Valor de salida filtrada.
 */
float filter_step(FilterState *state, float u, float cut_off, float DT);

#endif /* INC_FILTER_H_ */
