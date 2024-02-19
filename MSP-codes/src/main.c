#include "msp.h"
#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "inithw.h"
//uint8_t TXData = 1;
//uint8_t RXData = 0;
/*const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        19,                                      // BRDIV = 13
        8,                                       // UCxBRF = 0
        85,                                      // UCxBRS = 37
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};*/

pos_t current_position;

int main(void){
    inithw();

    //Selecting P3.2 and P3.3 in UART mode and P1.0 as output (LED)
    //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Setting DCO to 24MHz (upping Vcore) -> CPU operates at 24 MHz!
    //FlashCtl_setWaitState(FLASH_BANK0, 1);
    //FlashCtl_setWaitState(FLASH_BANK1, 1);
    //PCM_setCoreVoltageLevel(PCM_VCORE1);
    //CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
    // Configuring UART Module 
    //UART_initModule(EUSCI_A2_BASE, &uartConfig);
    // Enable UART module 
    //UART_enableModule(EUSCI_A2_BASE);
    // Enabling interrupts 
    //UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    //Interrupt_enableInterrupt(INT_EUSCIA2);
    //Interrupt_enableSleepOnIsrExit();
    //UART_transmitData(EUSCI_A2_BASE,0);

    //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        
    //GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
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
        //if (get_vector){
            //UART_get_vectors();
        //} else {
            UART_get_data(&current_position); //set_position is here
        //}        
        //set_position(&current_position);
        //int count = 0;
        //RXData = UART_receiveData(EUSCI_A2_BASE);
        //if(RXData >= 3){
        //    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //    while(1);
        //}
        //Interrupt_disableSleepOnIsrExit();
    }
}



