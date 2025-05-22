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

uint8_t BNO_ComsCounter = 0;
uint8_t BNO_DelayCounter = 0;

BNO_CurrentState_e BNO_CurrentState = Init;

IMU_t IMU;

void bno055_delay(uint8_t time) {
	BNO_DelayCounter += time;
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

HAL_StatusTypeDef BNO_Write_MB(uint8_t Address,uint8_t Size, uint8_t *pData){
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

void BNO_Page0Adress(void){
	switch (BNO_RxBuffer[0]) {
		case BNO055_CHIP_ID:
			if(BNO_RxBuffer[1] != 160) return;
			IMU.ID = BNO_RxBuffer[1];
			IMU.ACC.ID = BNO_RxBuffer[2];
			IMU.MAG.ID = BNO_RxBuffer[3];
			IMU.GYR.ID = BNO_RxBuffer[4];
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_PAGE_ID:
			IMU.Page = BNO_RxBuffer[1];
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_UNIT_SEL:
			IMU.Unit_Select = BNO_RxBuffer[1];
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_SYS_STATUS:
			IMU.System_Status = BNO_RxBuffer[1];
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_OPR_MODE:
			IMU.Op_Mode = BNO_RxBuffer[1];
			BNO_ErrorHandler = ReadSucces;
			break;
		case BNO055_CALIB_STAT:
			IMU.CalSatus.Full = BNO_RxBuffer[1];

			IMU.CalSatus.Sys = (BNO_RxBuffer[1] >> 6) & 0x03;
			IMU.CalSatus.Gyr = (BNO_RxBuffer[1] >> 4) & 0x03;
			IMU.CalSatus.Acc = (BNO_RxBuffer[1] >> 2) & 0x03;
			IMU.CalSatus.Mag = BNO_RxBuffer[1] & 0x03;

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
		case BNO055_ACC_DATA_X_LSB:

			IMU.ACC.x = ((double)((int16_t)((BNO_RxBuffer[2] << 8) | BNO_RxBuffer[1])))/100;
			IMU.ACC.y = ((double)((int16_t)((BNO_RxBuffer[4] << 8) | BNO_RxBuffer[3])))/100;
			IMU.ACC.z = ((double)((int16_t)((BNO_RxBuffer[6] << 8) | BNO_RxBuffer[5])))/100;

			IMU.MAG.x = ((double)((int16_t)((BNO_RxBuffer[8] << 8) | BNO_RxBuffer[7])))/16;
			IMU.MAG.y = ((double)((int16_t)((BNO_RxBuffer[10] << 8) | BNO_RxBuffer[9])))/16;
			IMU.MAG.z = ((double)((int16_t)((BNO_RxBuffer[12] << 8) | BNO_RxBuffer[11])))/16;

			IMU.GYR.x = ((double)((int16_t)((BNO_RxBuffer[14] << 8) | BNO_RxBuffer[13])))/16;
			IMU.GYR.y = -((double)((int16_t)((BNO_RxBuffer[16] << 8) | BNO_RxBuffer[15])))/16;
			IMU.GYR.z = -((double)((int16_t)((BNO_RxBuffer[18] << 8) | BNO_RxBuffer[17])))/16;

			IMU.Heading = ((double)((int16_t)((BNO_RxBuffer[20] << 8) | BNO_RxBuffer[19])))/16;
			IMU.Pitch = ((double)((int16_t)((BNO_RxBuffer[22] << 8) | BNO_RxBuffer[21])))/16;
			IMU.Roll = ((double)((int16_t)((BNO_RxBuffer[24] << 8) | BNO_RxBuffer[23])))/16;

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
			BNO_ErrorHandler = ReadSucces;
			break;

		default:
			break;
	}
}

void BNO_EmptyingBuffer(void){
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
}



void BNO_SelectPage(uint8_t Page){
	BNO_Write(BNO055_PAGE_ID, Page);
	BNO_Read(BNO055_PAGE_ID, 1);
}

void BNO_HWReset(void){
	static uint8_t ResetFlag = 0;
	switch (ResetFlag) {
		case 0:
			HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, RESET);
			bno055_delay(1);
			ResetFlag = 1;
			IMU.ID = 0x00;
			break;
		case 1:
			HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, SET);
			bno055_delay(70);
			ResetFlag = 0;
			BNO_CurrentState = Configuration;
			break;
		default:
			break;
	}
}

void BNO_SWReset(void){
	BNO_Write(BNO055_SYS_TRIGGER,0x20);
	bno055_delay(70);
	IMU.ID = 0x00;
	BNO_CurrentState = Configuration;
}

