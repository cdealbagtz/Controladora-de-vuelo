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

// Estructuras modificadas con arrays
typedef struct {
    uint16_t S[10]; // S[0] a S[9] para los 10 servos (S_1 a S_10)
} Servo_mgmt_s;

extern Servo_mgmt_s PWM_Output;

void PWM_Assign(void);


#endif /* INC_LIBRARIES_PWM_H_ */
