#include "motors.h"
#include "stdio.h"

void init_timer(void){

    MAP_WDT_A_holdTimer();                                      // waiting WDT
    MAP_Interrupt_disableMaster();                              // disabling master interrupts

    TIMER_A0->CCR[0] = 60000 - 1;                               /* PWM Period (3Mhz/60k=50hz) */
    TIMER_A0->CCTL[4] = 0xE0;                                   /* CCR4 reset/set mode */
    TIMER_A0->CTL = 0x0214;                                     /* use SMCLK, count up, clear TA0R register */
}

void move_servo(float angle){
    if(angle >= 0 && angle <= 180){
        int duty_cycle = angle / 180 * (7500 - 1500) + 1500;
        TIMER_A0 -> CCR[4] = duty_cycle;

    } else{
        printf("Wrong angle\n");
    }
}


/*
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
*/