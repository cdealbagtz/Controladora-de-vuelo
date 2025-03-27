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

typedef enum RC_Status{
	OFF = 0x00,
	ON  = 0xFF,
	Ok = 0xAA,
	Pkg_Lost = 0x10,
	Uart_Error = 0x20,
	FailSafe = 0x35
}RC_Status_e;

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

	RC_Status_e Interruptor_1;
	RC_Status_e Interruptor_2;

	RC_Status_e pkg_lost;
	RC_Status_e fail_safe;
	RC_Status_e uart_error;

	uint8_t  Uart_Counter;
}Radio_input_t;

void SBUS_IntegrityVerification(void);


void SBUS_init(void);
void SBUS_Receive(uint8_t SBUS_RxBuffer);
void SBUS_getData(void);

#endif /* INC_LIBRARIES_SBUS_H_ */
