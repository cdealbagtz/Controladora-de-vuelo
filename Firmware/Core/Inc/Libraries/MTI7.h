/*
 * MTI7.h
 *
 *  Created on: May 28, 2026
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_MTI7_H_
#define INC_LIBRARIES_MTI7_H_

#include "main.h"

#define MTI7_RxBuffer_Size 200

typedef enum{
	MTI7_Preamble,
	MTI7_BID,
	MTI7_MID,
	MTI7_LEN,
	MTI7_ID,
	MTI7_Data,
	MTI7_Checksum
}MTI7_RXEnum_e;

typedef struct{
	MTI7_RXEnum_e 	MTI7_RXEnum;

	uint8_t			Buffer0[200];
	uint8_t			Buffer1[200];
	uint8_t			ActualBuffer:1;

	uint8_t 		Buffer0_DataReady:1;
	uint8_t 		Buffer1_DataReady:1;
}MTI7_t;

#endif /* INC_LIBRARIES_MTI7_H_ */
