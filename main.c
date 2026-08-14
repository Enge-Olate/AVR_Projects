#include <util/delay.h>
#include "ports.h"
#include "gpio_config.h"

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
