/*
 * MTI7.c
 *
 *  Created on: May 28, 2026
 *      Author: cdealba
 */

#include "Libraries/MTI7.h"

MTI7_t MTI7 = {
		.Port = &huart4
};

void MTI7_init(void){
	 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer0,MTI7_RxBuffer_Size);
}

void MTI7_Tasks(void){
	if(MTI7.Buffer0_DataReady){
		for(uint8_t n = 0; n<MTI7_RxBuffer_Size;n++){
			MTI7_msgProcessing(MTI7.Buffer0[n],&MTI7);
		}
	}
	if(MTI7.Buffer1_DataReady){
		for(uint8_t n = 0; n<MTI7_RxBuffer_Size;n++){
			MTI7_msgProcessing(MTI7.Buffer1[n],&MTI7);
		}
	}
}

void MTI7_RxIRQ(void){
	switch (MTI7.ActualBuffer) {
		case 0:
			 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer1,MTI7_RxBuffer_Size);
			 MTI7.Buffer0_DataReady = 1;
			break;
		case 1:
			 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer0,MTI7_RxBuffer_Size);
			 MTI7.Buffer1_DataReady = 1;
			break;
		default:
			break;
	}
}

void MTI7_msgProcessing(uint8_t byte, MTI7_t *MTI7){
	switch (MTI7->MTI7_RXEnum) {
		case MTI7_Preamble:
			if(byte == 0xFA){
				MTI7->MTI7_RXEnum = MTI7_BID;
				MTI7->msg[MTI7->msgCount] = byte;
				MTI7->msgCount++;
			}
			break;
		case MTI7_BID:
			if(byte != 0xFF){
				MTI7->MTI7_RXEnum = MTI7_Preamble;
				MTI7->msgCount = 0;
			}
			else{
				MTI7->MTI7_RXEnum = MTI7_MID;
				MTI7->msg[MTI7->msgCount] = byte;
				MTI7->msgCount++;
			}
			break;
		case MTI7_LEN:
			if(byte == 0xFF){
				MTI7->MTI7_RXEnum = MTI7_EXTLEN;
			}
			else{
				MTI7->MTI7_RXEnum = MTI7_ID;
				MTI7->msgLen = byte;
			}
			MTI7->msg[MTI7->msgCount] = byte;
			MTI7->msgCount++;
			break;
		case MTI7_EXTLEN:

			MTI7->msg[MTI7->msgCount] = byte;

			if(MTI7->msgCount == 4){
				MTI7->MTI7_RXEnum = MTI7_ID;
				MTI7->msgLen = (MTI7->msg[4]<<8)|(MTI7->msg[3]);
			}
			MTI7->msgCount++;
			break;
		case MTI7_ID:
			MTI7->msg[MTI7->msgCount] = byte;
			MTI7->msgID = byte;
			MTI7->MTI7_RXEnum = MTI7_Data;
			MTI7->msgCount++;

			break;
		case MTI7_Data:
			MTI7->msg[MTI7->msgCount] = byte;
			MTI7->msgCount++;

			MTI7->msgLen--;
			if(MTI7->msgLen == 0){
				MTI7->MTI7_RXEnum = MTI7_Checksum;
			}
			break;
		case MTI7_Checksum:

			MTI7->msgCount = 0;
			MTI7->MTI7_RXEnum = MTI7_Preamble;
			break;
		default:
			break;
	}
}
