/*
 * MTI7.c
 *
 *  Created on: May 28, 2026
 *      Author: cdealba
 */

#include "Libraries/MTI7.h"

MTI7_t MTI7 = {
		.Port = &huart6
};

void MTI7_init(void){
	 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer0,MTI7_RxBuffer_Size);
}

void MTI7_Tasks(void){
	if(MTI7.Buffer0_DataReady){
		for(uint8_t n = 0; n<MTI7_RxBuffer_Size;n++){
			MTI7_msgProcessing(MTI7.Buffer0[n],&MTI7);
		}
		MTI7.Buffer0_DataReady = 0;
	}
	if(MTI7.Buffer1_DataReady){
		for(uint8_t n = 0; n<MTI7_RxBuffer_Size;n++){
			MTI7_msgProcessing(MTI7.Buffer1[n],&MTI7);
		}
		MTI7.Buffer1_DataReady = 0;
	}
	if(MTI7.UartErrorCount < 200){
		MTI7.UartErrorCount++;
	}
	else{
		MTI7_init();
	}

}

void MTI7_RxIRQ(void){
	switch (MTI7.ActualBuffer) {
		case 0:
			 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer1,MTI7_RxBuffer_Size);
			 MTI7.Buffer0_DataReady = 1;
			 MTI7.ActualBuffer = 1;
			break;
		case 1:
			 HAL_UART_Receive_IT(MTI7.Port, MTI7.Buffer0,MTI7_RxBuffer_Size);
			 MTI7.Buffer1_DataReady = 1;
			 MTI7.ActualBuffer = 0;
			break;
		default:
			break;
	}
}

void MTI7_DataBuild(uint8_t *Buffer){
	float_u Processing_Buffer;

	if((Buffer[4]==0x20)&&(Buffer[5]==0x30)){
		Processing_Buffer.bytes[3] = Buffer[7];
		Processing_Buffer.bytes[2] = Buffer[8];
		Processing_Buffer.bytes[1] = Buffer[9];
		Processing_Buffer.bytes[0] = Buffer[10];

		MTI7.INS.roll = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[11];
		Processing_Buffer.bytes[2] = Buffer[12];
		Processing_Buffer.bytes[1] = Buffer[13];
		Processing_Buffer.bytes[0] = Buffer[14];

		MTI7.INS.pitch = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[15];
		Processing_Buffer.bytes[2] = Buffer[16];
		Processing_Buffer.bytes[1] = Buffer[17];
		Processing_Buffer.bytes[0] = Buffer[18];

		MTI7.INS.yaw = Processing_Buffer.value;
	}

	if((Buffer[19]==0x40)&&(Buffer[20]==0x20)){
		Processing_Buffer.bytes[3] = Buffer[22];
		Processing_Buffer.bytes[2] = Buffer[23];
		Processing_Buffer.bytes[1] = Buffer[24];
		Processing_Buffer.bytes[0] = Buffer[25];

		MTI7.INS.Ax = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[26];
		Processing_Buffer.bytes[2] = Buffer[27];
		Processing_Buffer.bytes[1] = Buffer[28];
		Processing_Buffer.bytes[0] = Buffer[29];

		MTI7.INS.Ay = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[30];
		Processing_Buffer.bytes[2] = Buffer[31];
		Processing_Buffer.bytes[1] = Buffer[32];
		Processing_Buffer.bytes[0] = Buffer[33];

		MTI7.INS.Az = Processing_Buffer.value;
	}

	if((Buffer[34]==0x80)&&(Buffer[35]==0x20)){
		Processing_Buffer.bytes[3] = Buffer[37];
		Processing_Buffer.bytes[2] = Buffer[38];
		Processing_Buffer.bytes[1] = Buffer[39];
		Processing_Buffer.bytes[0] = Buffer[40];

		MTI7.INS.p = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[41];
		Processing_Buffer.bytes[2] = Buffer[42];
		Processing_Buffer.bytes[1] = Buffer[43];
		Processing_Buffer.bytes[0] = Buffer[44];

		MTI7.INS.q = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[45];
		Processing_Buffer.bytes[2] = Buffer[46];
		Processing_Buffer.bytes[1] = Buffer[47];
		Processing_Buffer.bytes[0] = Buffer[48];

		MTI7.INS.r = Processing_Buffer.value;
	}

	if((Buffer[49]==0x50)&&(Buffer[50]==0x40)){
		Processing_Buffer.bytes[3] = Buffer[52];
		Processing_Buffer.bytes[2] = Buffer[53];
		Processing_Buffer.bytes[1] = Buffer[54];
		Processing_Buffer.bytes[0] = Buffer[55];

		MTI7.INS.Lat = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[56];
		Processing_Buffer.bytes[2] = Buffer[57];
		Processing_Buffer.bytes[1] = Buffer[58];
		Processing_Buffer.bytes[0] = Buffer[59];

		MTI7.INS.Lon = Processing_Buffer.value;
	}

	if((Buffer[60]==0x50)&&(Buffer[61]==0x20)){
		Processing_Buffer.bytes[3] = Buffer[63];
		Processing_Buffer.bytes[2] = Buffer[64];
		Processing_Buffer.bytes[1] = Buffer[65];
		Processing_Buffer.bytes[0] = Buffer[66];

		MTI7.INS.GPSAlt = Processing_Buffer.value;
	}

	if((Buffer[67]==0xD0)&&(Buffer[68]==0x10)){
		Processing_Buffer.bytes[3] = Buffer[70];
		Processing_Buffer.bytes[2] = Buffer[71];
		Processing_Buffer.bytes[1] = Buffer[72];
		Processing_Buffer.bytes[0] = Buffer[73];

		MTI7.INS.velx = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[74];
		Processing_Buffer.bytes[2] = Buffer[75];
		Processing_Buffer.bytes[1] = Buffer[76];
		Processing_Buffer.bytes[0] = Buffer[77];

		MTI7.INS.vely = Processing_Buffer.value;

		Processing_Buffer.bytes[3] = Buffer[78];
		Processing_Buffer.bytes[2] = Buffer[79];
		Processing_Buffer.bytes[1] = Buffer[80];
		Processing_Buffer.bytes[0] = Buffer[81];

		MTI7.INS.velz = Processing_Buffer.value;
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
		case MTI7_MID:
			MTI7->msg[MTI7->msgCount] = byte;
			MTI7->msgID = byte;
			MTI7->MTI7_RXEnum = MTI7_LEN;
			MTI7->msgCount++;
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

			if(MTI7->msgCount == 5){
				MTI7->MTI7_RXEnum = MTI7_ID;
				MTI7->msgLen = (MTI7->msg[5]<<8)|(MTI7->msg[4]);
			}
			MTI7->msgCount++;
			break;
		case MTI7_ID:
			MTI7->msg[MTI7->msgCount] = byte;
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
			if(MTI7->msgID == 0x36){
				MTI7_DataBuild(&MTI7->msg[0]);
				MTI7->UartErrorCount = 0;
			}

			MTI7->msgCount = 0;
			MTI7->msgID = 0;
			MTI7->MTI7_RXEnum = MTI7_Preamble;
			break;
		default:
			MTI7->MTI7_RXEnum = MTI7_Preamble;
			break;
	}
}


