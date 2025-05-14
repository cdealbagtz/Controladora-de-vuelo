/*
 * SD.c
 *
 *  Created on: Feb 18, 2025
 *      Author: cdealba
 */

#include <CONFIG.h>
#include "Libraries/SD.h"

FATFS fs;  // file system

FIL fil; // File
FIL Config;

FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

uint8_t TestFlag = 0;

char pahtName[7] = "Data000";
char fileName[] = "FD000.csv";

char ConfigFile[10] = "Config.txt";
char GainsFile[9]    = "Gains.csv";

uint8_t ASCII2uint8(char *buffer, uint8_t Size){
	uint8_t inter = 0;

	for (uint8_t n = 0; n < Size; ++n) {
		inter *= 10;

		inter += (buffer[n] - 48);
	}
	return inter;
}

void uint82ASCII(uint8_t Inter, char *Output){
	Output[0] = Inter/100;
	Output[1] = (Inter - Output[0]*100) /10;
	Output[2] = (Inter - Output[0]*100 - Output[1]*10) ;

	for (uint8_t n = 0; n < 3; ++n) {
		Output[n] +=48;
	}
}

void ConfigFile_Create(void){

	f_unlink(ConfigFile);

	f_open(&Config, ConfigFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	f_printf(&Config, "Version: ");
	f_printf(&Config,  SystemConfig.FWVersion);
	f_printf(&Config, "\nN. Reset: 000\n");
	f_close(&Config);
}

void ConfigFile_Init(void){
	char FileVersion[5] = {0};
	char NResetChar[3]  = {255};

	f_open(&Config, ConfigFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	f_lseek(&Config, Config_FWVersion);

	f_read(&Config, FileVersion, 5, &br);

	f_close(&Config);

	for (uint8_t n = 0; n < 5; ++n) {
		if(FileVersion[n] != SystemConfig.FWVersion[n]){
			ConfigFile_Create();
			n = 5;
		}
	}
	f_open(&Config, ConfigFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	f_lseek(&Config, 26);
	f_read(&Config, NResetChar, 3, &br);

	SystemConfig.ResetCounter = ASCII2uint8(NResetChar, 3) + 1;
	uint82ASCII(SystemConfig.ResetCounter, NResetChar);

	f_lseek(&Config, 26);
	f_write(&Config, NResetChar, 3, &bw);

	f_close(&Config);

}

void SD_init(void){
	  fresult = f_mount(&fs, "/", 1);

	  ConfigFile_Init();
}
