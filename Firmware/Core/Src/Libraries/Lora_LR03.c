/*
 * Lora_LR03.c
 *
 *  Created on: Jun 16, 2025
 *      Author: cdealba
 */


#include "Libraries/Lora_LR03.h"

uint8_t Test_1;

void LR03_AT(void){

}

void Lora_StateMachine(void){
	if(Test_1 == 1){
		LR03_AT();
		Test_1 = 0;
	}
}
