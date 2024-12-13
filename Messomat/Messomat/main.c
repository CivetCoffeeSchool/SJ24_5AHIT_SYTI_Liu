/*
 * Messomat.c
 *
 * Created: 2024/12/5 13:16:47
 * Author : A
 */ 
#define F_CPU 16000000UL
#define UART_BAUD_RATE 9600
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "dht.h"
#include "lcd.h"
#include "uart.h"


int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	uint8_t currentTemp;
	uint8_t currentHumidity;
	int8_t errorStatus;
	char buffer[10];
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	
	
    /* Replace with your application code */
    while (1) 
    {
		if(dht_gettemperaturehumidity(&currentTemp,&currentHumidity)== DHT_ERROR_NOERR)
		{
			sprintf(buffer,"T:%dC H:%d%%",currentTemp,currentHumidity);
			lcd_clrscr();
			lcd_puts(buffer);
			sprintf(buffer, "%d%d",currentTemp,currentHumidity);
			uart_putc(0x02);
			uart_puts(buffer);
			uart_putc(0x03);
			_delay_ms(DHT_TIMEOUT);
		}
    }
}

