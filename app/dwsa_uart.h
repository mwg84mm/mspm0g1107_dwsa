

#ifndef DWSA_UART_H_
#define DWSA_UART_H_

#include "ti_msp_dl_config.h"

extern char uartBuf[100];

void uart_init();
void uart_update();
void uart_on();

void writeUART(char * buf);

#endif /* DWSA_UART_H_ */
