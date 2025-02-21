/*
 * SBUS.h
 *
 *  Created on: Feb 21, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_SBUS_H_
#define INC_LIBRARIES_SBUS_H_

#include "stm32h7xx.h"
#include "usart.h"

typedef struct Radio_input{
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
	uint16_t Canal_11;// 1000 a 2000
	uint16_t Canal_12;// 1000 a 2000
	uint16_t Canal_13;// 1000 a 2000
	uint16_t Canal_14;// 1000 a 2000
	uint16_t Canal_15;// 1000 a 2000
	uint16_t Canal_16;// 1000 a 2000

	uint8_t Interruptor_1; // 0 a 1
	uint8_t Interruptor_2; // 0 a 1

	uint8_t pkg_lost;
	uint8_t fail_safe;
}Radio_input_t;

void SBUS_init(void);
void SBUS_Receive(uint8_t SBUS_RxBuffer);
void SBUS_getData(void);

#endif /* INC_LIBRARIES_SBUS_H_ */
