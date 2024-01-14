#include "msp.h"
//#include "motors.h"
#include <driverlib.h>
#include "stdio.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include "inithw.h"
//#include "motors.h"
/*


bool count_xy = 0; // 0=x 1=y
uint8_t RXData = 0;


//config struct compare configuration (PWM)

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

    
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    // Setting DCO to 24MHz (upping Vcore) -> CPU operates at 24 MHz
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    // Configuring UART Module 
    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    // Enable UART module 
    UART_enableModule(EUSCI_A2_BASE);

    // Enabling interrupts
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
        for (int i = 0; i < 10; i++) {
            //GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //accendi led
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); // accendi led
            __delay_cycles(100000); // aggiungi un ritardo di 100000 cicli (adatta secondo necessità)
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // spegni led
            __delay_cycles(100000); // aggiungi un ritardo di 100000 cicli (adatta secondo necessità)
        }
        //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

        
        Interrupt_disableSleepOnIsrExit();
    }
}

*/


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//uint8_t TXData = 1;
uint8_t RXData = 0;


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
    /* Halting WDT  */
    WDT_A_holdTimer();

    /* Selecting P3.2 and P3.3 in UART mode and P1.0 as output (LED) */
    //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Setting DCO to 24MHz (upping Vcore) -> CPU operates at 24 MHz!*/
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    /* Configuring UART Module */
    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    /* Enable UART module */
    UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts */
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableSleepOnIsrExit();
    //////////////////////////
     // Accendi il LED

        // Aggiungi un ritardo di 0.5 secondi
    for(int i = 0; i< 10000; i++){
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // Spegni il LED
    //////////////////////
    while(1)
    {

        Interrupt_enableSleepOnIsrExit();
        PCM_gotoLPM0InterruptSafe();
    }
}

/* EUSCI A0 UART ISR - Echos data back to PC host */
void EUSCIA2_IRQHandler(void)
{
    //memorizzo lo stato dell'interrupt
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    //controllo che sia arrivato un interrupt
    
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        RXData = UART_receiveData(EUSCI_A2_BASE);

        
        for(int i = 0; i< 10000; i++){
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // Spegni il LED

        // Disabilita il controllo del sonno durante l'uscita dall'ISR
        Interrupt_disableSleepOnIsrExit();
    }

}