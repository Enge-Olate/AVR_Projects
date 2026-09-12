#ifndef DISPLAY7SET_H
#define DISPLAY7SET_H

#include <avr/io.h>
#include "hc595.h"
#include "gpio_config.h"
/* 
* @brief Protótipo das funções para o display.
 */

 void display_init(const hc595_t *shift_reg, const gpio_t bases[NUM_BASES]);
 void display_update_buffer(uint16_t value);
 void display_update_temp(int16_t temperature_tenths);
 void display_multiplex_step(void);

#endif/* DISPLAY7SET_H */