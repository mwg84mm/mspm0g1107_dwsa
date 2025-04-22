
#include "dwsa_i2c.h"

#include "app.h"
#include <stdio.h>

#include "I2C_communication.h"


#define MEMS_ADDR               0x18
#define MEMS_REG_WHOAMI         0x0F
#define MEMS_VALUE_WHOAMI       0x44

void dwsa_i2c_init(void)
{
  //     NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    uint8_t RX_data[2];
    I2C_ReadReg(MEMS_REG_WHOAMI, RX_data, 1);

    if (RX_data[0] ==  MEMS_VALUE_WHOAMI) 
    {
        sprintf(uartBuf, "I2C Initialized\n\r>"); writeUART(uartBuf);
    } 
    else 
    {
        sprintf(uartBuf, "Error Initializing I2C\n\r>"); writeUART(uartBuf);
    }
}
