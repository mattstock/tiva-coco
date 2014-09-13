#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdint.h>

void uart_init(uint32_t f);
void UART_printstr(char *str);
void UART_printchars(char *str, uint16_t count);

#endif

