#include "msp.h"
#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include <stdint.h>

extern Timer_A_PWMConfig pwmConfig;

void main(void){
    WDT_A_holdTimer(); 
    // Setting MCLK to REFO at 128Khz for LF mode
    // Setting SMCLK to 64Khz
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_PCM_setPowerState(PCM_AM_LF_VCORE0);

    // P1.1 for button interrupt
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    init_servo();

}





/*

extern volatile int servoMoving;                    // the declaration is in motor.c

void main(void){

    Interrupt_disableMaster();

    Timer_A_PWMConfig pwmConfig = init_servo();

    int i;
    for(i = 0; i < 10000; i++){
        printf("%d \t", i);
    }

    if(pwmConfig.dutyCycle >= (SERVO_DUTY_CYCLE_MIN - SERVO_DUTY_CYCLE_MAX) / 2){
        set_dutycycle(SERVO_DUTY_CYCLE_MIN, pwmConfig);
        set_servo(pwmConfig);
    } else{
        set_dutycycle(SERVO_DUTY_CYCLE_MAX, pwmConfig);
        set_servo(pwmConfig);
    }
    
    __enable_irq();
}

*/

/*

void main(void) {
    WDT_A_hold(WDT_A_BASE); // Stop the watchdog timer

    // Set the system clock
    CS_setDCOFrequency(CS_DCO_FREQUENCY_3); // Set the DCO to 3 MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Configure the PWM Timer for the servo
    Timer_A_PWMConfig pwmConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK, // Use SMCLK as the clock source
        TIMER_A_CLOCKSOURCE_DIVIDER_1, // Clock source divider
        20000, // PWM period (20 ms)
        TIMER_A_CAPTURECOMPARE_REGISTER_1, // CCR register to be used
        TIMER_A_OUTPUTMODE_RESET_SET, // Reset-Set output mode
        SERVO_DUTY_CYCLE_MIN // Initial duty cycle for the servo (minimum position)
    };

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // Set up the GPIO pin for the integrated LED
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    // Enable global interrupts
    __enable_irq();

    while (1) {
        // Activate the integrated LED when the servo is moving (other LEDs can be turned off)
        if (pwmConfig.dutyCycle > SERVO_DUTY_CYCLE_MIN && pwmConfig.dutyCycle < SERVO_DUTY_CYCLE_MAX) {
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4); // Turn on the integrated LED
        } else {
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4); // Turn off the integrated LED
        }

        // You can adjust the servo position by changing the duty cycle
        // For example, to set a new desired position (dutyCycleTarget), use:
        // pwmConfig.dutyCycle = dutyCycleTarget;
        // Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    }

}
*/


