#include "uart.h"
#include <avr/interrupt.h>

volatile _Bool new_command = 0;
volatile static uint8_t rx_data;

void UART_init(uint32_t bdr)
{
    UBRR0H = (uint8_t)(bdr >> 8);
    UBRR0L = (uint8_t)bdr;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

ISR(USART_RX_vect)
{
    rx_data = UDR0;
    new_command = 1;
}

uint8_t UART_getc(void)
{
    while(!new_command);
    new_command = 0;
    return rx_data;
}

void UART_send(uint8_t ch)
{
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = ch;
}
