#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ports.h"
#include "gpio_config.h"
#include "display7seg.h"
#include "timer1.h"
#include "timer0.h"
#include "dht22.h"
#include "uart.h"

#define BUTTON &BUTTON_STATE
#define BUTTONS &BUTTONS_STATE
#define MAX_COUNT 9999
#define COUNT_INTERVAL_MS 600UL

/* @brief Definindo pinos do PORTB para o CI 74HC595. */
const hc595_t CI_PINS = {
    .data = {&PORTB, &DDRB, &PINB, PB0},
    .clock = {&PORTB, &DDRB, &PINB, PB1},
    .latch = {&PORTB, &DDRB, &PINB, PB2},
};

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
    RESET,
} command_t;

typedef enum
{
    STATE_PAUSED = 0,
    STATE_RUNNING_UP,
    STATE_RUNNING_DOWN
} state_t;

typedef struct
{
    uint16_t count;
    state_t current_state;
    state_t last_running_dir;
    uint32_t last_count_update;
    uint32_t last_temp_update;
    int16_t last_temp_tenths;
} app_t;

static command_t check_buttons(void)
{
    static uint8_t last_btn_state[4] = {1, 1, 1, 1};
    command_t command = NONE;
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t current_state = gpio_read(BUTTONS[i]);
        if (last_btn_state[i] == 1 && current_state == 0)
        {
            command = (command_t)(UP + i);
        }
        last_btn_state[i] = current_state;
    }

    return command;
}

static void process_command(command_t command, state_t *current_state,
                            state_t *last_running_dir, uint16_t *count)
{
    switch (command)
    {
    case PAUSE:
        *current_state = (*current_state == STATE_PAUSED) ? *last_running_dir : STATE_PAUSED;
        break;
    case UP:
        *last_running_dir = STATE_RUNNING_UP;
        *current_state = STATE_RUNNING_UP;
        break;
    case DOWN:
        *last_running_dir = STATE_RUNNING_DOWN;
        *current_state = STATE_RUNNING_DOWN;
        break;
    case RESET:
        *count = 0;
        display_update_buffer(*count);
        break;
    default:
        break;
    }
}

static void update_count(state_t state, uint16_t *count)
{
    if (state == STATE_RUNNING_UP && *count < MAX_COUNT)
        (*count)++;
    else if (state == STATE_RUNNING_DOWN && *count > 0)
        (*count)--;
    else
        return;

    display_update_buffer(*count);
}

static void update_temperature(state_t state, uint32_t now,
                               uint32_t *last_temp_update,
                               int16_t *last_temp_tenths)
{
    if (state != STATE_PAUSED ||
        (*last_temp_update != 0 && (now - *last_temp_update) < 2000))
    {
        return;
    }

    dht22_data_t current;
    if (dht22_read(&current))
    {
        *last_temp_tenths = (int16_t)(current.temperature * 10.0f);
        display_update_temp(*last_temp_tenths);

        int16_t absolute_temp = (*last_temp_tenths < 0) ?
                                -*last_temp_tenths : *last_temp_tenths;
         printf("TEMP: %s%d.%d C\r\n",
             (*last_temp_tenths < 0) ? "-" : "",
             absolute_temp / 10, absolute_temp % 10);
    }
    else
    {
        printf("DHT22: falha na leitura\r\n");
    }

    *last_temp_update = now;
}

static app_t app = {
    .count = 0,
    .current_state = 0,
    .last_running_dir = STATE_RUNNING_UP,
    .last_count_update = 0};

int main(void)
{
    display_init(&CI_PINS, BASES);
    timer1_init_ctc();
    timer0_init_ctc();
    setup_button();
    // dht22_init();
    uart_init(9600);
    uart_stdio_init();
    printf("UART: OK\r\n");
    display_update_buffer(app.count);

    while (1)
    {
        process_command(check_buttons(), &app.current_state,
                        &app.last_running_dir, &app.count);

        uint32_t now = timer0_millis();
        if ((now - app.last_count_update) >= COUNT_INTERVAL_MS)
        {
            update_count(app.current_state, &app.count);
            app.last_count_update = now;
        }
        update_temperature(app.current_state, now,
                           &app.last_temp_update, &app.last_temp_tenths);
        _delay_ms(1);
    }

    return 0;
}
