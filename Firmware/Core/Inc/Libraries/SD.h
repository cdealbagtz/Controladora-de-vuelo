/*
 * SD.h
 *
 *  Created on: Feb 18, 2025
 *      Author: cdealba
 */

#ifndef INC_LIBRARIES_SD_H_
#define INC_LIBRARIES_SD_H_

#include "main.h"
#include "fatfs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Direcciones de memoria en archivo de configuración

#define Config_FWVersion 0x09
#define Config_

extern uint32_t TimeOn_Counter;

typedef struct{
	uint32_t Nmsg;
	uint32_t Time;

	double Ax;
	double Ay;
	double Az;

	double Gx;
	double Gy;
	double Gz;

	double Gfx;
	double Gfy;
	double Gfz;

	double Roll;
	double Pitch;
	double Heading;

	uint16_t Alt;
	int32_t Latitude;
	int32_t Longitude;

	uint16_t ARSP;

	uint32_t Pressure;

	uint8_t SBUS_Flags;
	uint8_t Flight_Mode;

	uint16_t Canal_1; // 1000 a 2000
	uint16_t Canal_2; // 1000 a 2000
	uint16_t Canal_3; // 1000 a 2000
	uint16_t Canal_4; // 1000 a 2000
	uint16_t Canal_5; // 1000 a 2000
	uint16_t Canal_6; // 1000 a 2000
	uint16_t Canal_7; // 1000 a 2000
	uint16_t Canal_8; // 1000 a 2000
	uint16_t Canal_9; // 1000 a 2000
	uint16_t Canal_10;// 1000 a 2000
	uint16_t Canal_11;// 1000 a 2000
	uint16_t Canal_12;// 1000 a 2000
	uint16_t Canal_13;// 1000 a 2000
	uint16_t Canal_14;// 1000 a 2000
	uint16_t Canal_15;// 1000 a 2000
	uint16_t Canal_16;// 1000 a 2000

	uint8_t Interruptor_1;
	uint8_t Interruptor_2;

	uint16_t OUT1; // 1000 a 2000
	uint16_t OUT2; // 1000 a 2000
	uint16_t OUT3; // 1000 a 2000
	uint16_t OUT4; // 1000 a 2000
	uint16_t OUT5; // 1000 a 2000
	uint16_t OUT6; // 1000 a 2000
	uint16_t OUT7; // 1000 a 2000
	uint16_t OUT8; // 1000 a 2000
	uint16_t OUT9; // 1000 a 2000
	uint16_t OUT10;// 1000 a 2000

}blackbox_data_t;

void SD_init(void);
void SD_blackbox_write(void);

#endif /* INC_LIBRARIES_SD_H_ */
