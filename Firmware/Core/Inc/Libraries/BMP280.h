/*
 * BMP280.h
 *
 *  Created on: Apr 23, 2024
 *      Author: cdealba
 */

#ifndef INC_PERIPHERAL_BMP280_H_
#define INC_PERIPHERAL_BMP280_H_

#include "main.h"
#include <math.h>

extern SPI_HandleTypeDef hspi2;

/*!
 * @def 	SPI_BMP280
 * @brief 	Almacena el puntero a la configuración del SPI para el BMP280
 */
#define SPI_BMP280				&hspi2

/*!
 * @def 	BMP280_select()
 * @brief 	Definición para dar un valor de bajo al pin de CSN del NRF24
 */
#define BMP280_select()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)

/*!
 * @def 	BMP280_unselect()
 * @brief 	Definición para dar un valor de alto al pin de CSN del NRF24
 */
#define BMP280_unselect()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

enum BMP280_memoryMap{
	ID 			= 0xD0,
	reset		= 0xE0,
	status		= 0xF3,
	ctrl_meas 	= 0xF4,
	config      = 0xF5,
	press_msb   = 0xF7,
	press_lsb,
	press_xlsb,
	temp_msb,
	temp_lsb,
	temp_xlsb
};

typedef struct BMP280_Data{
	uint8_t 	ID;
	int32_t		Temp;
	int32_t     Temp_inicial;
	uint32_t	Pressure;
	uint16_t 	Barometric_Altitude;
	uint16_t 	Max_Altitude;
}BMP280_t;

void BMP280_init(void);
void BMP280_calculate(void);

#endif /* INC_PERIPHERAL_BMP280_H_ */
