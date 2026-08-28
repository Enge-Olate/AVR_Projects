#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include "ports.h"
#include "gpio_config.h"
#include "display7seg.h"
#include "timer1.h"

#define BUTTON &BUTTON_STATE
#define BUTTONS &BUTTONS_STATE

/* @brief Definingo pinos do PORTB para o CI 74HC595. */
const hc595_t CI_PINS = {
    .data = {&PORTB, &DDRB, &PINB, PB0},
    .clock = {&PORTB, &DDRB, &PINB, PB1},
    .latch = {&PORTB, &DDRB, &PINB, PB2},
};


ISR(TIMER1_COMPA_vect)
{
    display_multiplex_step();
}


void setup_button(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_input_pullup(BUTTONS[i]);
    }
}

/*
* @brief Comandos para os botões.
*
*/

typedef enum
{
    NONE = 0,
    UP,
    DOWN,
    PAUSE,
    RESET

} comand_t;

/* @brief Estado do sistema. */
typedef enum
{
    STATE_PAUSED = 0,
    STATE_RUNNING_UP,
    STATE_RUNNING_DOWN
}state_t;

/* @brief Função para detectar borda de descida.*/
comand_t check_buttons(void)
{
    static uint8_t last_btn_state[4] = {1,1,1,1};
    comand_t comand = NONE;
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t current_state = gpio_read(BUTTONS[i]);
        if (last_btn_state[i] == 1 && current_state == 0)
        {
            switch (i)
            {
            case 0:
                comand = UP;
                break;
            case 1:
                comand = DOWN;
                break;
            case 2:
                comand = PAUSE;
                break;
            case 3:
                comand = RESET;
                break;
            default:
                break;
            }
        }
        last_btn_state[i] = current_state;
        
    }
    return comand;
}

int main(void)
{
    display_init(&CI_PINS, BASES);
    display_update_buffer(9999);
    timer1_init_ctc();
    setup_button();
    uint16_t counter = 0;
    state_t current_state = STATE_PAUSED;
    state_t last_running_dir = STATE_RUNNING_UP;
    uint16_t freq = 0;
    display_update_buffer(counter);

    while (1)
    {
        comand_t comand = check_buttons();

        switch (comand)
        {
        case PAUSE:
            if(current_state == STATE_PAUSED)
                current_state = last_running_dir;
            else
            {
                current_state = STATE_PAUSED;
            }
               
            break;
        case UP:
            last_running_dir = STATE_RUNNING_UP;
            current_state = STATE_RUNNING_UP;
            break;
        case DOWN:
            last_running_dir = STATE_RUNNING_DOWN;
            current_state = STATE_RUNNING_DOWN;
            break;
        case RESET:
            counter = 0;
            display_update_buffer(counter);
            break;
        default:
            break;
        }
        
        if (freq++ >= 20)
        {
            freq = 0;
            if (current_state == STATE_RUNNING_UP)
            {
                counter++;
                display_update_buffer(counter);
            }
            else if (current_state == STATE_RUNNING_DOWN)
            {
                counter--;
                display_update_buffer(counter);
            }

        }
        _delay_ms(10);
    }

    return 0;
}
