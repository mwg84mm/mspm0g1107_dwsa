#include "dwsa_uart.h"
#include "app.h"
#include <stdio.h>

char uartBuf[100];
volatile uint8_t uartRd = 0;
volatile uint8_t uart_cntr = 0;


void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            uartRd = DL_UART_Main_receiveData(UART_0_INST);
            F_UART = 1;
            break;
        default:
            break;
    }
}

const char * uartBoardName = "DWS Addressable, MSPM0G1107";

void writeUART(char * buf)
{
    uint8_t i = 0;
    while (buf[i]!=0)
    {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, buf[i++]);
    }
}

void uart_init(void)
{
    //UART init    
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    sprintf(uartBuf, "RESET!\n\r>"); writeUART(uartBuf);
    sprintf(uartBuf, "board info: %s\n\r", uartBoardName);  writeUART(uartBuf); 
}


void uart_update(void)
{
    switch (uartRd)
    {
        case '?':    
            sprintf(uartBuf, "board info: %s\n\r", uartBoardName);  writeUART(uartBuf); 
            if (F_ARMED == 1)
            {
                sprintf(uartBuf, "BOARD ARMED\n\r");  writeUART(uartBuf);             
            }
            else 
            {
                sprintf(uartBuf, "BOARD NOT ARMED\n\r");  writeUART(uartBuf);             
            }
            sprintf(uartBuf, "h - Halls ADC value \n\r");  writeUART(uartBuf); 
            sprintf(uartBuf, "a - arm board \n\r");  writeUART(uartBuf); 
            sprintf(uartBuf, "d - data out \n\r>");  writeUART(uartBuf); 
            break;
        
        case 'a':    
            armedHall1 = Hall1;
            armedHall2 = Hall2;
            F_ARMED = 1;
            
            sprintf(uartBuf, "BOARD ARMED at position:\n\r");  writeUART(uartBuf);             
            sprintf(uartBuf, "Hall1: %#04x, ", armedHall1);  writeUART(uartBuf); 
            sprintf(uartBuf, "Hall2: %#04x\n\r>", armedHall2);  writeUART(uartBuf); 
            break;

        case 'h':    
            sprintf(uartBuf, "Hall1: %#04x, ", Hall1);  writeUART(uartBuf); 
            sprintf(uartBuf, "Hall2: %#04x\n\r>", Hall2);  writeUART(uartBuf); 
            break;

        case 'd': 
            if (F_DATAOUT==0)   
            {
                F_DATAOUT = 1;
                sprintf(uartBuf, "Data output:\n\rAdc\n\r");  writeUART(uartBuf);
            }
            else                
            {
                F_DATAOUT = 0;
                sprintf(uartBuf, "Data output off\n\r>");  writeUART(uartBuf);
            }                    
            break;


        default:
            sprintf(uartBuf, "received: %c\n\r>", uartRd);  writeUART(uartBuf);
            break;
    }
}

void uart_on(void)
{
    sprintf(uartBuf, "Hall1: %#04x, ", Hall1);  writeUART(uartBuf); 
    sprintf(uartBuf, "Hall2: %#04x\n\r", Hall2);  writeUART(uartBuf); 
}