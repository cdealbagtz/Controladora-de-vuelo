/*
 * Lora_LR03.h
 *
 *  Created on: Jun 16, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_LORA_LR03_H_
#define INC_LIBRARIES_LORA_LR03_H_

#include "stm32h7b0xx.h"
#include "usart.h"

void LR03_StateMachine(void);
void LR03_Receive(uint8_t Data);

#endif /* INC_LIBRARIES_LORA_LR03_H_ */
