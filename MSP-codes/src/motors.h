/*
This file include the header for the management of the Servo motors used for the project.
*/

#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "msp.h"
#include <driverlib.h>
#include "communication.h"

#define SERVO_DUTY_CYCLE_MIN 500
#define SERVO_DUTY_CYCLE_MAX 9000
#define LINK_1 100
#define LINK_2 110

void init_servo(void);
uint16_t angle_2_duty(float angle);                  // inputs: angle --> outputs: duty cycle
void set_servo(uint16_t duty1, uint16_t duty2);         // inputs: both of the duty cycles
void set_pen();
void set_position(pos_t pos);                           // inputs: position

#endif
