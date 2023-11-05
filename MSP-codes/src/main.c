#include "msp.h"
#include "motors.h"
#include "communication.h"

#include <stdint.h>

#include <stdbool.h>

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig =
{
TIMER_A_CLOCKSOURCE_SMCLK,
TIMER_A_CLOCKSOURCE_DIVIDER_1,
240000,
TIMER_A_CAPTURECOMPARE_REGISTER_1,
TIMER_A_OUTPUTMODE_RESET_SET,
12000
};

int main(void)
{
/* Halting the watchdog */
MAP_WDT_A_holdTimer();

/* Setting MCLK to REFO at 128Khz for LF mode
* Setting SMCLK to 64Khz */
CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

/* Configuring GPIO2.4 as peripheral output for PWM and P6.7 for button
* interrupt */
MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
GPIO_PRIMARY_MODULE_FUNCTION);
MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

/* Configuring Timer_A to have a period of approximately 500ms and
* an initial duty cycle of 10% of that (3200 ticks) */
MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

/* Enabling interrupts and starting the watchdog timer */
MAP_Interrupt_enableInterrupt(INT_PORT1);
MAP_Interrupt_enableSleepOnIsrExit();
MAP_Interrupt_enableMaster();

/* Sleeping when not in use */
while (1)
{
MAP_PCM_gotoLPM0();
}
}

/* Port1 ISR - This ISR will progressively step up the duty cycle of the PWM
* on a button press
*/
void PORT1_IRQHandler(void)
{
uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

if (status & GPIO_PIN1)
{
if(pwmConfig.dutyCycle == 64000)
pwmConfig.dutyCycle = 32000;
else
pwmConfig.dutyCycle += 45000;

MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}
}

/*
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
    P2 -> DIR |= BIT4;
    P2 -> SEL0 |= BIT4;
    P2 -> SEL1 &= -BIT4;

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

    __enable_irq();                                                 // Global interrupt enable


    //test
    move_servo(SERVO_DUTY_CYCLE_MIN);
}

void TA0_N_IRQHandler(void) {                                       // Interrupt handler for A0 timer, if there is the need
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
*/
