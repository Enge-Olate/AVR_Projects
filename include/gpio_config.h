#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "ports.h"

/* * @brief Váriável para o LED_BUILTIN (led on-board).  */
extern const gpio_t LED_BUILTIN;


/* 
* @brief Array contendo a configuração das bases dos transistores.
* -BASES[0]: PD2;
* -BASES[1]: PD3;
* -BASES[2]: PD4;
* -BASES[3]: PD5;
*/
extern const gpio_t BASES[];
/* * @brief Quantidade de bases. */
#define NUM_BASES 4

#endif /* GPIO_CONFIG_H */