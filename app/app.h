

#ifndef APP_H_
#define APP_H_


#include "ti_msp_dl_config.h"

#include "dwsa_uart.h"
#include "dwsa_i2c.h"

struct bitno { 
   unsigned b0 : 1;
   unsigned b1 : 1;
   unsigned b2 : 1;
   unsigned b3 : 1;
   unsigned b4 : 1;
   unsigned b5 : 1;   
   unsigned b6 : 1;
   unsigned b7 : 1;
};

typedef union 
{
    unsigned char B;
    struct bitno bits;
} Byte;

volatile Byte Flags;
#define F_TMR0      Flags.bits.b0
#define F_TMR1      Flags.bits.b1
#define F_UART      Flags.bits.b2
#define F_ADC0      Flags.bits.b3
#define F_ADC1      Flags.bits.b4
#define F_DATAOUT   Flags.bits.b5
#define F_ARMED     Flags.bits.b6

extern volatile uint16_t Hall1;
extern volatile uint16_t Hall2;

extern volatile uint16_t armedHall1;
extern volatile uint16_t armedHall2;

void app_init();
void app_loop();

#endif /* APP_H_ */
