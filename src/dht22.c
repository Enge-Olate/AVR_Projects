#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "dht22.h"
#include "ports.h"


#define DHT22_TIMEOUT_US 4000U
#define DHT22_STARTUP_DELAY_MS 1000U

/* @brief Pino de dados do sensor DHT22 (A4/PC4). */
const gpio_t DHT22_PIN = {
    .ddr = &DDRC,
    .port = &PORTC,
    .pin = &PINC,
    .bit = PC4
};

/*
* @brief Função para iniciar o PC4 como entrada de dados.
*/
void dht22_init(void)
{
    gpio_input(&DHT22_PIN);
    _delay_ms(DHT22_STARTUP_DELAY_MS);
}

/*
* @brief Função para realizar leituras do sensor.
*/
bool dht22_read(dht22_data_t *data)
{
    if (data == NULL)
    {
        return false;
    }
    uint8_t raw[5] = {0};
    uint16_t timeout;
    
    
    /* 1. Pulso de start: saída em LOW por 18 ms. */
    
    gpio_output(&DHT22_PIN);
    gpio_clear(&DHT22_PIN);
    _delay_ms(18);

    /* 2. Libera a linha. */
    // *ddr_reg  &= ~pin_mask;
    gpio_set(&DHT22_PIN);
    gpio_input(&DHT22_PIN);
    _delay_us(30);
    uint8_t sreg = SREG;

    /* @brief Desabilita interrupções.*/
    cli();

    timeout = DHT22_TIMEOUT_US;
    while (gpio_read(&DHT22_PIN))
    {
        if(--timeout == 0)
        {
            SREG = sreg; 
            return false;
        }
    }
    timeout = DHT22_TIMEOUT_US;
    while (!(gpio_read(&DHT22_PIN)))
    {
        if(--timeout == 0)
        {
            SREG = sreg; 
            return false;
        }
    }

    timeout = DHT22_TIMEOUT_US;
    while (gpio_read(&DHT22_PIN))
    {
        if(--timeout == 0)
        {
            SREG = sreg; 
            return false;
        }
    }

    /* @brief Leitura dos 40 bits */
    for (uint8_t i = 0; i < 40; i++)
    {
        /* Espera o pulso preparatório LOW (~50 us) */
        timeout = DHT22_TIMEOUT_US;
        while (!(gpio_read(&DHT22_PIN)))
        {
            if (--timeout == 0) { SREG = sreg; return false; }
        }

        /* Mede a largura do pulso HIGH */
        uint8_t width = 0;
        while (gpio_read(&DHT22_PIN))
        {
            width++;
            _delay_us(1);
            if (width > 120) { SREG = sreg; return false; }
        }

        uint8_t byte_idx = i / 8;
        raw[byte_idx] <<= 1;

        /* Limiar entre bit 0 (~28 us) e bit 1 (~70 us) */
        if (width > 35)
        {
            raw[byte_idx] |= 1;
        }
    }


    /*
     * Restaura o estado original das interrupções.
     */
    SREG = sreg;

    /* Checksum*/
    
    uint8_t checksum = (uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]);
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