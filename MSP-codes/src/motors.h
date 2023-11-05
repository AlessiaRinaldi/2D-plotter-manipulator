/*
This file include the header for the management of the Servo motors used for the project.
*/

#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>
#include <driverlib.h>

#define PWM_FREQUENCY 50                                            // PWM frequency, check
#define SERVO_DUTY_CYCLE_MIN 3200                                   // Minimum duty cicle means minimum position
#define SERVO_DUTY_CYCLE_MAX 6400                                   // Maximum duty cicle means maximum position
#define CLK_FREQ 3000000

void PORT1_IRQHandler(void);
void init_timer(void);
void move_servo(float angle);

#endif
