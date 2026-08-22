#include <avr/interrupt.h>
#include <util/delay.h>
#include "gpio_config.h"
#include "display7seg.h"
#include "timer1.h"
#define BUTTON &BUTTON_STATE

const hc595_t CI_PINS = {
    .data = {&PORTB, &DDRB, &PINB, PB0},
    .clock = {&PORTB, &DDRB, &PINB, PB1},
    .latch = {&PORTB, &DDRB, &PINB, PB2},
};

ISR(TIMER1_COMPA_vect){
    display_multiplex_step();
}

int main(void)
{
    gpio_output(&LED_BUILTIN);
    gpio_clear(&LED_BUILTIN);
    display_init(&CI_PINS, BASES);      
    display_update_buffer(9999);
    timer1_init_ctc();
    uint16_t counter = 0;
    while (1 && counter <= 9999)
    {
        counter ++;
        display_update_buffer(counter);
        gpio_toggle(&LED_BUILTIN);
        _delay_ms(1000);
    }

    return 0;
}
