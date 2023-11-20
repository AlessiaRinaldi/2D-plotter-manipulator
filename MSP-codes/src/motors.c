#include "motors.h"
#include "stdio.h"
#include "math.h"

/*
Set pwmconfig structs
*/

Timer_A_CompareModeConfig compareConfig_PWM = {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_RESET_SET,               // Toggle output b
        500,                                        // starting Duty Cycle
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
Useful current position counter
*/
pos_t current_position;


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


void set_servo(uint16_t duty1, uint16_t duty2){

    /*
    Check duty cycle plausibility
    */   
    if(duty1 >= SERVO_DUTY_CYCLE_MIN && duty1 <= SERVO_DUTY_CYCLE_MAX && duty2 >= SERVO_DUTY_CYCLE_MIN && duty2 <= SERVO_DUTY_CYCLE_MAX){
        
        
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