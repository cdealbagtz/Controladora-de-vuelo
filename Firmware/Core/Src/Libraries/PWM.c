/*
 * PWM.c
 *
 *  Created on: Feb 21, 2025
 *      Author: cdealba
 */

#include "Libraries/PWM.h"

Servo_mgmt_s PWM_Output = {
	    .S = {1500, 1500, 1500, 1500, 1500,
	          1500, 1500, 1500, 1500, 1500}
	};

void PWM_Assign(void) {
    // Timer 2 (Canales 1, 2, 10, 9)
    TIM2->CCR1 = PWM_Output.S[0];  // Canal_1 (S[0])
    TIM2->CCR2 = PWM_Output.S[1];  // Canal_2 (S[1])
    TIM2->CCR3 = PWM_Output.S[9];  // Canal_10 (S[9])
    TIM2->CCR4 = PWM_Output.S[8];  // Canal_9 (S[8])

    // Timer 3 (Canales 3, 4)
    TIM3->CCR1 = PWM_Output.S[2];  // Canal_3 (S[2])
    TIM3->CCR2 = PWM_Output.S[3];  // Canal_4 (S[3])

    // Timer 4 (Canales 5-8)
    TIM4->CCR1 = PWM_Output.S[4];  // Canal_5 (S[4])
    TIM4->CCR2 = PWM_Output.S[5];  // Canal_6 (S[5])
    TIM4->CCR3 = PWM_Output.S[6];  // Canal_7 (S[6])
    TIM4->CCR4 = PWM_Output.S[7];  // Canal_8 (S[7])

    // Iniciar PWM (igual que antes)
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}
