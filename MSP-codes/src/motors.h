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

void init_servo(void);

int angle_2_dutyCycle(float angle);                                 // converter from degrees angle to duty-cycle
void blink_led(void);                                               // led blinker when the servo moves
void set_dutycycle(int dutyCycle, Timer_A_PWMConfig pwmConfig);
void set_servo(Timer_A_PWMConfig pwmConfig);       // servo actuation
void servo_timer_interrupt_handler(); 
void PORT1_IRQHandler(void);

void set_position(uint16_t x, uint16_t y);

#endif
