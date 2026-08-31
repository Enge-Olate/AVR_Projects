#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer0.h"

static volatile uint32_t g_ms_ticks = 0;

/**
 * @brief ISR do Timer0.
 */
ISR(TIMER0_COMPA_vect)
{
    g_ms_ticks++;
}

/**
 * @brief Inicializa Timer0 para tick de 1 ms.
 *
 * F_CPU = 16 MHz
 * Prescaler = 64
 *
 * 16 MHz / 64 = 250 kHz
 *
 * 250 contagens = 1 ms
 *
 * OCR0A = 249
 */
void timer0_init_ctc(void)
{
    cli();
    TCCR0A = 0;
    TCCR0B = 0;

    /* Modo CTC */
    TCCR0A |= (1 << WGM01);

    /* 1 ms */
    OCR0A = 249;

    /* Prescaler 64 */
    TCCR0B |= (1 << CS01) | (1 << CS00);

    /* Habilita interrupção */
    TIMSK0 |= (1 << OCIE0A);
    sei();
}

/**
 * @brief Retorna contador em ms.
 */
uint32_t timer0_millis(void)
{
    uint32_t value;

    uint8_t sreg = SREG;
    cli();

    value = g_ms_ticks;

    SREG = sreg;

    return value;
}