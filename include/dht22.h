#ifndef DHT22_H_
#define DHT22_H_

#include <stdint.h>
#include <stdbool.h>


/* @brief Estrutura para leituras.*/
typedef struct
{
    float temperature;
    float humidity;
} dht22_data_t;

/* @brief Inicializa o sensor*/
void dht22_init(void);

/* @brief Realiza leituras de temperatura e umidade.*/
bool dht22_read(dht22_data_t *data);

/* @brief Realiza somente uma leitura de temperatura. */
bool dht22_read_temperature(float *temperature);

/* @brief Realiza somente uma leitura de umidade. */
bool dht22_read_humidity(float *humidity);

#endif