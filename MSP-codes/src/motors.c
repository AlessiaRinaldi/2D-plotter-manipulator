#include "motors.h"
#include "stdio.h"
#include "math.h"

/*

PWM SETUP:
the pwm pin 2.6 is connected to the third servo, the one that lift and put down the pen;
the pwm pin 2.4 is connected to the second servo, the elbow;
the pwm pin 5.6 is connected to the first servo, the fixed shoulder

Set pwmconfig structs

*/

Timer_A_PWMConfig pwm_config_shoulder =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_48,
        1280,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        32 
};

Timer_A_PWMConfig pwm_config_elbow =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_48,
        1280,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        78 
};
/*
Useful current position counter of the arm and the pen
*/
pos_t current_position;
bool past_pen_position;

uint16_t current_duty_shoulder = 0;
uint16_t current_duty_elbow = 0;


void init_servo(void){
    // configure 2.4 pin as shoulder pwm
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);

    // configure 5.6 pin as elbow pwm
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_config_shoulder);
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwm_config_elbow);

}

void set_pen(){

    if(past_pen_position != current_position.pen){

        // update past pen position
        past_pen_position = current_position.pen;

        if(current_position.pen == true){
            // the pen must be down
            // compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
            // compareConfig_PWM.compareValue = 500;
            // Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

        } else{
            // the pen must be high
            // compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
            // compareConfig_PWM.compareValue = 2500;
            // Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

        }
    }
}


void set_servo(uint16_t duty1, uint16_t duty2){

    set_pen();

    /*
    Check duty cycle plausibility
    */   
    if(duty1 >= SERVO_DUTY_CYCLE_MIN && duty1 <= SERVO_DUTY_CYCLE_MAX && duty2 >= SERVO_DUTY_CYCLE_MIN && duty2 <= SERVO_DUTY_CYCLE_MAX){

        // set shoulder
        pwm_config_shoulder.dutyCycle = duty1;
        pwm_config_elbow.dutyCycle = duty2;
        
        current_duty_shoulder = pwm_config_shoulder.dutyCycle;
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_config_shoulder);

        current_duty_elbow = pwm_config_elbow.dutyCycle;
        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwm_config_elbow);      
    } 
}

uint16_t angle_2_duty(float angle){
    // conversion from radians to duty cycle    
    // pi : (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) = angle = x

    uint16_t duty = ((angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN)) / 3.1415) + SERVO_DUTY_CYCLE_MIN;
    return duty;
}


void set_position(pos_t *pos){

    /*
    Obtain angles from 2D points
    */

    current_position = *pos;             //check this, maybe it's not the right place

    uint8_t x = pos -> x;
    uint8_t y = pos -> y;

    float hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
    float hypotenuse_angle = asin(x / hypotenuse);

    float inner_angle = acos((pow(hypotenuse, 2) + pow(LINK_1, 2) - pow(LINK_2, 2)) / (2 * hypotenuse * LINK_1));            // the acos return the angle in radians
    float outer_angle = acos((pow(LINK_1, 2) + pow(LINK_2, 2) - pow(hypotenuse, 2)) / (2 * LINK_1 * LINK_2));

    float servo_1_angle = hypotenuse_angle - inner_angle;
    float servo_2_angle = 3.14 - outer_angle;

    /*
    Check on workspace, better than this
    */
    if(hypotenuse < (LINK_1 + LINK_2)){
 
        // set servo
        uint16_t duty1 = angle_2_duty(servo_1_angle);
        uint16_t duty2 = angle_2_duty(servo_2_angle);
        set_servo(duty1, duty2);            // consider set_servo(duty1, duty2, pen); without relying in the global var
    }
}