#include "msp.h"
#include <driverlib.h>
#include "motors.h"
#include "communication.h"


#define PWM_FREQUENCY 50                                            // PWM frequency, check
#define SERVO_DUTY_CYCLE_MIN 3200                                   // Minimum duty cicle means minimum position
#define SERVO_DUTY_CYCLE_MAX 6400                                   // Maximum duty cicle means maximum position

const Timer_A_PWMConfig pwmConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    SERVO_DUTY_CYCLE_MIN,
    TIMER_A_CAPTURECOMPARE_REGISTER_1,
    TIMER_A_OUTPUTMODE_RESET_SET,
    SERVO_DUTY_CYCLE_MAX
};

void main(void){

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;                     // Watchdog timer stop

    CS_setDCOFrequency(3000000);                                    // 3 MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);                 // Configure PWM timer for the servo

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    __enable_irq();                                                 // Global interrupt enable

    uint16_t dutyCycle = SERVO_DUTY_CYCLE_MIN;                      // Starting duty cicle for the servo --> starting position
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

void TA0_N_IRQHandler(void) {                                       // Interrupt handler for A0 timer, if there is the need
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}