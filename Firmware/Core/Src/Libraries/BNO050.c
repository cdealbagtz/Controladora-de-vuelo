/*
 * BNO050.c
 *
 *  Created on: Apr 24, 2025
 *      Author: cdealba
 */

#include "Libraries/BNO050.h"
#include "Libraries/LED.h"

uint8_t BNO_RxBuffer[255];
uint8_t BNO_BufferByte = 0x00;
BNO_Receive_Status_e BNO_Rx_Status;
BNO_errorHandler_e BNO_ErrorHandler;
BNO_bufffer_Status_e BNO_bufferStatus;
uint8_t CalibrationFlag;

IMU_t IMU;

void bno055_delay(int time) {
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}

HAL_StatusTypeDef BNO_Read(uint8_t Address,uint8_t Size){
	uint8_t ReadCommand[4] = {0xAA, 0x01, Address, Size};
	HAL_StatusTypeDef uartError;

	BNO_RxBuffer[0] = Address;

	uartError = HAL_UART_Transmit(&huart3, ReadCommand, 4,100);
	return uartError;
}

HAL_StatusTypeDef BNO_Write(uint8_t Address,uint8_t Data){
	uint8_t WriteCommand[5];
	HAL_StatusTypeDef uartError;

	WriteCommand[0] = 0xAA;
	WriteCommand[1] = 0x00;
	WriteCommand[2] = Address;
	WriteCommand[3] = 1;
	WriteCommand[4] = Data;

	uartError = HAL_UART_Transmit(&huart3, WriteCommand,5,100);
	return uartError;
}

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
		case BNO055_UNIT_SEL:
			IMU.Unit_Select = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_SYS_STATUS:
			IMU.System_Status = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_OPR_MODE:
			IMU.Op_Mode = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_CALIB_STAT:
			IMU.SysCalibration = BNO_RxBuffer[1];
			BNO_bufferStatus = Awaiting;
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_ACC_OFFSET_X_LSB:
			IMU.Calibration_Data.offset.accel.x = (int16_t)((BNO_RxBuffer[2] << 8) | BNO_RxBuffer[1]);
			IMU.Calibration_Data.offset.accel.y = (int16_t)((BNO_RxBuffer[4] << 8) | BNO_RxBuffer[3]);
			IMU.Calibration_Data.offset.accel.z = (int16_t)((BNO_RxBuffer[6] << 8) | BNO_RxBuffer[5]);

			IMU.Calibration_Data.offset.mag.x = (int16_t)((BNO_RxBuffer[8] << 8) | BNO_RxBuffer[7]);
			IMU.Calibration_Data.offset.mag.y = (int16_t)((BNO_RxBuffer[10] << 8) | BNO_RxBuffer[9]);
			IMU.Calibration_Data.offset.mag.z = (int16_t)((BNO_RxBuffer[12] << 8) | BNO_RxBuffer[11]);

			IMU.Calibration_Data.offset.gyro.x = (int16_t)((BNO_RxBuffer[14] << 8) | BNO_RxBuffer[13]);
			IMU.Calibration_Data.offset.gyro.y = (int16_t)((BNO_RxBuffer[16] << 8) | BNO_RxBuffer[15]);
			IMU.Calibration_Data.offset.gyro.z = (int16_t)((BNO_RxBuffer[18] << 8) | BNO_RxBuffer[17]);

			IMU.Calibration_Data.radius.accel = (uint16_t)((BNO_RxBuffer[20] << 8) | BNO_RxBuffer[19]);
			IMU.Calibration_Data.radius.mag   = (uint16_t)((BNO_RxBuffer[22] << 8) | BNO_RxBuffer[21]);
			break;
		case BNO055_EUL_HEADING_LSB:


			IMU.Heading = ((double)((int16_t)((BNO_RxBuffer[2] << 8) | BNO_RxBuffer[1])))/16;

			IMU.Roll = ((double)((int16_t)((BNO_RxBuffer[4] << 8) | BNO_RxBuffer[3])))/16;

			IMU.Pitch = ((double)((int16_t)((BNO_RxBuffer[6] << 8) | BNO_RxBuffer[5])))/16;

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



void BNO_SelectPage(uint8_t Page){
	BNO_Write(BNO055_PAGE_ID, Page);
	BNO_Read(BNO055_PAGE_ID, 1);
}

void BNO_HWReset(void){
	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, SET);
	bno055_delay(700);
}

void BNO_SWReset(void){
	BNO_Write(BNO055_SYS_TRIGGER,0x20);
	bno055_delay(700);
}

void bno055_setOperationMode(bno055_opmode_t mode) {
  BNO_Write(BNO055_OPR_MODE, mode);
  if (mode == BNO055_OPERATION_MODE_CONFIG) {
    bno055_delay(25);
  } else {
    bno055_delay(10);
  }
}

void bno055_setOperationModeConfig() {
  bno055_setOperationMode(BNO055_OPERATION_MODE_CONFIG);
}

void bno055_setOperationModeNDOF() {
  bno055_setOperationMode(BNO055_OPERATION_MODE_NDOF);
}

void BNO_GetCalibrationData(void){
	BNO_Read(BNO055_ACC_OFFSET_X_LSB, 22);
}

void BNO_CalibrationStatus(void){
	if(IMU.SysCalibration != 0xFF){
		LED_Info.B_LED1.LED_status = SET;
		LED_Info.B_LED1.Sequence = Blink;
		LED_Info.B_LED1.Time_On = 200;
		LED_Info.B_LED1.Time_Off = 200;

		BNO_Read(BNO055_CALIB_STAT,1);
	}
	else{
		LED_Info.B_LED1.LED_status = RESET;
		BNO_GetCalibrationData();
	}
	bno055_delay(10);
}

void BNO_Init(void){
	HAL_UART_Receive_DMA(&huart3, &BNO_BufferByte,1);
	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, SET);
	HAL_GPIO_WritePin(IMU_BOOT_GPIO_Port, IMU_BOOT_Pin, SET);
	BNO_HWReset();
	BNO_Read(BNO055_CHIP_ID, 4);
	BNO_Write(BNO055_SYS_TRIGGER, 0x00);

	//while(CalibrationFlag != 0x01){
	//	BNO_CalibrationStatus();
	//}

	bno055_setOperationModeNDOF();
	BNO_Read(BNO055_OPR_MODE, 1);
}

void BNO_GetAtt(void){
	BNO_Read(BNO055_EUL_HEADING_LSB, 6);
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
				BNO_EmptyingBuffer();
			}

			break;
		default:
			break;
	}
}


