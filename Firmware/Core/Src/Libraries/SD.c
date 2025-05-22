/*
 * SD.c
 *
 *  Created on: Feb 18, 2025
 *      Author: cdealba
 */

#include <CONFIG.h>
#include "Libraries/SD.h"

#include "Libraries/BNO050.h"
#include "Libraries/BMP280.h"
#include "Libraries/SBUS.h"
#include "Libraries/PWM.h"

FATFS fs;  // file system

FIL BlackBox; // File
FIL Config;
FIL Gains;

FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

char FlightPaht[9] = "Flight000";

char BlackBoxFile[9] = "FD000.bin";
char ConfigFile[10]  = "Config.txt";
char GainsFile[9]    = "Gains.csv";

char NResetChar[3]  = {255};

char BlackBoxBuffer[16][255];

blackbox_data_t blackbox_data;

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

uint16_t ASCII2uint16(char *buffer, uint8_t Size){
	uint16_t inter = 0;

	for (uint8_t n = 0; n < Size; ++n) {
		inter *= 10;

		inter += (buffer[n] - 48);
	}
	return inter;
}

void uint162ASCII(uint16_t Inter, char *Output){
	uint16_t Scaler = 10000;

	for (uint8_t n = 0; n < 5; ++n) {

		Output[n] = Inter/Scaler;
		Inter -= Output[n]*Scaler;
		Output[n] +=48;
		Scaler /= 10;
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

void SD_ConfigFileInit(void){
	char FileVersion[5] = {0};

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

void SD_GainsInit(void){

}

void SD_CreateFlightPath(void){
	FlightPaht[6] = NResetChar[0];
	FlightPaht[7] = NResetChar[1];
	FlightPaht[8] = NResetChar[2];

	f_mkdir(FlightPaht);
	f_chdir(FlightPaht);
}

void SD_blackbox_init(void){

	f_open(&BlackBox, BlackBoxFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
}

void SD_blackbox_refresh(void){

	blackbox_data.Time = TimeOn_Counter;

	blackbox_data.Ax = IMU.ACC.x;
	blackbox_data.Ay = IMU.ACC.y;
	blackbox_data.Az = IMU.ACC.z;

	blackbox_data.Gx = IMU.GYR.x;
	blackbox_data.Gy = IMU.GYR.y;
	blackbox_data.Gz = IMU.GYR.z;

	blackbox_data.Gfx = 0;
	blackbox_data.Gfy = 0;
	blackbox_data.Gfz = 0;

	blackbox_data.Roll    = IMU.Roll;
	blackbox_data.Pitch   = IMU.Pitch;
	blackbox_data.Heading = IMU.Heading;

	blackbox_data.Alt = BMP280.Barometric_Altitude;
	blackbox_data.Latitude = 0;
	blackbox_data.Longitude= 0;

	blackbox_data.ARSP = 0;

	blackbox_data.Pressure = BMP280.Pressure;

	blackbox_data.SBUS_Flags = Radio_input.Banderas;
	blackbox_data.Flight_Mode = 0;

	blackbox_data.Canal_1 = Radio_input.Canal_1;
	blackbox_data.Canal_2 = Radio_input.Canal_2;
	blackbox_data.Canal_3 = Radio_input.Canal_3;
	blackbox_data.Canal_4 = Radio_input.Canal_4;
	blackbox_data.Canal_5 = Radio_input.Canal_5;
	blackbox_data.Canal_6 = Radio_input.Canal_6;
	blackbox_data.Canal_7 = Radio_input.Canal_7;
	blackbox_data.Canal_8 = Radio_input.Canal_8;
	blackbox_data.Canal_9 = Radio_input.Canal_9;
	blackbox_data.Canal_10 = Radio_input.Canal_10;
	blackbox_data.Canal_11 = Radio_input.Canal_11;
	blackbox_data.Canal_12 = Radio_input.Canal_12;
	blackbox_data.Canal_13 = Radio_input.Canal_13;
	blackbox_data.Canal_14 = Radio_input.Canal_14;
	blackbox_data.Canal_15 = Radio_input.Canal_15;
	blackbox_data.Canal_16 = Radio_input.Canal_16;

	blackbox_data.Interruptor_1 = Radio_input.Interruptor_1;
	blackbox_data.Interruptor_2 = Radio_input.Interruptor_2;

	blackbox_data.OUT1  = PWM_Output.Canal_1;
	blackbox_data.OUT2  = PWM_Output.Canal_2;
	blackbox_data.OUT3  = PWM_Output.Canal_3;
	blackbox_data.OUT4  = PWM_Output.Canal_4;
	blackbox_data.OUT5  = PWM_Output.Canal_5;
	blackbox_data.OUT6  = PWM_Output.Canal_6;
	blackbox_data.OUT7  = PWM_Output.Canal_7;
	blackbox_data.OUT8  = PWM_Output.Canal_8;
	blackbox_data.OUT9  = PWM_Output.Canal_9;
	blackbox_data.OUT10 = PWM_Output.Canal_10;
}

void SD_blackboxNewFile(void){
	char ActualFile[5] = {0};
	uint16_t FileCount;
	f_close(&BlackBox);

	FileCount = ASCII2uint16(&BlackBoxFile[2], 3) + 1;
	uint162ASCII(FileCount, ActualFile);

	BlackBoxFile[2] = ActualFile[2];
	BlackBoxFile[3] = ActualFile[3];
	BlackBoxFile[4] = ActualFile[4];

	f_open(&BlackBox, BlackBoxFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
}

void SD_blackbox_write(void){
	static uint8_t NewFileCount = 0;
	static uint32_t DataCount = 0;
	static uint16_t WriteCount = 0;

	SD_blackbox_refresh();

	blackbox_data.Nmsg = DataCount;
	memcpy(&BlackBoxBuffer[WriteCount],&blackbox_data,sizeof(blackbox_data));

	if(WriteCount == 15){
		for (uint8_t n = 0; n < 16; ++n) {
			f_write(&BlackBox, &BlackBoxBuffer[n], sizeof(blackbox_data), &bw);
		}
		fresult = f_sync(&BlackBox);
		WriteCount = 0;
		++NewFileCount;
		++DataCount;
		return;
	}
	++WriteCount;
	++DataCount;
}


void SD_init(void){
	  fresult = f_mount(&fs, "/", 1);
	  SD_ConfigFileInit();
	  SD_GainsInit();
	  SD_CreateFlightPath();
	  SD_blackbox_init();
}
