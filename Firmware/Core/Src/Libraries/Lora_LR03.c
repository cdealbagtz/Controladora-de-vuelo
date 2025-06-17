/*
 * Lora_LR03.c
 *
 *  Created on: Jun 16, 2025
 *      Author: cdealba
 */


#include "Libraries/Lora_LR03.h"

uint8_t LR03_RxBuffer[256] = {0};

uint8_t Test_1;

void LR03_AT(void){

	uint8_t AT_Command[5] = {"+++"};
	AT_Command[3]= 0x0D;
	AT_Command[4]= 0x0A;
	HAL_UART_Transmit_IT(&huart6, AT_Command, 5);
}

void LR03_Level(uint8_t Level){
	uint8_t Level_Command[11] = {"AT+LEVEL"};
	if(Level > 7) Level = 7;
	if(Level < 1) Level = 1;
	Level_Command[8] = Level + 48;
	Level_Command[9] = 0x0D;
	Level_Command[10] = 0x0A;
	HAL_UART_Transmit_IT(&huart6, Level_Command, 11);
}

void LR03_Receive(uint8_t Data){
	static uint8_t DataPointer = 0;
	LR03_RxBuffer[DataPointer] = Data;
	DataPointer++;
}

void LR03_StateMachine(void){

	switch (Test_1) {
		case 1:
			LR03_AT();
			break;
		case 2:
			LR03_Level(7);
			break;
		default:
			break;
	}
	Test_1 = 0;
}
