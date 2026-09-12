#include "dht22.h"
#include "gpio_config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#define DHT22_TIMEOUT_US    100U
#define DHT22_STARTUP_DELAY_MS 1000U


/**
 * @brief Aguarda o pino atingir um nível lógico.
 *
 * @param level Nível esperado.
 * @param timeout_us Timeout em microssegundos.
 *
 * @return true se o nível esperado foi detectado.
 * @return false em timeout.
 */
static bool wait_for_level(uint8_t level, uint16_t timeout_us)
{
    while (timeout_us--)
    {
        if (gpio_read(&DHT22_PIN) == level)
        {
            return true;
        }

        _delay_us(1);
    }

    return false;
}

/**
 * @brief Recebe um bit transmitido pelo DHT22.
 */
static bool dht22_read_bit(uint8_t *bit)
{
    uint16_t pulse_width = 0;

    /* Aguarda o pulso LOW (~50 us). */
    if (!wait_for_level(0, DHT22_TIMEOUT_US))
    {
        return false;
    }

    /* Aguarda o início do pulso HIGH. */
    if (!wait_for_level(1, DHT22_TIMEOUT_US))
    {
        return false;
    }

    /* mede duração do HIGH */
    while (gpio_read(&DHT22_PIN))
    {
        pulse_width++;

        if (pulse_width >= DHT22_TIMEOUT_US)
        {
            return false;
        }

        _delay_us(1);
    }

    /*
     * 26~28 us -> bit 0
     * 70 us    -> bit 1
     */
    *bit = (pulse_width > 40U) ? 1U : 0U;

    return true;
}


void dht22_init(void)
{
    gpio_output(&DHT22_PIN);
    gpio_set(&DHT22_PIN);
    _delay_ms(DHT22_STARTUP_DELAY_MS);
}

bool dht22_read(dht22_data_t *data)
{
    uint8_t raw[5] = {0};
    bool frame_received = false;

    if (data == NULL)
    {
        return false;
    }

    /*--------------------------------------------------------------
     * Sinal de start
     *-------------------------------------------------------------*/

    gpio_output(&DHT22_PIN);

    gpio_clear(&DHT22_PIN);

    _delay_ms(1.2);

    gpio_set(&DHT22_PIN);

    _delay_us(30);

    gpio_input_pullup(&DHT22_PIN);

    /*--------------------------------------------------------------
     * Região crítica
     *-------------------------------------------------------------*/

    uint8_t sreg = SREG;

    cli();

    do
    {
        /*
         * Resposta do sensor:
         * 80 us LOW
         * 80 us HIGH
         */

        if (!wait_for_level(0, DHT22_TIMEOUT_US))
        {
            break;
        }

        if (!wait_for_level(1, DHT22_TIMEOUT_US))
        {
            break;
        }

        if (!wait_for_level(0, DHT22_TIMEOUT_US))
        {
            break;
        }

        /*----------------------------------------------------------
         * Leitura dos 40 bits
         *---------------------------------------------------------*/

        for (uint8_t byte = 0; byte < 5; byte++)
        {
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                uint8_t bit_value;

                if (!dht22_read_bit(&bit_value))
                {
                    goto exit_critical;
                }

                raw[byte] <<= 1;
                raw[byte] |= bit_value;
            }
        }

        frame_received = true;

    } while (0);

exit_critical:

    /*
     * Restaura o estado original das interrupções.
     */
    SREG = sreg;

    if (!frame_received)
    {
        return false;
    }

    /*--------------------------------------------------------------
     * Checksum
     *-------------------------------------------------------------*/

    uint8_t checksum =
        (uint8_t)(raw[0] +
                  raw[1] +
                  raw[2] +
                  raw[3]);

    if (checksum != raw[4])
    {
        return false;
    }

    /*--------------------------------------------------------------
     * Umidade
     *-------------------------------------------------------------*/

    uint16_t humidity_raw =
        ((uint16_t)raw[0] << 8) |
        raw[1];

    data->humidity = (float)humidity_raw / 10.0f;

    /*--------------------------------------------------------------
     * Temperatura
     *-------------------------------------------------------------*/

    uint16_t temperature_raw =
        ((uint16_t)raw[2] << 8) |
        raw[3];

    if (temperature_raw & 0x8000)
    {
        temperature_raw &= 0x7FFF;
        data->temperature =
            -((float)temperature_raw / 10.0f);
    }
    else
    {
        data->temperature =
            (float)temperature_raw / 10.0f;
    }

    return true;
}

bool dht22_read_temperature(float *temperature)
{
    dht22_data_t data;

    if (temperature == NULL)
    {
        return false;
    }

    if (!dht22_read(&data))
    {
        return false;
    }

    *temperature = data.temperature;

    return true;
}

bool dht22_read_humidity(float *humidity)
{
    dht22_data_t data;

    if (humidity == NULL)
    {
        return false;
    }

    if (!dht22_read(&data))
    {
        return false;
    }

    *humidity = data.humidity;

    return true;
}