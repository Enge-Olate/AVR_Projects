#ifndef PORTS_H
#define PORTS_H

#include <avr/io.h>
#include <stdint.h>
/*
 * @brief Estrutura genérica para registradores do chip ATmega328p.
 */
typedef struct ports
{
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
    uint8_t bit;
} gpio_t;

/*
 * @brief Protótipos de funções genéricas de uso geral.
 */

/*
 * @brief Função para definir pino como saída.
 */
static inline void gpio_output(const gpio_t *gpio)
{
    *(gpio->ddr) |= (1 << gpio->bit);
}

/*
 * @brief Função para definir pino como entrada.
 */
static inline void gpio_input(const gpio_t *gpio)
{
    *(gpio->ddr) &= ~(1 << gpio->bit);
}

/* @brief Função para setar(HIGH / LOW) pino. */
static inline void gpio_set(const gpio_t *gpio)
{
    *(gpio->port) |= (1 << gpio->bit);
}

/*
 * @brief Função para limpar(Nível baixo / LOW) um pino.
 */
static inline void gpio_clear(const gpio_t *gpio)
{
    *(gpio->port) &= ~(1 << gpio->bit);
}

/*
 * @brief Função para alternar o estado de um pino.
 */
static inline void gpio_toggle(const gpio_t *gpio)
{
    *(gpio->port) ^= (1 << gpio->bit);
}

/*
 * @brief Funçao para ler um pino.
 */
static inline uint8_t gpio_read(const gpio_t *gpio)
{
    return (*(gpio->pin) >> gpio->bit) & 0x01;
}

#endif /* PORTS_H */