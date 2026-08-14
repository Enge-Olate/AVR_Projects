#ifndef HC595_H
#define HC595_H

#include <avr/io.h>
#include "ports.h"

/* 
* @brief Estrutura de dados para mapeamento de hardware.
*/

typedef struct
{
    gpio_t data;
    gpio_t clock;
    gpio_t latch;
} hc595_t;


/*
* @brief Protótipo das funções para controle do ci 74HC595. 
 */
void hc595_init(const hc595_t  *dev);
void hc595_write_byte(const hc595_t *dev, uint8_t data);


#endif /* HC595_H */