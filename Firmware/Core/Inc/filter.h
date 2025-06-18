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

// Prototipos
LPF_s filtrado_lpf(LPF_s *filtro);

#endif /* INC_FILTER_H_ */
