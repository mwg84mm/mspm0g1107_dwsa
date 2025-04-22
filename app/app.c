#include "app.h"


#define LED_CNTR 100                //100 * 10ms = 1s LED toggle time
uint8_t led_cntr;

#define HALL_CNTR 50                 //50 * 10ms = 500ms HAL refresh time
uint8_t hall_cntr;

volatile uint16_t Hall1 = 0;
volatile uint16_t Hall2 = 0;

volatile uint16_t armedHall1 = 0;
volatile uint16_t armedHall2 = 0;

#define HALL_DIFF   50

#define LED_OFF               DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_PIN );
#define LED_ON                DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_PIN );

void TIMER_0_INST_IRQHandler (void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)){
    case DL_TIMERA_INTERRUPT_ZERO_EVENT:
        F_TMR0 = 1;
        break;    
    default:
        break;
    }
}

void TIMER_1_INST_IRQHandler (void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_1_INST)){
    case DL_TIMERA_INTERRUPT_ZERO_EVENT:
        F_TMR1 = 1;
        NVIC_DisableIRQ(TIMER_1_INST_INT_IRQN);
        break;    
    default:
        break;
    }
}

void app_init(void)
{
    //TMR0 init    
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

//ADC0 init
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    led_cntr = 0;
    hall_cntr = 0;

    uart_init();

    dwsa_i2c_init();

}

// if |val1 - val2| > limit then res = 1, else res = 0
uint8_t diff (uint16_t val1, uint16_t val2, uint8_t limit)
{
    uint8_t res = 0;
    if (val1 > val2)   
    {
        if (val1 - val2 > limit)    res = 1;
    } 

    if (val2 > val1)   
    {
        if (val2 - val1 > limit)    res = 1;
    } 
    return res;
}

void app_loop(void)
{
     if (F_TMR0 == 1 )
        {
            F_TMR0 = 0;
            if (++led_cntr == LED_CNTR)
            {
                led_cntr = 0;
            }

            if (++hall_cntr == HALL_CNTR)
            {
                hall_cntr = 0;
                DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_HALL_EN_PIN );

                NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
                DL_TimerA_startCounter(TIMER_1_INST);

            }

        }

     if (F_TMR1 == 1 )
        {
            F_TMR1 = 0;
            DL_ADC12_startConversion(ADC12_0_INST);
            while ( (1 != F_ADC0) && (1 != F_ADC1)) __WFE();              
            Hall2 = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);
            Hall1 = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_1);
            F_ADC0 = 0; F_ADC1 = 0;
            DL_ADC12_enableConversions(ADC12_0_INST);
            
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_HALL_EN_PIN );
            if (F_DATAOUT)      uart_on();

            if (F_ARMED == 1)
            {
                uint8_t diff1 = diff (Hall1, armedHall1, HALL_DIFF);
                uint8_t diff2 = diff (Hall2, armedHall2, HALL_DIFF);

                if (diff1 + diff2 == 2) 
                {
                        LED_ON;
                }
                else
                {
                        LED_OFF;
                }
            }
        }

     if (F_UART == 1)
        {
            F_UART = 0;
            uart_update();    
        }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            F_ADC0 = 1;
            break;

        case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
            F_ADC1 = 1;
            break;

        default:
            break;
    }
}