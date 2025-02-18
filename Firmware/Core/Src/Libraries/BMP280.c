/*
 * BMP280.c
 *
 *  Created on: Apr 23, 2024
 *      Author: cdealba
 */


#include "Libraries/BMP280.h"

BMP280_t BMP280;

uint16_t dig_T1, dig_P1;
int16_t  dig_T2, dig_T3, dig_P2,dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

int32_t T_raw, P_raw, t_fine;

void BMP280_write(uint8_t Address, uint8_t Data){
	BMP280_select();
	Address &= 0x7F;
	HAL_SPI_Transmit(SPI_BMP280, &Address, 1, 100);
	HAL_SPI_Transmit(SPI_BMP280, &Data, 1, 100);

	BMP280_unselect();
}

uint8_t BMP280_read(uint8_t Address){
	uint8_t Buffer = (Address|0x80);
	BMP280_select();
	HAL_SPI_Transmit(SPI_BMP280, &Buffer, 1, 100);
	HAL_SPI_Receive(SPI_BMP280, &Buffer, 1, 100);
	BMP280_unselect();

	return Buffer;
}

void BMP280_config(void){
	BMP280_write(ctrl_meas, 0x57);
	BMP280_write(config, 0x10);
}

void BMP280_reset(void){
	BMP280_write(reset, 0xB6);
}

void BMP280_calibrationData(void){
	uint8_t Address = 0x88;
	uint8_t Buffer[24] = {0};
	BMP280_select();
	HAL_SPI_Transmit(SPI_BMP280, &Address, 1, 100);
	HAL_SPI_Receive(SPI_BMP280, Buffer, 24, 100);
	BMP280_unselect();

	dig_T1 = (Buffer[1]<<8)|Buffer[0];
	dig_T2 = (Buffer[3]<<8)|Buffer[2];
	dig_T3 = (Buffer[5]<<8)|Buffer[4];

	dig_P1 = (Buffer[7]<<8)|Buffer[6];
	dig_P2 = (Buffer[9]<<8)|Buffer[8];
	dig_P3 = (Buffer[11]<<8)|Buffer[10];
	dig_P4 = (Buffer[13]<<8)|Buffer[12];
	dig_P5 = (Buffer[15]<<8)|Buffer[14];
	dig_P6 = (Buffer[17]<<8)|Buffer[16];
	dig_P7 = (Buffer[19]<<8)|Buffer[18];
	dig_P8 = (Buffer[21]<<8)|Buffer[20];
	dig_P9 = (Buffer[23]<<8)|Buffer[22];
}

void BMP280_readRawValues(void){
	uint8_t Address = press_msb;
	uint8_t Buffer[6] = {0};
	BMP280_select();
	HAL_SPI_Transmit(SPI_BMP280, &Address, 1, 100);
	HAL_SPI_Receive(SPI_BMP280, Buffer, 6, 100);
	BMP280_unselect();

	P_raw = (Buffer[0]<<12)|(Buffer[1]<<4)|(Buffer[2]>>4);
	T_raw = (Buffer[3]<<12)|(Buffer[4]<<4)|(Buffer[5]>>4);
}

int32_t BMP280_measureT(int32_t adc_T){
	int32_t var1, var2, T;
	var1 =  ((((T_raw>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

uint32_t BMP280_measureP(int32_t adc_P){
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
	if (var1 == 0)
	{
	return 0;
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (uint32_t)p;
}

uint16_t BMP280_measureH(uint32_t Pres, int32_t Temp){
	double var1, var2, h;

	if(Pres == 0) return 0;
	var1 = -log(((double)Pres)/101325);

	if(var1 == 0) return 0;
	var2 = 0.0341663/((((double)Temp)/100)+273.15);
	h = var1/var2;
	return (uint16_t)h;
}

void BMP280_init(void){
	BMP280_unselect();
	BMP280_config();
	BMP280.ID = BMP280_read(0x89);
	BMP280_calibrationData();
	BMP280_readRawValues();
	BMP280.Temp_inicial = BMP280_measureT(T_raw);
}

void BMP280_calculate(void){
	//BMP280.ID = 0;
	BMP280.ID = BMP280_read(0x89);
	BMP280_readRawValues();
	BMP280.Temp = BMP280_measureT(T_raw);
	BMP280.Pressure    		= BMP280_measureP(P_raw)/256;
	BMP280.Barometric_Altitude = BMP280_measureH(BMP280.Pressure, BMP280.Temp_inicial);

	if(BMP280.Barometric_Altitude > BMP280.Max_Altitude) BMP280.Max_Altitude = BMP280.Barometric_Altitude;
}

