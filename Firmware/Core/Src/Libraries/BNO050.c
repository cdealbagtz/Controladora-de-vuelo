/*
 * BNO050.c
 *
 *  Created on: Apr 24, 2025
 *      Author: cdealba
 */

#include "Libraries/BNO050.h"

uint8_t BNO_RxBuffer[255];
BNO_Receive_Status_e BNO_Rx_Status;
BNO_errorHandler_e BNO_ErrorHandler;
BNO_bufffer_Status_e BNO_bufffer_Status;


void BNO_Read(uint8_t Address,uint8_t Size){

}


void BNO_Receive(uint8_t Buffer){
	static uint8_t MsgSize = 0;
	static uint8_t Counter = 0;

	switch (BNO_Rx_Status) {
		case AwaitingMsg:
			if(Buffer == Read_Response || Buffer == Header) BNO_Rx_Status = Buffer;
			break;
		case Read_Response:
			MsgSize = Buffer;
			BNO_Rx_Status = Emptying_Buffer;
			break;
		case Header:
			BNO_ErrorHandler = Buffer;
			BNO_Rx_Status = AwaitingMsg;
			break;
		case Emptying_Buffer:

			if(Counter != MsgSize){
				BNO_RxBuffer[Counter] = Buffer;
				Counter++;
			}
			else{
				Counter = 0;
				MsgSize = 0;
				BNO_bufffer_Status = Ready;
				BNO_Rx_Status = AwaitingMsg;
			}
			break;
		default:
			break;
	}
}
