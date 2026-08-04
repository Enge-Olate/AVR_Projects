/* 
* @file gpio.c
* @brief Implementação do driver GPIO.
*/
#include <avr/io.h>
#include "gpio.h"

/*
 * Arduíno UNO
 * LED_BUILTIN = PB5 = D13
 */

/* Definindo direção do bit. */
#define LED_DDR DDRB
/* Definindo o bit no portdb. */
#define LED_PORT PORTB
/* LED_BUILTIN onboard (portb5). */
#define LED_PIN PB5

void gpio_led_init()
{
    LED_DDR |= (1 << LED_PIN);
}

void gpio_led_on(void)
{
    LED_PORT |= (1 << LED_PIN);
}

void gpio_led_off(void)
{
    LED_PORT &= ~(1 << LED_PIN);
}

void gpio_led_toggle(void)
{
    LED_PORT ^= (1 << LED_PIN);
}