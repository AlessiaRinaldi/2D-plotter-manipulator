#include "msp.h"
//#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "motors.h"

/*
config struct compare configuration (PWM)
*/
extern Timer_A_CompareModeConfig compareConfig_PWM ;
extern const Timer_A_UpModeConfig upConfig;


int main(void)
{
    inithw();

    while (1)
    {
        MAP_PCM_gotoLPM0();
    }
}

