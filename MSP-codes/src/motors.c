#include "motors.h"
#include "stdio.h"
#include "math.h"

typedef struct Servo_t{
    uint16_t x;
    uint16_t y;
    int dc;                 // duty cycle
} servo1, servo2;


volatile int servoMoving = 0;
Timer_A_PWMConfig pwmConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        20000, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        SERVO_DUTY_CYCLE_MIN // 5% or 1ms duty cycle             OG 3200
    };

void init_servo(void){
    
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);


/*
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // set up the gpio pin for the integrate led

    Timer_A_PWMConfig pwmConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,                                                                          // Use SMCLK as the clock source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,                                                                      // Clock source divider
        1280,                                                                                              // PWM period (20 ms)
        TIMER_A_CAPTURECOMPARE_REGISTER_1,                                                                  // CCR register to be used
        TIMER_A_OUTPUTMODE_RESET_SET,                                                                       // Reset-Set output mode
        (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 2 +500,                                                  // Initial duty cycle for the servo (minimum position)
    };

                                                                                         // stop watchdog timer
    CS_setDCOFrequency(CS_DCO_FREQUENCY_3);                                                                 // set the DCO to 3MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    
*/


    //check this    
    //MAP_Timer_A_configureUpMode(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

    //set_dutycycle(SERVO_DUTY_CYCLE_MAX, pwmConfig);
    set_servo(pwmConfig);

    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();

    // Sleeping when not in use
    while (1)
    {
        MAP_PCM_gotoLPM0();
    }

    /*
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    __delay_cycles(2000);
    //pwmConfig.dutyCycle = angle_2_dutyCycle(160);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    __delay_cycles(2000);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    */


    //return pwmConfig;
}



void PORT1_IRQHandler(void)
{
    volatile uint32_t i;
    int32_t speedChange;    


    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    if (status & GPIO_PIN1)
    {
        // TimerA0.1 pin 2.4 Motor 1 CCW
        if(pwmConfig.dutyCycle > (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 2 && pwmConfig.dutyCycle <= SERVO_DUTY_CYCLE_MAX) //   10% cycle at 128,           OG 28800 90%
        {
            speedChange = -4;
//            pwmConfig.dutyCycle += speedChange;
        }
        if(pwmConfig.dutyCycle <= (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 2 && pwmConfig.dutyCycle >= SERVO_DUTY_CYCLE_MIN) //   5% cycle, 1ms          OG 28800 90%
        {
            speedChange = 4;
//            pwmConfig.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%
        }
//            pwmConfig.dutyCycle = 64;  //  reset to 5% or 1ms         3200 10%
//        else
        pwmConfig.dutyCycle += speedChange; //  1.25% increase or 0.25ms 32 for 2.5% or .5ms     OG 3200 10%

        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    }
}





















int angle_2_dutyCycle(float angle){

    // 180 : (max - min) = angle : x
    int d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return (d + SERVO_DUTY_CYCLE_MIN);
}

void set_dutycycle(int dutyCycle, Timer_A_PWMConfig pwmConfig){
    pwmConfig.dutyCycle = dutyCycle;
}

void set_servo(Timer_A_PWMConfig pwmConfig){
    
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // TODO: blink led while the servo is moving

/*
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    MAP_Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
*/
}

void servo_timer_interrupt_handler(){
    servoMoving = 0;
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
}

void set_position(uint16_t x, uint16_t y){
    // check if the position is acheivable
    // check hypotenuse adn matlab workspace

    uint16_t hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
    uint16_t hypotenuse_angle = asin(x / hypotenuse);

    uint16_t inner_angle = acos((pow(hypotenuse, 2) + pow(LINK_1, 2) - pow(LINK_2, 2)) / (2 * hypotenuse + LINK_1));
    uint16_t outer_angle = acos((pow(LINK_1, 2) + pow(LINK_2, 2) - pow(hypotenuse, 2)) / (2 * LINK_1 * LINK_2));

    uint16_t servo_1_angle = hypotenuse_angle - inner_angle;
    uint16_t servo_2_angle = 3.14 - outer_angle;

    // check trigonometric functions output, radiant or degrees
    //set_dutycycle
}