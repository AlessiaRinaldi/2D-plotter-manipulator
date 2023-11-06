#include "motors.h"
#include "stdio.h"

volatile int servoMoving = 0;

Timer_A_PWMConfig init_servo(void){

    Timer_A_PWMConfig pwmConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,                                                                          // Use SMCLK as the clock source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,                                                                      // Clock source divider
        20000,                                                                                              // PWM period (20 ms)
        TIMER_A_CAPTURECOMPARE_REGISTER_1,                                                                  // CCR register to be used
        TIMER_A_OUTPUTMODE_RESET_SET,                                                                       // Reset-Set output mode
        (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 2,                                                  // Initial duty cycle for the servo (minimum position)
    };

    WDT_A_holdTimer();                                                                                      // stop watchdog timer
    CS_setDCOFrequency(CS_DCO_FREQUENCY_3);                                                                 // set the DCO to 3MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // set up the gpio pin for the integrate led

    return pwmConfig;
}

int angle_2_dutyCycle(float angle){
    // dc min = 3200
    // dc max = 6400

    // 180 : (max - min) = angle : x
    int d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return (d + SERVO_DUTY_CYCLE_MIN);
}


void move_servo(int dutyCycle, Timer_A_PWMConfig pwmConfig){
    pwmConfig.dutyCycle = dutyCycle;
    servoMoving = 1;
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // TODO: blink led while the servo is moving

    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    MAP_Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

void stop_servo(Timer_A_PWMConfig pwmConfig){
    pwmConfig.dutyCycle = 0;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    servoMoving = 0;
}

void servo_timer_interrupt_handler(){
    servoMoving = 0;
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
}