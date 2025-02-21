/*
 * PWM.c
 *
 *  Created on: Feb 21, 2025
 *      Author: cdealba
 */

#include "Libraries/PWM.h"

PWM_Output_t PWM_Output = {
		.Canal_1  = 1500,
		.Canal_2  = 1500,
		.Canal_3  = 1500,
		.Canal_4  = 1500,
		.Canal_5  = 1500,
		.Canal_6  = 1500,
		.Canal_7  = 1500,
		.Canal_8  = 1500,
		.Canal_9  = 1500,
		.Canal_10 = 1500,
};

void PWM_Assing(void){
	TIM2->CCR1 = PWM_Output.Canal_1;
	TIM2->CCR2 = PWM_Output.Canal_2;

	TIM3->CCR1 = PWM_Output.Canal_3;
	TIM3->CCR2 = PWM_Output.Canal_4;

	TIM4->CCR1 = PWM_Output.Canal_5;
	TIM4->CCR2 = PWM_Output.Canal_6;
	TIM4->CCR3 = PWM_Output.Canal_7;
	TIM4->CCR4 = PWM_Output.Canal_8;

	TIM2->CCR4 = PWM_Output.Canal_9;
	TIM2->CCR3 = PWM_Output.Canal_10;

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