void bno055_setOperationMode(bno055_opmode_t mode) {
  BNO_Write(BNO055_OPR_MODE, mode);
  if (mode == BNO055_OPERATION_MODE_CONFIG) {
    bno055_delay(5);
  } else {
    bno055_delay(3);
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

void BNO_SetCalibrationData(void){
	uint8_t buffer[22];

	  memcpy(&IMU.Calibration_Data.offset.accel, buffer, 6);
	  memcpy(&IMU.Calibration_Data.offset.mag, buffer + 6, 6);
	  memcpy(&IMU.Calibration_Data.offset.gyro, buffer + 12, 6);
	  memcpy(&IMU.Calibration_Data.radius.accel, buffer + 18, 2);
	  memcpy(&IMU.Calibration_Data.radius.mag, buffer + 20, 2);

	BNO_Write_MB(BNO055_ACC_OFFSET_X_LSB, 22, buffer);
}

void BNO_Init(void){

	HAL_GPIO_WritePin(IMU_RST_GPIO_Port, IMU_RST_Pin, SET);
	HAL_GPIO_WritePin(IMU_BOOT_GPIO_Port, IMU_BOOT_Pin, SET);

	bno055_delay(100);

	BNO_CurrentState = Reset;
}

void BNO_Config(void){
	static uint8_t ConfigFlag = 0;

	switch (ConfigFlag) {
		case 0:
			BNO_Write(BNO055_SYS_TRIGGER,0x00);
			ConfigFlag = 1;
			break;
		case 1:
			bno055_setOperationModeConfig();
			ConfigFlag = 2;
			break;
		case 2:
			BNO_Read(BNO055_OPR_MODE, 1);
			if(IMU.Op_Mode == BNO055_OPERATION_MODE_CONFIG){
				ConfigFlag = 3;
			}
			else ConfigFlag = 1;
			break;
		case 3:
			BNO_Read(BNO055_CHIP_ID, 4);

			if(IMU.ID == BNO055_ID){
				BNO_CurrentState = Calibration;
			}
			ConfigFlag = 0;
			break;
		default:
			break;
	}

}

void BNO_CalibrationStatus(void){
	static uint8_t CalibrationFlag = 0;
	static uint8_t Calibrated = 1;

	switch (CalibrationFlag) {
		case 0:
			BNO_Write(BNO055_SYS_TRIGGER,0x00);
			CalibrationFlag = 1;
			break;
		case 1:
			bno055_setOperationModeNDOF();
			CalibrationFlag = 2;
			break;
		case 2:
			BNO_Read(BNO055_OPR_MODE, 1);
			if(IMU.Op_Mode == BNO055_OPERATION_MODE_NDOF){
				if(Calibrated){
					CalibrationFlag = 0;
					BNO_CurrentState = Operation;
				}
				else CalibrationFlag = 3;
			}
			else CalibrationFlag = 1;
			break;
		case 3:
			BNO_Read(BNO055_CALIB_STAT, 1);
			if(IMU.CalSatus.Full == 0xFF){
				CalibrationFlag = 4;
			}
			break;
		case 4:
			bno055_setOperationModeConfig();
			CalibrationFlag = 5;
			break;
		case 5:
			BNO_Read(BNO055_OPR_MODE, 1);
			if(IMU.Op_Mode == BNO055_OPERATION_MODE_CONFIG){
				CalibrationFlag = 6;

			}
			else CalibrationFlag = 4;
			break;
		case 6:
			BNO_GetCalibrationData();
			Calibrated = 1;
			CalibrationFlag = 1;
			break;
		case 7:
			BNO_SetCalibrationData();
			CalibrationFlag = 1;
			break;
		default:
			CalibrationFlag = 0;
			break;
	}
	/*
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
	 */

	/*
    if(IMU.Op_Mode == BNO055_OPERATION_MODE_NDOF){
    	BNO_CurrentState = Operation;
    }
    else bno055_setOperationModeNDOF();
    */

}

void BNO_GetData(void){
	BNO_Read(BNO055_ACC_DATA_X_LSB,24);
}

void BNO_Receive(uint8_t Buffer){
	static uint8_t MsgSize = 0;
	static uint8_t Counter = 0;

	BNO_ComsCounter = 0;

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
			memset(BNO_RxBuffer, 0, sizeof(BNO_RxBuffer));
			break;
		case Emptying_Buffer:

			BNO_RxBuffer[Counter + 1] = Buffer;
			Counter++;
			if(Counter == MsgSize){
				Counter = 0;
				MsgSize = 0;
				BNO_Rx_Status = AwaitingMsg;
				BNO_ErrorHandler = ReadSucces;
				BNO_EmptyingBuffer();
			}

			break;
		default:
			break;
	}
}

void BNO_FaultManager(void){
	if(BNO_ComsCounter > 3){
		Reset_UART(&huart3);
		BNO_ComsCounter = 0;
		BNO_Rx_Status = AwaitingMsg;
		HAL_UART_Receive_DMA(&huart3, &BNO_BufferByte,1);
		memset(BNO_RxBuffer, 0, sizeof(BNO_RxBuffer));
	}
}

void BNO_Tasks(void){
	if(BNO_DelayCounter == 0){
		switch (BNO_CurrentState) {
			case Init:
				BNO_Init();
				break;
			case Reset:
				BNO_SWReset();
			break;
			case Configuration:
				BNO_Config();
				break;
			case Calibration:
				BNO_CalibrationStatus();
				break;
			case Operation:
				BNO_GetData();
				break;

			default:
				break;
		}
		++BNO_ComsCounter;
	}
	else{
		--BNO_DelayCounter;
	}
	BNO_FaultManager();
}


