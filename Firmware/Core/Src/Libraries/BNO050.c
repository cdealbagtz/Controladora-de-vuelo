/*
 * BNO050.c
 *
 *  Created on: Apr 24, 2025
 *      Author: cdealba
 */

#include "Libraries/BNO050.h"

uint8_t BNO_RxBuffer[255];
uint8_t BNO_BufferByte = 0x00;
BNO_Receive_Status_e BNO_Rx_Status;
BNO_errorHandler_e BNO_ErrorHandler;
BNO_bufffer_Status_e BNO_bufferStatus;


IMU_t IMU;


void BNO_Page0Adress(void){
	switch (BNO_RxBuffer[0]) {
		case BNO055_CHIP_ID:
			IMU.ID = BNO_RxBuffer[1];
			IMU.ACC.ID = BNO_RxBuffer[2];
			IMU.MAG.ID = BNO_RxBuffer[3];
			IMU.GYR.ID = BNO_RxBuffer[4];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_PAGE_ID:
			IMU.Page = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		default:
			break;
	}
}

void BNO_Page1Adress(void){
	switch (BNO_RxBuffer[0]) {
		case BNO055_PAGE_ID:
			IMU.Page = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;

		default:
			break;
	}
}

void BNO_EmptyingBuffer(void){
	static uint64_t BusyTime;
	static uint8_t BusyFlag = 0;

	if(BNO_bufferStatus == Ready){
		switch (IMU.Page) {
			case 0:
				BNO_Page0Adress();
				break;
			case 1:
				BNO_Page1Adress();
				break;
			default:
				break;
		}
		memset(BNO_RxBuffer, 0, sizeof(BNO_RxBuffer));
		BusyFlag = 0;
	}

	if(BNO_bufferStatus ==  Busy && !BusyFlag){
		BusyFlag = 1;
		BusyTime = TimeOn_Counter;
	}
	if((TimeOn_Counter - BusyTime)>100){
		BNO_bufferStatus = Awaiting;
		BNO_ErrorHandler = READ_FAIL;
		BusyFlag = 0;
	}

}

HAL_StatusTypeDef BNO_Read(uint8_t Address,uint8_t Size){
	uint8_t ReadCommand[4] = {0xAA, 0x01, Address, Size};
	HAL_StatusTypeDef uartError;

	BNO_EmptyingBuffer();

	if(BNO_bufferStatus == Awaiting){
		BNO_RxBuffer[0] = Address;
		BNO_bufferStatus = Busy;
	}
	else return uartError = HAL_BUSY;

	uartError = HAL_UART_Transmit(&huart3, ReadCommand, 4,100);
	return uartError;
}

HAL_StatusTypeDef BNO_Write(uint8_t Address,uint8_t Size, uint8_t *pData){
	uint8_t WriteCommand[255];
	HAL_StatusTypeDef uartError;

	WriteCommand[0] = 0xAA;
	WriteCommand[1] = 0x00;
	WriteCommand[2] = Address;
	WriteCommand[3] = Size;

	for (uint8_t n = 0; n < Size; ++n) {
		WriteCommand[n + 4] = pData[n];
	}

	uartError = HAL_UART_Transmit(&huart3, WriteCommand, (Size + 4),100);
	return uartError;
}

void BNO_SelectPage(uint8_t Page){
	BNO_Write(BNO055_PAGE_ID, 1, &Page);
	BNO_Read(BNO055_PAGE_ID, 1);
}

void BNO_Reset(void){
	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, SET);
	HAL_Delay(700);
}

void BNO_Init(void){
	HAL_UART_Receive_DMA(&huart3, &BNO_BufferByte,1);
	HAL_GPIO_WritePin(IMU_BOOT_GPIO_Port, IMU_BOOT_Pin, SET);
	BNO_Reset();
	BNO_Read(BNO055_CHIP_ID,4);
}

void BNO_FaultHandler(void){
	switch (BNO_ErrorHandler) {
		case READ_FAIL:

			break;
		default:
			break;
	}
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

			BNO_RxBuffer[Counter + 1] = Buffer;
			Counter++;
			if(Counter == MsgSize){
				Counter = 0;
				MsgSize = 0;
				BNO_bufferStatus = Ready;
				BNO_Rx_Status = AwaitingMsg;
				BNO_ErrorHandler = ReadSucces;
			}

			break;
		default:
			break;
	}
	BNO_EmptyingBuffer();
}


