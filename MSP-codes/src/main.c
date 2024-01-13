#include "msp.h"
//#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "motors.h"

bool count_xy = 0; // 0=x 1=y
uint8_t RXData = 0;

/*
config struct compare configuration (PWM)
*/
extern Timer_A_CompareModeConfig compareConfig_PWM ;
extern const Timer_A_UpModeConfig upConfig;

const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        13,                                      // BRDIV = 13
        0,                                       // UCxBRF = 0
        37,                                      // UCxBRS = 37
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_MSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};


int main(void)
{
    inithw();
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);//abilita il led sulla scheda
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring UART Module */
    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    /* Enable UART module */
    UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts */
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    while(1){
        MAP_PCM_gotoLPM0();
    }
}
//da rasp mi arriva 1 byte-> da msp devo ricere un uint8_t alla volta
void EUSCIA2_IRQHandler(void){
    //memorizzo lo stato dell'interrupt
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    //controllo che sia arrivato un interrupt
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        RXData = UART_receiveData(EUSCI_A2_BASE);
        for (int i = 0; i++ ; i<10){
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //accendi led
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

        
        Interrupt_disableSleepOnIsrExit();
    }
}


