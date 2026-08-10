#include <util/delay.h>
#include <avr/io.h>
#include "ports.h"

static const gpio_t LED_BUILTIN = {
    .ddr = &DDRB,
    .port = &PORTB,
    .pin = &PINB,
    .bit = PB5
};

int main(void)
{
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
