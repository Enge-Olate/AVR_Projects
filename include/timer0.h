#ifndef TIMER0_H
#define TIMER0_H

#include <stdint.h>

/**
 * @brief Inicializa o Timer0 em mode ctc para gerar uma interrupção a cada 1ms.
 */
void timer0_init_ctc(void);

/**
 * @brief Retorna o contador de millisegundos do sistema.
 * @return uint32_t tempo em ms desde a inicialização do sistema.
 */
uint32_t timer0_millis(void);

#endif /* TIMMER0_H */