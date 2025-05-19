/*
 * SYSTEM.h
 *
 *  Created on: May 14, 2025
 *      Author: cdealba
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "stm32h7b0xx.h"


typedef struct{
	char FWVersion[5];
	uint8_t ResetCounter;
}SystemConfig_t;

SystemConfig_t SystemConfig = {
		.FWVersion = "0.0.1"
};

#endif /* INC_CONFIG_H_ */
