#include <avr/io.h>
#include "button.h"

#define BUTTON_DDR      DDRB
#define BUTTON_PIN      PB0
#define BUTTON_PORT     PORTB
#define BUTTON_PINREG   PINB
void button_init(void)
{
    BUTTON_DDR &= (1<< BUTTON_PIN);
    BUTTON_PORT |= (1<< BUTTON_PIN);
}

Button_State_t button_read()
{
    if (!(BUTTON_PINREG & (1<< BUTTON_PIN)))      
    {
        return BUTTON_PRESSED;
    }
    return BUTTON_RELEASED;
    
    
}