#include "motors.h"
#include "stdio.h"
#include "math.h"

/*
Set pwmconfig structs and 2D point struct
*/

Timer_A_PWMConfig pwmConfig0 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

Timer_A_PWMConfig pwmConfig2 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        1280, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

pos_t current_position;


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
    //set servo

    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();
}

uint16_t angle_2_duty(uint16_t angle){
    uint16_t d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return d;
}

void set_servo(uint16_t duty1, uint16_t duty2){
    // check if the duty cycle is valid

    if(duty1 >= SERVO_DUTY_CYCLE_MIN && duty1 <= SERVO_DUTY_CYCLE_MAX && duty2 >= SERVO_DUTY_CYCLE_MIN && duty2 <= SERVO_DUTY_CYCLE_MAX){
        
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
    } 
}

void set_position(pos_t pos){
    // check if the position is acheivable
    // check hypotenuse adn matlab workspace
    uint16_t x = pos.x;
    uint16_t y = pos.y;

    uint16_t hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
    uint16_t hypotenuse_angle = asin(x / hypotenuse);

    uint16_t inner_angle = acos((pow(hypotenuse, 2) + pow(LINK_1, 2) - pow(LINK_2, 2)) / (2 * hypotenuse + LINK_1));            // the acos return the angle in radians
    uint16_t outer_angle = acos((pow(LINK_1, 2) + pow(LINK_2, 2) - pow(hypotenuse, 2)) / (2 * LINK_1 * LINK_2));

    uint16_t servo_1_angle = hypotenuse_angle - inner_angle;
    uint16_t servo_2_angle = 3.14 - outer_angle;

    // conversion from radians to degree
    // conversion from radians to duty cycle

    //test

    //return tmp;

    // set servo
}