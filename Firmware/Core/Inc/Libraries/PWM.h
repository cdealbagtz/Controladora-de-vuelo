/*
 * PWM.h
 *
 *  Created on: Feb 21, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_PWM_H_
#define INC_LIBRARIES_PWM_H_

#include "stm32h7xx.h"
#include "tim.h"

typedef struct{
	uint16_t Canal_1; // 1000 a 2000
	uint16_t Canal_2; // 1000 a 2000
	uint16_t Canal_3; // 1000 a 2000
	uint16_t Canal_4; // 1000 a 2000
	uint16_t Canal_5; // 1000 a 2000
	uint16_t Canal_6; // 1000 a 2000
	uint16_t Canal_7; // 1000 a 2000
	uint16_t Canal_8; // 1000 a 2000
	uint16_t Canal_9; // 1000 a 2000
	uint16_t Canal_10;// 1000 a 2000
}PWM_Output_t;

extern PWM_Output_t PWM_Output;

void PWM_Assing(void);


#endif /* INC_LIBRARIES_PWM_H_ */
