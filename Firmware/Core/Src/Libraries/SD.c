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

char BlackBoxFile[9] = "FD000.csv";
char ConfigFile[10]  = "Config.txt";
char GainsFile[9]    = "Gains.csv";

char NResetChar[3]  = {255};

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
	f_printf(&BlackBox,"Nmsg,Time,Ax,Ay,Az,Gx,Gy,Gz,Roll,Pitch,Heading,Alt,Lat,Lon,ARSP,Pressure,SBUS_Flags,FlightMode,");
	f_printf(&BlackBox,"PWM1,PWM2,PWM3,PWM4,PWM5,PWM6,PWM7,PWM8,PWM9,PWM10,PWM11,PWM12,PWM13,PWM14,PWM15,PWM16,INT1,INT2,");
	f_printf(&BlackBox,"OUT1,OUT2,OUT3,OUT4,OUT5,OUT6,OUT7,OUT8,OUT9,OUT10\n");
	f_sync(&BlackBox);

}

void SD_blackboxNewFile(void){
	char ActualFile[5] = {0};
	uint16_t FileCount;

	FileCount = ASCII2uint16(&BlackBoxFile[2], 3) + 1;
	uint162ASCII(FileCount, ActualFile);

	BlackBoxFile[2] = ActualFile[2];
	BlackBoxFile[3] = ActualFile[3];
	BlackBoxFile[4] = ActualFile[4];

	f_open(&BlackBox, BlackBoxFile, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
}

void SD_blackbox_write(void){
	static uint32_t DataCount;
	static uint16_t WriteCount;

	f_printf(&BlackBox, "%d,%d,%d,%d,%d,",DataCount,TimeOn_Counter,(int32_t)(IMU.ACC.x*100),(int32_t)(IMU.ACC.y*100),(int32_t)(IMU.ACC.z*100));
	f_printf(&BlackBox, "%d,%d,%d,%d,%d,%d,",(int32_t)(IMU.GYR.x*100),(int32_t)(IMU.GYR.y*100),(int32_t)(IMU.GYR.z*100),(int32_t)(IMU.Roll*100),(int32_t)(IMU.Pitch*100),(int32_t)(IMU.Heading*100));
	f_printf(&BlackBox, "%d,NA,NA,NA,%d,%d,NA,",BMP280.Barometric_Altitude,BMP280.Pressure,Radio_input.Banderas);
	f_printf(&BlackBox, "%d,%d,%d,%d,",Radio_input.Canal_1,Radio_input.Canal_2,Radio_input.Canal_3,Radio_input.Canal_4);
	f_printf(&BlackBox, "%d,%d,%d,%d,",Radio_input.Canal_5,Radio_input.Canal_6,Radio_input.Canal_7,Radio_input.Canal_8);
	f_printf(&BlackBox, "%d,%d,%d,%d,",Radio_input.Canal_9,Radio_input.Canal_10,Radio_input.Canal_11,Radio_input.Canal_12);
	f_printf(&BlackBox, "%d,%d,%d,%d,",Radio_input.Canal_13,Radio_input.Canal_14,Radio_input.Canal_15,Radio_input.Canal_16);
	f_printf(&BlackBox, "%d,%d,%d,%d,",Radio_input.Interruptor_1,Radio_input.Interruptor_2,PWM_Output.Canal_1,PWM_Output.Canal_2);
	f_printf(&BlackBox, "%d,%d,%d,%d,",PWM_Output.Canal_3,PWM_Output.Canal_4,PWM_Output.Canal_5,PWM_Output.Canal_6);
	f_printf(&BlackBox, "%d,%d,%d,%d\n",PWM_Output.Canal_7,PWM_Output.Canal_8,PWM_Output.Canal_9,PWM_Output.Canal_10);

	if(WriteCount > 100){
		fresult = f_sync(&BlackBox);
		WriteCount = 0;
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
