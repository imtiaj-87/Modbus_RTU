

#include "main.h"

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

void UARTSetup(void);



//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//***************************************************************************
//
// The UART Setup
//
//***************************************************************************

void UARTSetup(void)
{

        //MY_SLAVE_ID=17u;
        //uint8_t i;
        //
        // Run from the PLL at 120 MHz.
        // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
        // later to better reflect the actual VCO speed due to SYSCTL#22.
        //
        g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_240), 120000000);

        //
        // Enable the GPIO port that is used for the on-board LED.
        //
        MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

        //
        // Enable the GPIO pins for the LED (PN0).
        //
        MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

        //
        // Enable the peripherals used by this example.
        //
        MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        //
        // Enable processor interrupts.
        //
        MAP_IntMasterEnable();

        //
        // Set GPIO A0 and A1 as UART pins.
        //
        MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
        MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
        MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        //
        // Configure the UART for 115,200, 8-N-1 operation.
        //
        MAP_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                 UART_CONFIG_PAR_NONE));

        //
        // Enable the UART interrupt.
        //
        MAP_IntEnable(INT_UART0);
        MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void)
{
    uint32_t ui32Status;

    uint8_t i;

    //
    // Get the interrrupt status.
    //
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(MAP_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //

        data_in[i] = (uint8_t)MAP_UARTCharGetNonBlocking(UART0_BASE);

        i++;


        if(DataPos=8u & (data_in[0]=MY_SLAVE_ID))
        {
            DataPos=0;

            switch(data_in[1])
            {
            case 0x03:
            {
                MBProcessRegisters(data_in[1]);
                //MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
            }
            break;
            default:
            {
                MBException(0x01); //Illegal function code 01
                MBSendData(ResponseFrameSize);
                //MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
            }
            break;
            }
        }


    }

            /*MAP_UARTCharPutNonBlocking(UART0_BASE,
                                              a );
            MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);*/


        // MAP_UARTCharPutNonBlocking(UART0_BASE,
                                 // data );







        //
        // Blink the LED to show a character transfer is occuring.
        //
        // MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);


        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        //SysCtlDelay(g_ui32SysClock / (1000 * 3));

        //
        // Turn off the LED
        //
        // else MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

    }

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        MAP_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{

    MY_SLAVE_ID=17u;
    UARTSetup();

    //
    // Prompt for text to be entered.
    //
    //UARTSend((uint8_t *)"\033[2JEnter text: ", 16);


    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {

    }
}

