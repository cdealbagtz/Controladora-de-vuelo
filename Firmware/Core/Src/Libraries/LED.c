/*
 * LED.c
 *
 *  Created on: Feb 26, 2025
 *      Author: cdealba
 */


#include "Libraries/LED.h"

LED_Info_t LED_Info;

void LED_Tasks(void){
	if(LED_Info.R_LED.LED_status){
		switch (LED_Info.R_LED.Sequence) {
			case Static:
				HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, SET);
				break;
			case Blink:
				if(HAL_GPIO_ReadPin(LED_1_GPIO_Port, LED_1_Pin) == SET){

				}
				break;
			default:
				HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, SET);
				break;
		}
	}
	else HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, RESET);

}

