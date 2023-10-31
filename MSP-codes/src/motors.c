#include "motors.h"

void move_servo(uint16_t dutyCycle){
    // dutyCycle = SERVO_DUTY_CYCLE_MIN;                      // Starting duty cicle for the servo --> starting position
    // TODO: check dutycycle validity
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, dutyCycle);

    // Here the job starts
    while (1) {
        if (dutyCycle > SERVO_DUTY_CYCLE_MIN && dutyCycle < SERVO_DUTY_CYCLE_MAX) {
            dutyCycle++;
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, dutyCycle);

            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        } else {
            if(dutyCycle >= SERVO_DUTY_CYCLE_MAX){
                do{
                    dutyCycle--;
                    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, dutyCycle);
                } while(dutyCycle > SERVO_DUTY_CYCLE_MAX);
            }
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }
}