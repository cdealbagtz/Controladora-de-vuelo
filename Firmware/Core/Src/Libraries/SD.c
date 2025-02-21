/*
 * SD.c
 *
 *  Created on: Feb 18, 2025
 *      Author: cdealba
 */

#include "Libraries/SD.h"

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

char pahtName[7] = "Data000";
char fileName[] = "FD000.csv";

void SD_init(void){
	  fresult = f_mount(&fs, "/", 1);
	  //Get_NewDir();
	  f_open(&fil, fileName, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	  fresult = f_lseek(&fil , f_size(&fil));
	  f_printf(&fil,"Nmsg,AccX,AccY,AccZ,GyroX,GyroY,GyroZ,AngX,AngY,Alt,Pressure,Temp,Parachute,Peripheral\n");
	  f_close(&fil);

	  if(fresult == FR_OK){

	  }
	  else{
		  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
	  }
}
