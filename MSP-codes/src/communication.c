#include "communication.h"
#include "motors.h"
#include "screen.h"

uint8_t TXData = 1; 
uint8_t RXData = 0;

int xory = 0;
int count = 2;      //starting with lifted pen
int track = 0; 

const eUSCI_UART_ConfigV1 uartConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // SMCLK Clock Source
    19,                                             // BRDIV = 19
    8,                                              // UCxBRF = 8
    85,                                             // UCxBRS = 85
    EUSCI_A_UART_NO_PARITY,                         // No Parity
    EUSCI_A_UART_LSB_FIRST,                         // LSB First
    EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
    EUSCI_A_UART_MODE,                              // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
    EUSCI_A_UART_8_BIT_LEN                          // 8 bit data length
};

void init_UART(){
    //pin configuration for UART
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //initialize UART
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    //UART enable
    UART_enableModule(EUSCI_A2_BASE);
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableSleepOnIsrExit();
    UART_transmitData(EUSCI_A2_BASE,0);

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
// get positions and pen status from raspberry pi 0
void UART_get_data(pos_t *pos){

    switch (xory) {
    case 0:
        pos -> x = UART_receiveData(EUSCI_A2_BASE);
        xory++;
        break;
    case 1:
        pos -> y = UART_receiveData(EUSCI_A2_BASE);
        xory++;
    case 2:
        if (count == 1){
            count++;
        }
        if(count == 2) {
            count = 0;
            pos->pen = !pos->pen;
        }
        set_position(pos);
        xory = 0;

        break;

    }
    
    RXData = UART_receiveData(EUSCI_A2_BASE);
    if((int)RXData == 255){
        pos->pen = !pos->pen;
        xory = 0;
        count++;
        //set_position(pos);
        updateScreen();
        for (int a = 0; a < 100; a++) {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
        for (int b = 0; b < 100; b++) {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    } else if ((int)RXData == 254){
        updateScreen();
    }
    Interrupt_disableSleepOnIsrExit();
}
// receive vector number from vectorization
void UART_get_vectors(void){
    RXData = UART_receiveData(EUSCI_A2_BASE);
    uint16_t multiplier;
    
    switch (track) {
    case 0:
        multiplier = 1;
        break;
    case 1:
        multiplier = 100;
        break;
    case 2:
        multiplier = 10000;
        get_vector = false;
        break;
    default:
        multiplier = 0;
        break;
    }

    numVec += (int) RXData * multiplier;
    track++;
}