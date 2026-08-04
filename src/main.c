#include <util/delay.h>
#include<avr/io.h>
#include "gpio.h"

int main(void)
{
    gpio_led_init();
    while (1)
    {
        gpio_led_toggle();
        _delay_ms(500);
    }
    
    return 0;
}
