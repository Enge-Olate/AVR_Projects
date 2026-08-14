#include <util/delay.h>
#include "ports.h"

static const gpio_t LED_BUILTIN = {
    .ddr = &DDRB,
    .port = &PORTB,
    .pin = &PINB,
    .bit = PB5
};

static const gpio_t BASES[] = {
    {&PORTD, &DDRD, &PIND, PD2},
    {&PORTD, &DDRD, &PIND, PD3},
    {&PORTD, &DDRD, &PIND, PD4},
    {&PORTD, &DDRD, &PIND, PD5},
};

int main(void)
{
    gpio_output(&BASES[0]);
    gpio_set(&BASES[0]);
    
    
    
    gpio_output(&LED_BUILTIN);
    gpio_clear(&LED_BUILTIN);
    for(;;){
        if(gpio_read(&LED_BUILTIN) == 0){
            gpio_toggle(&LED_BUILTIN);
            _delay_ms(200);
        }
    }

    return 0;
}
