/*
 * control_allocator.h
 *
 *  Created on: Jul 15, 2025
 *      Author: rtrujillo
 */

#ifndef INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_
#define INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

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


typedef struct{
    uint16_t S_1; 		// pwm_servo1
	uint16_t S_2; 		// pwm_servo2
	uint16_t S_3; 		// pwm_servo3
	uint16_t S_4; 		// pwm_servo4
	uint16_t S_5; 		// pwm_servo5
	uint16_t S_6; 		// pwm_servo6
	uint16_t S_7; 		// pwm_servo7
	uint16_t S_8; 		// pwm_servo8
	uint16_t S_9; 		// pwm_servo9
	uint16_t S_10; 		// pwm_servo10
}Servo_outputs_s;

typedef struct{
    bool S_1;
    bool S_2;
    bool S_3;
    bool S_4;
    bool S_5;
    bool S_6;
    bool S_7;
    bool S_8;
    bool S_9;
}Servo_reverse_s;

typedef struct{
	uint16_t S_1; 		// pwm_servo1
	uint16_t S_2; 		// pwm_servo2
	uint16_t S_3; 		// pwm_servo3
	uint16_t S_4; 		// pwm_servo4
	uint16_t S_5; 		// pwm_servo5
	uint16_t S_6; 		// pwm_servo6
	uint16_t S_7; 		// pwm_servo7
	uint16_t S_8; 		// pwm_servo8
	uint16_t S_9; 		// pwm_servo9
	uint16_t S_10; 		// pwm_servo10
}Amplitude_servo_s;


Servo_outputs_s control_allocator(Cmd_s control_cmd, Trim_s trims, Amplitude_servo_s Amplitude ,Servo_reverse_s reverse, uint8_t frame_type);

#endif /* INC_FLIGHT_MANAGEMENT_CONTROL_CONTROL_ALLOCATOR_H_ */
