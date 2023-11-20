#include "msp.h"
//#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "motors.h"

/*

// Timer_A1.1 PWM Configuration Parameter pin 7.7 primary (reg 1), MOTOR 1 CCW
Timer_A_PWMConfig pwmConfigA1_1 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
        //96 // 7.5% or 1.5ms duty cycle        cannot start above throttle 0     OG 3200
};
// Timer_A1.2 PWM Configuration Parameter pin 7.6 primary, MOTOR 2 CCW
Timer_A_PWMConfig pwmConfigA1_2 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_2,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};
// Timer_A1.3 PWM Configuration Parameter pin 7.5 primary, MOTOR 3 CW
Timer_A_PWMConfig pwmConfigA1_3 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};
// Timer_A1.4 PWM Configuration Parameter pin 7.4 primary (reg 4), MOTOR 4 CW
Timer_A_PWMConfig pwmConfigA1_4 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

*/


// Timer_A PWM Configuration Parameter pin 2.4 primary, MOTOR 1 CCW
extern Timer_A_PWMConfig pwmConfig0;

/*

// Timer_A1 PWM Configuration Parameter pin 7.7 primary, MOTOR 2 CCW
Timer_A_PWMConfig pwmConfig1 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
        //96 // 7.5% or 1.5ms duty cycle        cannot start above throttle 0     OG 3200
};
*/


// Timer_A2 PWM Configuration Parameter pin 5.6 primary, MOTOR 3 CW
extern Timer_A_PWMConfig pwmConfig2;

/*

// Timer_A3 PWM Configuration Parameter pin 10.5 primary, MOTOR 4 CW
Timer_A_PWMConfig pwmConfig3 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

*/

// Timer_A_UpModeConfig upConfig = {
// }


int main(void)
{
    // Halting the watchdog
    MAP_WDT_A_holdTimer();

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

    
    // P1.4 for button interrupt
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    /*
    // Configuring GPIO 7.7 as peripheral output Timer A1.1 PWM
    // Configuring GPIO 7.6 for Timer A1.2 PWM output
    // Configuring GPIO 7.5 for Timer A1.3 PWM output
    // Configuring GPIO 7.4 as peripheral output TIMER A1.4 for PWM output
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    */

    // Configuring GPIO 2.4 as peripheral output Timer A0.1 PWM
    // Configuring GPIO 7.7 for Timer A1.1 PWM output
    // Configuring GPIO 5.6 for Timer A2.1 PWM output
    // Configuring GPIO 10.5 as peripheral output TIMER A3.1 for PWM output
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
/*
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,GPIO_PRIMARY_MODULE_FUNCTION);
/* 
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P10, GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
*/

    // Set P1.0 to output direction (LED)
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);

    // Configuring Timer_A to have a period of approximately 20ms and an initial duty cycle of 5% of that (64 ticks)

    /*
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_1);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_2);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_3);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_4);
    */




    // OG Configuring Timer_A to have a period of approximately 500ms and an initial duty cycle of 10% of that (3200 ticks)
    // Configuring Timer_A to have a period of approximately 20ms and an initial duty cycle of 5% of that (64 ticks)
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
/*    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig1);
*/
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
/*
    MAP_Timer_A_generatePWM(TIMER_A3_BASE, &pwmConfig3);
*/

    // Enabling interrupts and starting the watchdog timer
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();

    // Sleeping when not in use
    while (1)
    {
        MAP_PCM_gotoLPM0();
    }
}



// Port1 ISR - This ISR will progressively step up the duty cycle of the PWM  on a button press
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;
    int32_t speedChange;           // %percent change on button press 16 for 1.25% increase or 0.25ms 32 for 2.5% or .5ms
    
    /*
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1) // if button 1.1 is pressed
    {
        // TimerA1.1 pin 7.7 Motor 1 CCW
        if(pwmConfigA1_1.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -16;
        }
        if(pwmConfigA1_1.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 16;
        }
        pwmConfigA1_1.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_1);

        // TimerA1.2 pin 7.6 Motor 2 CCW
        if(pwmConfigA1_2.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -16;
        }
        if(pwmConfigA1_2.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 16;
        }
        pwmConfigA1_2.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_2);

        // Toggle P1.0 output - Button Press debug purposes
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
    }
    if (status & GPIO_PIN4) // if button 1.4 is pressed
    {
        // TimerA1.3 pin 7.5 Motor 3 CW
        if(pwmConfigA1_3.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -16;
        }
        if(pwmConfigA1_3.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 16;
        }
        pwmConfigA1_3.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_3);

        // TimerA1.4 pin 7.4 Motor 4 CW
        if(pwmConfigA1_4.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -16;
        }
        if(pwmConfigA1_4.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 16;
        }
        pwmConfigA1_4.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfigA1_4);

        // Toggle P1.0 output - Button Press debug purposes
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
    }
*/



    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1)
    {
        /*
        pos_t tmp = set_position(120, 120);
        pwmConfig0.dutyCycle = tmp.x;
        pwmConfig2.dutyCycle = tmp.y;

        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
        */

                                                                            
        // TimerA0.1 pin 2.4 Motor 1 CCW
        if(pwmConfig0.dutyCycle >= 2000) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -64;
//            pwmConfig.dutyCycle += speedChange;
        }
        if(pwmConfig0.dutyCycle <= 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 64;
//            pwmConfig.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        }
//            pwmConfig.dutyCycle = 64;  //  reset to 5% or 1ms         3200 10%
//        else
        pwmConfig0.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%

        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
    
                                                                            
    }
                                                                            

    if (status & GPIO_PIN4)
    {
        // TimerA2.1 pin 5.6 Motor 3 CW
        if(pwmConfig2.dutyCycle >= 2000) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -64;
        }
        if(pwmConfig2.dutyCycle <= 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 64;
        }

        pwmConfig2.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%

        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
    }
                                                                                                



/*
        // TimerA1.1 pin 7.7 Motor 2 CCW
         if(pwmConfig1.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
         {
             speedChange = -16;
         }
         if(pwmConfig1.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
         {
             speedChange = 16;
         }

         pwmConfig1.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%

         MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig1);


        // Button Press debug
        // Toggle P1.0 output debug button press
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
        // Delay
        for(i=1000; i>0; i--);
    }
*/

/*
        // TimerA3.1 pin 10.5 Motor 4 CW
        if(pwmConfig3.dutyCycle == 128) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -16;
        }
        if(pwmConfig3.dutyCycle == 64) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 16;
        }

        pwmConfig3.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%

        MAP_Timer_A_generatePWM(TIMER_A3_BASE, &pwmConfig3);



        // Button Press debug
        // Toggle P1.0 output debug button press
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
     }
*/
}








/////////////////////////////////////////////////////////////////////////////////



/*

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
*/




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

