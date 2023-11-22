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

Timer_A_CompareModeConfig compareConfig_PWM = {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_RESET_SET,               // Toggle output b
        SERVO_DUTY_CYCLE_MIN,                       // starting Duty Cycle
};

const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,                  // SMCLK = 3 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_1,              // SMCLK/12 = 250 KhZ
        10000,                                      // 40 ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,             // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,        // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                            // Clear value
};

/*
Useful current position counter of the arm and the pen
*/
pos_t current_position;
bool past_pen_position;


void init_servo(void){
    /* Configures P2.4 to PM_TA0.1 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configures P2.6 to PM_TA0.3 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configures P5.6 to PM_TA0.4 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring Timer_A0 for Up Mode and starting */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Configuring Timer_A2 for Up Mode and starting */
    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    /* Initialize compare registers to generate PWM */
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM); // For P2.6

    // For Port 2.4
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    compareConfig_PWM.compareValue = 3000;
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

    // For Port 5.6
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    compareConfig_PWM.compareValue = 9000;
    Timer_A_initCompare(TIMER_A2_BASE, &compareConfig_PWM);
}

uint16_t angle_2_duty(uint16_t angle){
    uint16_t d = angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) / 180;
    return d;
}

void set_pen(){

    if(past_pen_position != current_position.pen){

        // update past pen position
        past_pen_position = current_position.pen;

        if(current_position.pen == true){
            // the pen must be down
            compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
            compareConfig_PWM.compareValue = 500;
            Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

        } else{
            // the pen must be high
            compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
            compareConfig_PWM.compareValue = 2500;
            Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

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
        compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
        compareConfig_PWM.compareValue = duty1;
        Timer_A_initCompare(TIMER_A2_BASE, &compareConfig_PWM);

        // set elbow
        compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
        compareConfig_PWM.compareValue = 3000;
        Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
    } 
}

uint16_t angle_2_duty(uint16_t angle){
    // conversion from radians to duty cycle    
    // pi : (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN) = angle = x

    uint16_t duty = ((angle * (SERVO_DUTY_CYCLE_MAX - SERVO_DUTY_CYCLE_MIN)) / 3.1415) + SERVO_DUTY_CYCLE_MIN;
    return duty;
}


void set_position(pos_t pos){

    /*
    Obtain angles from 2D points
    */

    current_position = pos;             //check this, maybe it's not the right place

    uint16_t x = pos.x;
    uint16_t y = pos.y;

    uint16_t hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
    uint16_t hypotenuse_angle = asin(x / hypotenuse);

    uint16_t inner_angle = acos((pow(hypotenuse, 2) + pow(LINK_1, 2) - pow(LINK_2, 2)) / (2 * hypotenuse + LINK_1));            // the acos return the angle in radians
    uint16_t outer_angle = acos((pow(LINK_1, 2) + pow(LINK_2, 2) - pow(hypotenuse, 2)) / (2 * LINK_1 * LINK_2));

    uint16_t servo_1_angle = hypotenuse_angle - inner_angle;
    uint16_t servo_2_angle = 3.14 - outer_angle;

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