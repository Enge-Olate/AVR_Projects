#include <util/delay.h>
#include <avr/io.h>
#include "gpio.h"
#include "button.h"

int main(void)
{
    button_init();
    gpio_led_init();
    while (1)
    {
        if (button_read() == BUTTON_PRESSED)
        {
            gpio_led_toggle();
            _delay_ms(500);
        }
    }

    return 0;
}
