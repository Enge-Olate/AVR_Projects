#include <util/delay.h>
#include "ports.h"
#include "gpio_config.h"

#define BUTTON &BUTTON_STATE
int main(void)
{
    gpio_output(&BASES[0]);
    gpio_set(&BASES[0]);
    gpio_input(BUTTON);

    gpio_output(&LED_BUILTIN);
    gpio_clear(&LED_BUILTIN);
    while (1)
    {
        if (gpio_read(BUTTON))
        {
            gpio_toggle(&LED_BUILTIN);
            _delay_ms(200);
        }
    }

    return 0;
}
