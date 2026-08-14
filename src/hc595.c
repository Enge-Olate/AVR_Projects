#include <stdint.h>
#include "hc595.h"
#include "ports.h"


/* 
* @brief Implementação da função qeu inicia o CI 74HC595.
* @param PB0 = data; PB1 = clock; PB2 = latch.
*/
void hc595_init(const hc595_t *dev){
    gpio_output(&dev->data);
    gpio_output(&dev->clock);
    gpio_output(&dev->latch);
    gpio_clear(&dev->clock);
    gpio_clear(&dev->data);
}

/* 
* @brief Implementação da função que escreve no CI 74HC595.
*/
void hc595_write_byte(const hc595_t *dev, uint8_t data){
    for (uint8_t i = 7; i >= 0; i--)
    {   
        // Limpa o clock
        gpio_clear(&dev->clock);
        data & (1 << i) ? gpio_set(&dev->data):gpio_clear(&dev->data);
        gpio_set(&dev->clock);
    }
    gpio_clear(&dev->latch);
    gpio_set(&dev->latch);
    gpio_clear(&dev->latch);

    
}