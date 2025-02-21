/*
 * SBUS.c
 *
 *  Created on: Feb 21, 2025
 *      Author: cdealba
 */


#include "Libraries/SBUS.h"
#include "Libraries/PWM.h"

extern PWM_Output_t PWM_Output;

uint8_t SBUS_UART_Rx[25];
uint8_t SBUS_RxBuffer = 0x00;

Radio_input_t Radio_input = {
		.Canal_1  = 1500,
		.Canal_2  = 1500,
		.Canal_3  = 1500,
		.Canal_4  = 1500,
		.Canal_5  = 1500,
		.Canal_6  = 1500,
		.Canal_7  = 1500,
		.Canal_8  = 1500,
		.Canal_9  = 1500,
		.Canal_10 = 1500,
		.Canal_11 = 1500,
		.Canal_12 = 1500,
		.Canal_13 = 1500,
		.Canal_14 = 1500,
		.Canal_15 = 1500,
		.Canal_16 = 1500,

		.Interruptor_1 = 0,
		.Interruptor_2 = 0,
		.pkg_lost = 1,
		.fail_safe = 1

};


extern UART_HandleTypeDef huart1;
#define SBUS_UART_handler &huart1


void SBUS_init(void){
	HAL_UART_Receive_DMA(SBUS_UART_handler, &SBUS_RxBuffer, 1);

}

void SBUS_Receive(uint8_t SBUS_RxBuffer){
	static uint8_t Actual_status = 0;
	static uint8_t RxCount;

	switch (Actual_status) {
		case 0:
			if(SBUS_RxBuffer == 0x0F){
				SBUS_UART_Rx[RxCount] = SBUS_RxBuffer;
				Actual_status = 1;
				RxCount++;
			}
			else{
				RxCount = 0;
			}
			break;
		case 1:
			SBUS_UART_Rx[RxCount] = SBUS_RxBuffer;
			RxCount++;
			if(RxCount == 24){
				RxCount = 0;
				Actual_status = 0;
				SBUS_getData();
			}
			break;
		default:
			Actual_status = 0;
			break;
	}
}

void Temp_BypassFunct (void){
	PWM_Output.Canal_1 = Radio_input.Canal_1;
	PWM_Output.Canal_2 = Radio_input.Canal_2;
	PWM_Output.Canal_3 = Radio_input.Canal_3;
	PWM_Output.Canal_4 = Radio_input.Canal_4;
	PWM_Output.Canal_5 = Radio_input.Canal_5;
	PWM_Output.Canal_6 = Radio_input.Canal_6;
	PWM_Output.Canal_7 = Radio_input.Canal_7;
	PWM_Output.Canal_8 = Radio_input.Canal_8;
	PWM_Output.Canal_9 = Radio_input.Canal_9;
	PWM_Output.Canal_10 = Radio_input.Canal_10;

	PWM_Assing();
}

