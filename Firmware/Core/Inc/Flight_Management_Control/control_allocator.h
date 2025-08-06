/*
 * control_allocator.h
 *
 *  Created on: Jul 15, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_

//Incluye stdint, etc
#include "stm32h7b0xx.h"
#include "stdbool.h"
#include "Libraries/PWM.h"
#include "Libraries/SBUS.h"

typedef struct{
    float roll;
    float pitch;
    float yaw;
    float thrust;
}Cmd_s;


typedef struct{
    float roll;
    float pitch;
    float yaw;
    float thrust;
}Trim_s;

typedef enum
{
    FIX_WING = 0,
	FLYING_WING,
	TANDEM_WING,
	CUSTOM_FRAME,
	TOTAL_FRAMES
}FRAMES_e;

typedef struct {
    bool S[10]; // S[0] a S[9] para los 10 servos (equivalente a S_1 a S_10)
} Servo_reverse_s;


void control_allocator(Cmd_s control_cmd, Trim_s trims,Servo_reverse_s reverse, uint8_t frame_type);

uint16_t reverse_servo_value(uint16_t input) ;

Servo_mgmt_s reverse_servos(Servo_mgmt_s inputs, Servo_reverse_s reverse);

float saturation(float signal_in,float sat_min,float sat_max);

Cmd_s get_commands_rc(void);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_ */
