/*
 * signal_gen.c
 *
 *  Created on: May 29, 2025
 *      Author: Rafael Trujillo
 */
#include "signal_gen.h"  //
#include <math.h>


// Constantes
#define PI 3.141592653589793f
#define TEST_DURATION 15.0f   // segundos
#define LAG_START 3.0f        //segundos

float generate_test_signal(float t, int signal_type, float t_sample) {
    if(t < LAG_START){
        //
        return 0.0f;
    }

    if (signal_type == 1) {
        // Señal multi-sinusoidal
        const float amplitudes[5] = {10.0f, 3.5f, 6.15f, 2.30f, 4.25f};
        const float frequencies[5] = {1.00f, 2.0f, 8.00f, 15.0f, 13.0f};
        float signal = 0.0f;

        for (int i = 0; i < 5; i++) {
            signal += amplitudes[i] * sinf(2 * PI * frequencies[i] * t);
        }
        return signal;
    } else if(signal_type == 2){
        // Barrido de frecuencia (chirp)
        static float theta = 0.0f;
        const float omega_min = 3.0f;  // rad/s
        const float omega_max = 60.0f; // rad/s
        const float c1 = 4.0f;
        const float c2 = 0.0187f;

        float K = c2 * (expf(c1 * t / TEST_DURATION) - 1);
        float omega = omega_min + K * (omega_max - omega_min);
        theta += omega * t_sample;

        return sinf(theta);
    }else if(signal_type == 3){
        //Pulsos Cuadrados
        return (fmodf(t, 2.0f) < 1.0f) ? 1.0f : -1.0f;

    }else {
        // Señal nula
        return 0.0f;
    }
}
