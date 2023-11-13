/*
This file include the header for the management of the Servo motors used for the project.
*/

#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "msp.h"
#include <driverlib.h>
#define SERVO_DUTY_CYCLE_MIN 600
#define SERVO_DUTY_CYCLE_MAX 3510
#define LINK_1 100
#define LINK_2 110

/*
Set 2D point struct
*/

typedef struct pos_t{
    uint16_t x;
    uint16_t y;
} pos_t;

void init_servo(void);
uint16_t angle_2_duty(uint16_t angle);                 // inputs: angle --> outputs: duty cycle
void set_servo(uint16_t duty1, uint16_t duty2);         // inputs: both of the duty cycles
void set_position(pos_t pos);



#endif
