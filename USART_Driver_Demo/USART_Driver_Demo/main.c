/*
 * USART_Driver_Demo.c
 *
 * Created: 2024/10/10 13:47:21
 * Author : A
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

//Baudrate 9600
#define UART_BAUD_RATE 9600
int main(void)
{
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	unsigned int c;
	char buffer[7];

    /* Replace with your application code */
	while(1)
    {
        /*
         * Get received character from ringbuffer
         * uart_getc() returns in the lower byte the received character and 
         * in the higher byte (bitmask) the last receive error
         * UART_NO_DATA is returned when no data is available.
         *
         */
        c = uart_getc();
        if ( c & UART_NO_DATA )
        {//nothing to do
        }
        else
        {
			if(c == 'A')
				uart_puts("du hast mir ein A gesendet!");
            uart_putc( (unsigned char)c );
        }
    }
//    while (1) 
//    {
//		uart_puts("String stored in SRAM\n\r");
//		_delay_ms(1000);
//    }
}

