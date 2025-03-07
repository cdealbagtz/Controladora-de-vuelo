/*
 * LED.c
 *
 *  Created on: Feb 26, 2025
 *      Author: cdealba
 */


#include "Libraries/LED.h"

LED_Info_t LED_Info;

void LED_Init(void){
	LED_Info.R_LED.LED_GPIO_GPIOx = LED_1_GPIO_Port;
	LED_Info.R_LED.LED_GPIO_Pin   = LED_1_Pin;

	LED_Info.B_LED1.LED_GPIO_GPIOx = LED_2_GPIO_Port;
	LED_Info.B_LED1.LED_GPIO_Pin   = LED_2_Pin;

	LED_Info.B_LED2.LED_GPIO_GPIOx = LED_3_GPIO_Port;
	LED_Info.B_LED2.LED_GPIO_Pin   = LED_3_Pin;

	LED_Info.B_LED3.LED_GPIO_GPIOx = LED_4_GPIO_Port;
	LED_Info.B_LED3.LED_GPIO_Pin   = LED_4_Pin;

	LED_Info.G_LED.LED_GPIO_GPIOx = LED_5_GPIO_Port;
	LED_Info.G_LED.LED_GPIO_Pin   = LED_5_Pin;
}

LED_config_t LED_blink(LED_config_t LED){

		switch (LED.Blink_status) {
			case 0x00:
				if(LED.count >= LED.Time_Off){
					HAL_GPIO_WritePin(LED.LED_GPIO_GPIOx, LED.LED_GPIO_Pin, SET);
					LED.Blink_status = 1;
					LED.count = 0;
				}
				break;
			case 0x01:
				if(LED.count >= LED.Time_On){
					HAL_GPIO_WritePin(LED.LED_GPIO_GPIOx, LED.LED_GPIO_Pin, RESET);
					LED.Blink_status = 0;
					LED.count = 0;
				}
				break;
			default:
				LED.Blink_status = 0;
				break;
		}

	LED.count++;
	return LED;
}

LED_config_t LED_Rutine(LED_config_t LED){
	if(LED.LED_status == 1){
		switch (LED.Sequence) {
			case Static:
				HAL_GPIO_WritePin(LED.LED_GPIO_GPIOx, LED.LED_GPIO_Pin, SET);
				break;
			case Blink:
				LED = LED_blink(LED);
				break;
			default:
				break;
		}
	}
	else{
		HAL_GPIO_WritePin(LED.LED_GPIO_GPIOx, LED.LED_GPIO_Pin, RESET);
		LED.count = 0;
	}
	return LED;
}

void LED_Tasks (void){
	LED_Info.R_LED = LED_Rutine(LED_Info.R_LED);
	LED_Info.B_LED1 = LED_Rutine(LED_Info.B_LED1);
	LED_Info.B_LED2 = LED_Rutine(LED_Info.B_LED2);
	LED_Info.B_LED3 = LED_Rutine(LED_Info.B_LED3);
	LED_Info.G_LED = LED_Rutine(LED_Info.G_LED);
}

