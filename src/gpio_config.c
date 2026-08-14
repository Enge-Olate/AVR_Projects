#include "gpio_config.h"
/* 
* @brief Configuração para o led on-board.
 */
const gpio_t LED_BUILTIN = {
    .ddr = &DDRB,
    .port = &PORTB,
    .pin = &PINB,
    .bit = PB5
};
/* 
* @brief Mapeamento de hardware para base transistor.
*/
const gpio_t BASES[NUM_BASES] = {
    {&PORTD, &DDRD, &PIND, PD2},
    {&PORTD, &DDRD, &PIND, PD3},
    {&PORTD, &DDRD, &PIND, PD4},
    {&PORTD, &DDRD, &PIND, PD5},
};