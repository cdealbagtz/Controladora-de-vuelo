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
 * @brief Calcula un paso de filtrado con Runge–Kutta 4.
 *
 * @param state   Puntero al estado del filtro.
 * @param u       Entrada actual (señal a filtrar).
 * @param cut_off Parámetro proporcional a la frecuencia de corte.
 *                Para un filtro RC clásico: cut_off = 1/(RC).
 * @param DT      Paso de integración (segundos).
 *
 * @return        Valor de salida filtrada.
 */
float filter_step(FilterState *state, float u, float cut_off, float DT);

#endif /* INC_FILTER_H_ */
