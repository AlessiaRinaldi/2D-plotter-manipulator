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

    //init_UART();
    init_servo();
    init_screen();
}
/*
void init_UART() {
    // Configurazione dei pin per UART
    //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    // Configurazione della periferica UART
    const eUSCI_UART_ConfigV1 uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // Clock Source
        13,                                      // BRDIV
        0,                                       // UCxBRF
        37,                                      // UCxBRS
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_MSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One Stop Bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
    };

    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //initialize UART
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    // Abilita la periferica UART
    UART_enableModule(EUSCI_A2_BASE);
}*/
