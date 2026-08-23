/**
 * @file uart.h
 * @brief Driver UART para ATmega328P.
 *
 * Implementa comunicação serial assíncrona utilizando a USART0
 * do ATmega328P.
 *
 * Também fornece integração com printf() através da biblioteca stdio.
 *
 * @author Márcio Olate
 * @date 23 de agosto de 2026
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicializa a UART.
 *
 * Configura a USART0 para operação assíncrona:
 * - 8 bits de dados
 * - 1 bit de parada
 * - Sem paridade
 *
 * @param baudrate Taxa de transmissão em bps.
 *
 * @note Exemplo:
 * @code
 * uart_init(9600);
 * @endcode
 */
void uart_init(uint32_t baudrate);

/**
 * @brief Envia um caractere.
 *
 * Função bloqueante.
 *
 * @param c Caractere a transmitir.
 */
void uart_putc(char c);

/**
 * @brief Envia uma string terminada em '\0'.
 *
 * @param str Ponteiro para string.
 */
void uart_puts(const char *str);

/**
 * @brief Recebe um caractere.
 *
 * Função bloqueante.
 *
 * @return Caractere recebido.
 */
char uart_getc(void);

/**
 * @brief Configura o printf() para transmitir pela UART.
 *
 * Após chamar esta função:
 *
 * @code
 * printf("Valor = %d\r\n", valor);
 * @endcode
 *
 * será enviado pela UART.
 */
void uart_stdio_init(void);

/**
 * @brief Verifica se existe dado recebido na UART.
 *
 * Função não bloqueante.
 *
 * @return
 *         1 -> Existe um byte disponível para leitura.
 *         0 -> Nenhum dado recebido.
 */
uint8_t uart_available(void);


#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */