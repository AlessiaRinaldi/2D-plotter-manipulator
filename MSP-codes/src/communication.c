/*#include "communication.h"

const eUSCI_UART_ConfigV1 uartConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    13,
    0,
    37,
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_MSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
    EUSCI_A_UART_8_BIT_LEN
};

void init_UART(){
    //pin configuration for UART
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    //set DCO = 24Mz
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //initialize UART
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
    

    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    //UART enable
    UART_enableModule(EUSCI_A2_BASE);
}*/