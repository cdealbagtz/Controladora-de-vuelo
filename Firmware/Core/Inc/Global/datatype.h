/*
 * datatype.h
 *
 *  Created on: May 16, 2025
 *      Author: cdealba
 */

#ifndef INC_GLOBAL_DATATYPE_H_
#define INC_GLOBAL_DATATYPE_H_

#include "stm32h7b0xx.h"

typedef struct{
	uint8_t Bytes[2];
	uint16_t Inter;
}U16_u;

typedef struct{
	uint8_t Bytes[2];
	int16_t Inter;
}S16_u;

typedef struct{
	uint8_t Bytes[4];
	uint32_t Inter;
}U32_u;

typedef struct{
	uint8_t Bytes[4];
	int32_t Inter;
}S32_u;

typedef struct{
	uint8_t Bytes[4];
	float inter;
}Float_u;

typedef struct{
	uint8_t Bytes[8];
	double inter;
}Double_u;

#endif /* INC_GLOBAL_DATATYPE_H_ */
