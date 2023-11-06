/*
This file include the header for the management of the Servo motors used for the project.
*/

#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "msp.h"
#include <driverlib.h>

#define SERVO_DUTY_CYCLE_MIN 3200
#define SERVO_DUTY_CYCLE_MAX 6400

Timer_A_PWMConfig pwmConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK, // Use SMCLK as the clock source
        TIMER_A_CLOCKSOURCE_DIVIDER_1, // Clock source divider
        20000, // PWM period (20 ms)
        TIMER_A_CAPTURECOMPARE_REGISTER_1, // CCR register to be used
        TIMER_A_OUTPUTMODE_RESET_SET, // Reset-Set output mode
        SERVO_DUTY_CYCLE_MIN // Initial duty cycle for the servo (minimum position)
    };

void init_servo(void);
int angle_2_dutyCycle(float angle);
void blink_led(void);
void move_servo(int dutyCycle);

#endif
