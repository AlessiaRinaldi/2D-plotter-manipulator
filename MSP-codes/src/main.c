#include "msp.h"
#include "motors.h"
#include "communication.h"

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


    //test
    move_servo(SERVO_DUTY_CYCLE_MIN);
}

void TA0_N_IRQHandler(void) {                                       // Interrupt handler for A0 timer, if there is the need
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}