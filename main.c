#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include "gpio_config.h"
#include "display7seg.h"
#include "timer1.h"
#include "uart.h"

#define BUTTON &BUTTON_STATE
/*
 *   @brief Estrutura para os pinos do CI 74HC595.
 *   @param Pinos para controle (Arduíno UNO):
 *        DATA = 8;
 *        CLOCK = 9;
 *        LATCH = 10
 *
 */
const hc595_t CI_PINS = {
    .data = {&PORTB, &DDRB, &PINB, PB0},
    .clock = {&PORTB, &DDRB, &PINB, PB1},
    .latch = {&PORTB, &DDRB, &PINB, PB2},
};

/*
 * @brief Estrutura para controle.
 */
typedef struct
{
    char comando;
    uint8_t running;
} uart_control_t;

ISR(TIMER1_COMPA_vect)
{
    display_multiplex_step();
}

int main(void)
{

    uart_init(9600);
    uart_stdio_init();
    printf("\r\n");
    printf("ATmega 328p UART driver \r\n");
    printf("\n");
    printf("Controle para contagem\r\n");
    printf("Digite:\r\n");
    printf("c -> para começar a contagem.\r\n");
    printf("p -> para pausar a contagem.\r\n");
    printf("s -> para parar a contagem.\r\n");
    printf("d -> para parar decrementar a contagem.\r\n");
    gpio_output(&LED_BUILTIN);
    gpio_clear(&LED_BUILTIN);
    display_init(&CI_PINS, BASES);
    display_update_buffer(9999);
    timer1_init_ctc();
    uint16_t counter = 0;
    uart_control_t cmd = {
        .running = 0,
    };
    while (1)
    {
        if (uart_available())
        {
            cmd.comando = uart_getc();
            switch (cmd.comando)
            {
            case 'c':
            case 'C':
                cmd.running = 1;
                break;

            case 'd':
            case 'D':
                cmd.running = 1;
                break;
            case 'p':
            case 'P':
                cmd.running = 0;
                break;
            case 's':
            case 'S':
                cmd.running = 0;
                counter = 0;
                display_update_buffer(counter);
                break;


            default:
                printf("Comando não encontrado: %c\r\n", cmd.comando);
                break;
            }
        }
        if (cmd.running)
        {
            if (cmd.comando == 'c' || cmd.comando == 'C')
            {
                if (counter < 9999)
                {
                    counter++;
                }
                else
                {
                    counter = 0;
                }
            }
            else if (cmd.comando == 'd' || cmd.comando == 'D')
            {
                if (counter > 0)
                {
                    counter--;
                }
                else
                {
                    counter = 0;
                }
            }
            display_update_buffer(counter);
        }
        gpio_toggle(&LED_BUILTIN);
        _delay_ms(60);
    }
    return 0;
}
