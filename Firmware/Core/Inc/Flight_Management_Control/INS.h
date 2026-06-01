/*
 * INS.h
 *
 *  Created on: May 29, 2026
 *      Author: cdealba
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_INS_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_INS_H_

#include "main.h"
#include "Libraries/MTI7.h"
#include "Libraries/BMP280.h"

typedef struct{
	float	p;
	float	q;
	float	r;

	float roll;
	float pitch;
	float yaw;

	uint16_t Bar_Altitude;
	uint16_t GPS_Altitude;

	uint8_t Groundspeed;
	uint8_t  Airspeed;
}INS_t;

extern INS_t INS;

void INS_RefreshData(void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_INS_H_ */
