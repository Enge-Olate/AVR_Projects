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

/* @brief botão no pino 8. */
const gpio_t BUTTON_STATE = {
    .ddr = &DDRB,
    .port = &PORTB,
    .pin = &PINB,
    .bit = PB0};

/*
 * @brief Mapeamento para botões de controle do contador digital.
 * PB0 = ++;
 * PB1 = pause;
 * PB2 = reset;
 * PB3 = --.
 */

const gpio_t BUTTONS_STATE[] = {
    {&PORTC, &DDRC, &PINC, PC0},
    {&PORTC, &DDRC, &PINC, PC1},
    {&PORTC, &DDRC, &PINC, PC2},
    {&PORTC, &DDRC, &PINC, PC3},
    
};

/* @brief Mapeamento de hardware para o sensor DHT22*/
const gpio_t DHT22_PIN=
{
    .ddr = &DDRC,
    .port = &PORTC,
    .pin = &PINC,
    .bit = PC4
};