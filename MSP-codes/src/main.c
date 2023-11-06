#include "msp.h"
#include "motors.h"
#include <driverlib.h>
#include <stdint.h>

void main(void){

    Timer_A_PWMConfig pwmConfig = init_servo();

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
    __enable_irq();
    
    int i = 0;
    //for(i = 0; i < 10000; i++);

    //move_servo(angle_2_dutyCycle(45), pwmConfig);

    //for(i = 0; i < 10000; i++);
    //move_servo(angle_2_dutyCycle(180), pwmConfig);
}

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
