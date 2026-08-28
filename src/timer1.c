#include <avr/interrupt.h>
#include "timer1.h"
#include <avr/interrupt.h>
/*
 * @brief Implementação da função do timer 1.
 *  Fórmula para calcular o tempo no modo CTC é:
 *  TEMPO = PRESCALER * (1 + OCR1A) / FREQUÊNCIA CPU.
 */

void timer1_init_ctc(void)
{
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 499;
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1<<CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}