void SBUS_getData(void){
	//Se hace el corrimiento de bits para dar la estructura de los 16 canales, donde cada canal esta compuesto por 11 bits
	//pero cada paquete es de 1 byte. Para cada canal se utilizan los 8 bits (O los que correspondan) de el byte 1
	//(O el que corresponda) como los bits menos significativos del canal 1, y los tres bits faltantes se toman los 3 bits
	// menos significativos (O los que correspondan) del byte 2 y se colocan como los 3 bits más significativos del canal 1.
	Radio_input.Canal_1 = ((SBUS_UART_Rx[1] | SBUS_UART_Rx[2] << 8) & 0x07FF);
	Radio_input.Canal_2 = ((SBUS_UART_Rx[2] >> 3 | SBUS_UART_Rx[3] << 5) & 0x07FF);
	Radio_input.Canal_3 = ((SBUS_UART_Rx[3] >> 6 | SBUS_UART_Rx[4] << 2 | SBUS_UART_Rx[5] << 10) & 0x07FF);
	Radio_input.Canal_4 = ((SBUS_UART_Rx[5] >> 1 | SBUS_UART_Rx[6] << 7) & 0x07FF);
	Radio_input.Canal_5 = ((SBUS_UART_Rx[6] >> 4 | SBUS_UART_Rx[7] << 4) & 0x07FF);
	Radio_input.Canal_6 = ((SBUS_UART_Rx[7] >> 7 | SBUS_UART_Rx[8] << 1 | SBUS_UART_Rx[9] << 9) & 0x07FF);
	Radio_input.Canal_7 = ((SBUS_UART_Rx[9] >> 2 | SBUS_UART_Rx[10] << 6) & 0x07FF);
	Radio_input.Canal_8 = ((SBUS_UART_Rx[10] >> 5 | SBUS_UART_Rx[11] << 3) & 0x07FF);
	Radio_input.Canal_9 = ((SBUS_UART_Rx[12] | SBUS_UART_Rx[13] << 8) & 0x07FF);
	Radio_input.Canal_10 = ((SBUS_UART_Rx[13] >> 3 | SBUS_UART_Rx[14] << 5) & 0x07FF);
	Radio_input.Canal_11 = ((SBUS_UART_Rx[14] >> 6 | SBUS_UART_Rx[15] << 2 | SBUS_UART_Rx[16] << 10) & 0x07FF);
	Radio_input.Canal_12 = ((SBUS_UART_Rx[16] >> 1 | SBUS_UART_Rx[17] << 7) & 0x07FF);
	Radio_input.Canal_13 = ((SBUS_UART_Rx[17] >> 4 | SBUS_UART_Rx[18] << 4) & 0x07FF);
	Radio_input.Canal_14 = ((SBUS_UART_Rx[18] >> 7 | SBUS_UART_Rx[19] << 1 | SBUS_UART_Rx[20] << 9) & 0x07FF);
	Radio_input.Canal_15 = ((SBUS_UART_Rx[20] >> 2 | SBUS_UART_Rx[21] << 6) & 0x07FF);
	Radio_input.Canal_16 = ((SBUS_UART_Rx[21] >> 5 | SBUS_UART_Rx[22] << 3) & 0x07FF);

	Radio_input.Canal_1 = (uint16_t)((((float)Radio_input.Canal_1 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_2 = (uint16_t)((((float)Radio_input.Canal_2 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_3 = (uint16_t)((((float)Radio_input.Canal_3 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_4 = (uint16_t)((((float)Radio_input.Canal_4 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_5 = (uint16_t)((((float)Radio_input.Canal_5 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_6 = (uint16_t)((((float)Radio_input.Canal_6 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_7 = (uint16_t)((((float)Radio_input.Canal_7 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_8 = (uint16_t)((((float)Radio_input.Canal_8 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_9 = (uint16_t)((((float)Radio_input.Canal_9 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_10 = (uint16_t)((((float)Radio_input.Canal_10 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_11 = (uint16_t)((((float)Radio_input.Canal_11 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_12 = (uint16_t)((((float)Radio_input.Canal_12 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_13 = (uint16_t)((((float)Radio_input.Canal_13 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_14 = (uint16_t)((((float)Radio_input.Canal_14 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_15 = (uint16_t)((((float)Radio_input.Canal_15 - 352) /1344 ) * 1000) + 1000;
	Radio_input.Canal_16 = (uint16_t)((((float)Radio_input.Canal_16 - 352) /1344 ) * 1000) + 1000;

	//En el caso del byte 24, el primer bit menos significativo corresponde al canal 17, el segundo al canal 18, el tercero a el fail safe y
	//el cuarto a la perdida de paquetes. Los 4 restantes no se utilizan.
	if(SBUS_UART_Rx[23]>=8){
		SBUS_UART_Rx[23]-=8;
		Radio_input.fail_safe = 1;
	}else Radio_input.fail_safe= 0;

	if(SBUS_UART_Rx[23]>=4){
		SBUS_UART_Rx[23]-=4;
		Radio_input.pkg_lost = 1;
	}else Radio_input.pkg_lost=0;

	if(SBUS_UART_Rx[23]>=2){
		SBUS_UART_Rx[23]-=2;
		Radio_input.Interruptor_2=1;
	}else Radio_input.Interruptor_2=0;


	Radio_input.Interruptor_1=SBUS_UART_Rx[23];


	Temp_BypassFunct();

}


