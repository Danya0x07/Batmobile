#pragma once
#include <avr/io.h>

extern volatile _Bool new_command;
void UART_init(uint32_t);
uint8_t UART_getc(void);
void UART_send(uint8_t);
