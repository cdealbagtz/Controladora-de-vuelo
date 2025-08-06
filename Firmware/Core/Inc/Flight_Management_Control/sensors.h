/*
 * sensors.h
 *
 *  Created on: Aug 4, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_SENSORS_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_SENSORS_H_
#include "Libraries/BNO050.h"
#include "filter.h"


typedef struct{
    float roll;
    float pitch;
    float yaw;
}Gyro_s;

typedef struct{
    float X ;
    float Y ;
    float Z ;
}Acc_s;

extern Gyro_s actual_rates;

Gyro_s get_actual_rates(void);

Acc_s get_actual_acc(void) ;

void LPF_gyro_filter_init(void);

void refresh_actual_rates (void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_SENSORS_H_ */
