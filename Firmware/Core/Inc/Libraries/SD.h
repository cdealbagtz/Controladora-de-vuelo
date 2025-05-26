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
	// === 4-byte aligned (32 bits) ===
	float Ax, Ay, Az;
	float Gx, Gy, Gz;
	float Gfx, Gfy, Gfz;
	float Roll, Pitch, Heading;

    uint32_t Nmsg;
    uint32_t Time;
    int32_t Latitude;
    int32_t Longitude;
    uint32_t Pressure;

    // === 2-byte aligned (16 bits) ===
    uint16_t Alt;
    uint16_t ARSP;

    uint16_t Canal_1;
    uint16_t Canal_2;
    uint16_t Canal_3;
    uint16_t Canal_4;
    uint16_t Canal_5;
    uint16_t Canal_6;
    uint16_t Canal_7;
    uint16_t Canal_8;
    uint16_t Canal_9;
    uint16_t Canal_10;
    uint16_t Canal_11;
    uint16_t Canal_12;
    uint16_t Canal_13;
    uint16_t Canal_14;
    uint16_t Canal_15;
    uint16_t Canal_16;

    uint16_t OUT1;
    uint16_t OUT2;
    uint16_t OUT3;
    uint16_t OUT4;
    uint16_t OUT5;
    uint16_t OUT6;
    uint16_t OUT7;
    uint16_t OUT8;
    uint16_t OUT9;
    uint16_t OUT10;

    // === 1-byte aligned ===
    uint8_t SBUS_Flags;
    uint8_t Flight_Mode;
    uint8_t Interruptor_1;
    uint8_t Interruptor_2;

}blackbox_data_t;

void SD_init(void);
void SD_blackbox_write(void);

#endif /* INC_LIBRARIES_SD_H_ */
