#include "msp.h"
#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "inithw.h"

pos_t current_position;

int main(void){
    inithw();

    for(int i = 0; i < 5000; i++){
        // starting delay
    }

    while(1){
        Interrupt_enableSleepOnIsrExit();
        PCM_gotoLPM0InterruptSafe();
        
    }
}

void EUSCIA2_IRQHandler(void){
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        if (get_vector){
            UART_get_vectors();
        } else {
            UART_get_data(&current_position);
        }        
    }
}



