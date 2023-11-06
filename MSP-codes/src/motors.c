#include "motors.h"
#include "stdio.h"

void init_servo(void){

    WDT_A_holdTimer();                                      // stop watchdog timer
    CS_setDCOFrequency(CS_DCO_FREQUENCY_3);                 // set the DCO to 3MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // set up the gpio pin for the integrate led
}

int angle_2_dutyCycle(float angle){
    // dc min = 3200
    // dc max = 6400

    // 180 : (max - min) = angle : x
    int d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return (d + SERVO_DUTY_CYCLE_MIN);
}

void blink_led(void){
    if (pwmConfig.dutyCycle > SERVO_DUTY_CYCLE_MIN && pwmConfig.dutyCycle < SERVO_DUTY_CYCLE_MAX) {
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4); // Turn on the integrated LED
        } else {
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4); // Turn off the integrated LED
    }
}

void move_servo(int dutyCycle){
    pwmConfig.dutyCycle = dutyCycle;
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // TODO: blink led while the servo is moving
}