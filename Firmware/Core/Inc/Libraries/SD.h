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


void SD_init(void);
void SD_blackbox_write(void);

#endif /* INC_LIBRARIES_SD_H_ */
