/*
 * MTI7.h
 *
 *  Created on: May 28, 2026
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_MTI7_H_
#define INC_LIBRARIES_MTI7_H_

#include "main.h"
#include "usart.h"

#define MTI7_RxBuffer_Size 	50
#define MTI7_msgMaxSize		512

typedef enum{
	MTI7_Preamble,
	MTI7_BID,
	MTI7_MID,
	MTI7_LEN,
	MTI7_EXTLEN,
	MTI7_ID,
	MTI7_Data,
	MTI7_Checksum
}MTI7_RXEnum_e;

typedef struct{
	UART_HandleTypeDef *Port;
	MTI7_RXEnum_e 	MTI7_RXEnum;

	uint8_t			Buffer0[MTI7_RxBuffer_Size];
	uint8_t			Buffer1[MTI7_RxBuffer_Size];

	uint8_t			msg[MTI7_msgMaxSize];
	uint8_t			ActualBuffer:1;
	uint8_t 		msgLen;
	uint8_t 		msgID;
	uint8_t			msgCount;

	uint8_t 		Buffer0_DataReady:1;
	uint8_t 		Buffer1_DataReady:1;
}MTI7_t;

extern MTI7_t MTI7;

void MTI7_init(void);
void MTI7_Tasks(void);
void MTI7_RxIRQ(void);
void MTI7_msgProcessing(uint8_t byte, MTI7_t *MTI7);

#endif /* INC_LIBRARIES_MTI7_H_ */
