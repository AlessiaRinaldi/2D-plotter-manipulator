/*
This file includes the headers for the serial communication between Raspberry and MSP432
*/

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "msp.h"
#include <driverlib.h>
//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h> 

/*
Set 2D point struct
*/

typedef struct pos_t{
    uint8_t x;
    uint8_t y;
    bool pen;
} pos_t;

void init_UART();



//leggo da rasp e metto in struct

//fare var globali 

//void init_communication();
//pos_t getpos();  prendere posizione x e y
#endif
