#ifndef GPIO_H
#define GPIO_H

/* 
* @file gpio.h
* @brief Driver GPIO para Arduíno UNO.
*/

/* 
* @brief Inicializa o LED_BUILTIN como saída.
*/
void gpio_led_init(void);

/* 
* @brief Alterna o estado do LED_BUILTIN.
*/
void gpio_led_toggle(void);

/* 
* @brief On LED_BUILTIN.
*/
void gpio_led_on(void);

/* 
* @brief Off LED_BUILTIN.
*/
void gpio_led_off(void);

#endif