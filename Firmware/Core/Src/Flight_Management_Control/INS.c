/*
 * INS.c
 *
 *  Created on: May 29, 2026
 *      Author: cdealba
 */


#include "Flight_Management_Control/INS.h"

INS_t INS;

void INS_RefreshData(void){
	INS.p = MTI7.INS.p;
	INS.q = MTI7.INS.q;
	INS.r = MTI7.INS.r;

	INS.roll = MTI7.INS.roll;
	INS.pitch = MTI7.INS.pitch;
	INS.yaw = MTI7.INS.yaw;

	INS.Bar_Altitude = BMP280.Barometric_Altitude;
}
