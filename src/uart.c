/**
 * @file uart.c
 * @brief Implementação do driver UART para ATmega328P.
 */

#include "uart.h"

#include <avr/io.h>
#include <stdio.h>

/**
 * @brief Calcula valor UBRR.
 *
 * Modo normal:
 *
 * UBRR = (F_CPU / (16 * BAUD)) - 1
 */
#define UART_UBRR(baud) ((F_CPU / (16UL * (baud))) - 1UL)

/* Forward declaration */
static int uart_printf_putchar(char c, FILE *stream);

/**
 * @brief Stream utilizada pelo printf().
 */
static FILE uart_stdout =
    FDEV_SETUP_STREAM(uart_printf_putchar, NULL, _FDEV_SETUP_WRITE);

/**
 * @brief Inicializa UART.
 */
void uart_init(uint32_t baudrate)
{
    uint16_t ubrr;

    ubrr = (uint16_t)UART_UBRR(baudrate);

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);

    /* RX enable + TX enable */
    UCSR0B =
        (1 << RXEN0) |
        (1 << TXEN0);

    /* 8N1 */
    UCSR0C =
        (1 << UCSZ01) |
        (1 << UCSZ00);
}

/**
 * @brief Envia um caractere.
 */
void uart_putc(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }

    UDR0 = c;
}

/**
 * @brief Envia string.
 */
void uart_puts(const char *str)
{
    while (*str)
    {
        uart_putc(*str++);
    }
}

/**
 * @brief Recebe caractere.
 */
char uart_getc(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }

    return UDR0;
}

/**
 * @brief Wrapper para printf().
 *
 * Converte '\n' para "\r\n".
 */
static int uart_printf_putchar(char c, FILE *stream)
{
    (void)stream;

    if (c == '\n')
    {
        uart_putc('\r');
    }

    uart_putc(c);

    return 0;
}

/**
 * @brief Habilita printf() pela UART.
 */
void uart_stdio_init(void)
{
    stdout = &uart_stdout;
}

/**
 * @brief Verifica se há dado disponível na recepção.
 *
 * @return
 *         1 se existe um byte recebido.
 *         0 caso contrário.
 */
uint8_t uart_available(void)
{
    return (UCSR0A & (1 << RXC0));
}