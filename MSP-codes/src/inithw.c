#include "inithw.h"
#include "motors.h"
#include "communication.h"
#include "screen.h"

Graphics_Context context;

void inithw(){
    /*
    stop wd timer, disable interrupts and initialize clock system
    */

    WDT_A_holdTimer();
    Interrupt_disableMaster();

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    init_UART();
    init_servo();
    init_screen();
}

