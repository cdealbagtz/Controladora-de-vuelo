/*
 * LED.h
 *
 *  Created on: Feb 26, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_LED_H_
#define INC_LIBRARIES_LED_H_

#include "main.h"
#include "stm32h7xx.h"


typedef enum LED_sequence{
	Static,
	Blink
}LED_sequence_e;

typedef struct LED_config{
	uint8_t LED_status;	//ON == 0x01 Off == 0x00
	LED_sequence_e Sequence;   //Static == 0x00 Blink == 0x01

	uint8_t Time_On;	//Time on in milisecons
	uint8_t Time_Off;   //Time off in milisecons
}LED_config_t;

typedef struct LED_Info{
	LED_config_t R_LED;
	LED_config_t B_LED1;
	LED_config_t B_LED2;
	LED_config_t B_LED3;
	LED_config_t G_LED;
}LED_Info_t;



#endif /* INC_LIBRARIES_LED_H_ */
