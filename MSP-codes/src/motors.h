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

/**
 * @brief       Init function for all the servos
*/
void init_servo(void);

/**
 * @brief       Convert the angle (radians) to duty cycle
 * @param       angle The angle that should be converted
 * @return      corrispondent duty cycle
*/
uint16_t angle_2_duty(float angle);

/**
 * @brief       Set the pwms duty cycles in order to move the pen
 * @param       duty1 Duty cycle for the first servo
 * @param       duty2 Duty cycle for the second servo
*/
void set_servo(uint16_t duty1, uint16_t duty2);

/**
 * @brief       Change the pen status when it's called, high or low
*/
void set_pen();

/**
 * @brief       Overall function, receive positions and set motors angles
 * @param       pos pos_t instance that contains the new position of the pen
*/
void set_position(pos_t *pos);

#endif
