#include "motors.h"
#include "stdio.h"
#include "math.h"

/*
Set pwmconfig structs
*/

Timer_A_PWMConfig pwmConfig0 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        2000, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

Timer_A_PWMConfig pwmConfig2 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        2000, // this num/ 64k = period       OG 32000
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        64 // 5% or 1ms duty cycle             OG 3200
};

/*
Useful current position counter
*/
pos_t current_position;


void init_servo(void){

    /*
    Set up pwm pins module function
    */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,GPIO_PRIMARY_MODULE_FUNCTION);

    /*
    Set starting pwm configuration
    */
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
}

uint16_t angle_2_duty(uint16_t angle){
    uint16_t d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return d;
}


void set_servo(uint16_t duty1, uint16_t duty2){

    /*
    Check duty cycle plausibility
    */   
    if(duty1 >= SERVO_DUTY_CYCLE_MIN && duty1 <= SERVO_DUTY_CYCLE_MAX && duty2 >= SERVO_DUTY_CYCLE_MIN && duty2 <= SERVO_DUTY_CYCLE_MAX){
        
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig0);
        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig2);
    } 
}


void set_position(pos_t pos){

    /*
    Obtain angles from 2D points
    */
    uint16_t x = pos.x;
    uint16_t y = pos.y;

    uint16_t hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
    uint16_t hypotenuse_angle = asin(x / hypotenuse);

    uint16_t inner_angle = acos((pow(hypotenuse, 2) + pow(LINK_1, 2) - pow(LINK_2, 2)) / (2 * hypotenuse + LINK_1));            // the acos return the angle in radians
    uint16_t outer_angle = acos((pow(LINK_1, 2) + pow(LINK_2, 2) - pow(hypotenuse, 2)) / (2 * LINK_1 * LINK_2));

    uint16_t servo_1_angle = hypotenuse_angle - inner_angle;
    uint16_t servo_2_angle = 3.14 - outer_angle;

    /*
    Check on workspace
    */
    if(hypotenuse < (LINK_1 + LINK_2)){
        // conversion from radians to degree
        // conversion from radians to duty cycle

        // set servo
    }
}