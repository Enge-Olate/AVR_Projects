#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ports.h"
#include "gpio_config.h"
#include "display7seg.h"
#include "timer1.h"
#include "timer0.h"

#define BUTTON &BUTTON_STATE
#define BUTTONS &BUTTONS_STATE

/* @brief Definindo pinos do PORTB para o CI 74HC595. */
const hc595_t CI_PINS = {
    .data = {&PORTB, &DDRB, &PINB, PB0},
    .clock = {&PORTB, &DDRB, &PINB, PB1},
    .latch = {&PORTB, &DDRB, &PINB, PB2},
};


void setup_button(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_input_pullup(BUTTONS[i]);
    }
}

typedef enum
{
    NONE = 0,
    UP,
    DOWN,
    PAUSE,
    STOP

} control_t;

control_t read_controls(void)
{
    if (gpio_read(BUTTONS[0]) == 0)
        return UP;
    if (gpio_read(BUTTONS[1]) == 0)
        return DOWN;
    if (gpio_read(BUTTONS[2]) == 0)
        return PAUSE;
    if (gpio_read(BUTTONS[3]) == 0)
        return STOP;
    return NONE;
}

int main(void)
{
    display_init(&CI_PINS, NUM_BASES);
    display_update_buffer(9999);
    timer1_init_ctc();
    uint16_t counter = 0;
    setup_button();
    control_t control = NONE;
    while (1)
    {
        control = read_controls();

        if (control != NONE)
        {

            switch (control)
            {
            case UP:
                counter++;

                break;
            case DOWN:
                counter--;

                break;
            case PAUSE:

                break;

            case STOP:
                counter = 0;

                break;
            case NONE:
            default:
                break;
            }
        }

        display_update_buffer(counter);
        _delay_ms(150);
    }

    return 0;
}
