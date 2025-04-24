/*
 * BNO050.h
 *
 *  Created on: Apr 24, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_BNO050_H_
#define INC_LIBRARIES_BNO050_H_

#include "stm32h7xx.h"

typedef enum{
	AwaitingMsg = 0x00,
	Read_Response = 0xBB,
	Header = 0xEE,
	Emptying_Buffer = 0xFF
}BNO_Receive_Status_e;

typedef enum{
	WriteSuccess = 0x01,
	READ_FAIL = 0x02,
	WRITE_FAIL = 0x03,
	REGMAP_INVALID_ADDRESS = 0x04,
	REGMAP_WRITE_DISABLED = 0x05,
	WRONG_START_BYTE = 0x06,
	BUS_OVER_RUN_ERROR = 0x07,
	MAX_LENGTH_ERROR = 0x08,
	MIN_LENGTH_ERROR = 0x09,
	RECEIVE_CHARACTER_TIMEOUT = 0x0A

}BNO_errorHandler_e;

typedef enum{
	Awaiting = 0x00,
	Ready = 1,

}BNO_bufffer_Status_e;

#endif /* INC_LIBRARIES_BNO050_H_ */
