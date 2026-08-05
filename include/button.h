/* 
* @file button.h
* @brief Biblioteca para gerenciamento de botões.
*/
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>

/* 
* @brief Estados possíveis do botão.
*/
typedef enum
{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} Button_State_t;

/* 
* @brief Inicializa o hardware do botão.
*/
void button_init(void);

/* 
* @brief Lê o estado do botão.
* @return BUTTON_PRESSED ou BUTTON_RELEASED.
*/
Button_State_t button_read(void);

#endif