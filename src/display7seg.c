#include "display7seg.h"

/*
 * @brief Variáveis de controle das bases dos tranistores.
 */

static const hc595_t *p_hc595;
static const gpio_t *p_bases;
static volatile uint8_t buffer_digitos[4] = {};
static volatile uint8_t digito_atual = 0;

/*
 * @brief Mapeamento dos bytes para display de sete segmentos de catodo comum.
 */


static const uint8_t BYTES[11] = {
    0x3f, // 0
    0x06, // 1
    0x5b, // 2
    0x4f, // 3
    0x66, // 4
    0x6d, // 5
    0x7d, // 6
    0x07, // 7
    0x7f, // 8
    0x6f, // 9
    0x80, // .
};

/*
 * @brief Implementação da função display_init()
 */
void display_init(const hc595_t *shift_reg, const gpio_t bases[NUM_BASES])
{
    p_hc595 = shift_reg;
    p_bases = bases;

    hc595_init(p_hc595);
    //  Nesse loop setamos como saída as bases dos transistores e iniciando desligados, com a função gpio_set().
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_output(&p_bases[i]);
        gpio_set(&p_bases[i]);
    }
}

/*
 * @brief Implementação da função que atualiza o buffer do display.
 */
void display_update_buffer(uint16_t value)
{

    uint8_t temp[4];
    temp[0] = (value / 1000) % 10;
    temp[1] = (value / 100) % 10;
    temp[2] = (value / 10) % 10;
    temp[3] = value % 10;

    buffer_digitos[0] = temp[0];
    buffer_digitos[1] = temp[1];
    buffer_digitos[2] = temp[2];
    buffer_digitos[3] = temp[3];
}

// @brief Implementação da função para multiplexar as bases dos transistores.
void display_multiplex_step(void)
{
    // Desliga todas as bases.
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_set(&p_bases[i]);
    }
    uint8_t num = buffer_digitos[digito_atual];
    hc595_write_byte(p_hc595, BYTES[num]);
    gpio_clear(&p_bases[digito_atual]);
    digito_atual++;
    if (digito_atual >= 4)
        digito_atual = 0;
}