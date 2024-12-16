/*
 * Messomat.c
 *
 * Created: 2024/12/5 13:16:47
 * Author : Yi
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
	char buffer[20];
	
	volatile uint8_t interval = 1;
	volatile uint8_t sendFlag = 0;
	volatile uint8_t seqNumber = 1;
	volatile uint8_t ackReceived = 0;
	volatile uint8_t retryCount = 0;

	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	
	void timer1_init(void) {		// Set Timer1 für CTC-Modus
		TCCR1B |= (1 << WGM12);		// CTC-Modus
		OCR1A = 15624;				// 1 Sekunde bei 16 MHz und Prescaler 1024
		TCCR1B |= (1 << CS12) | (1 << CS10);	// Prescaler 1024
		TIMSK1 |= (1 << OCIE1A);	// Output Compare A Interrupt aktivieren
	}

	ISR(TIMER1_COMPA_vect) {
		static uint8_t counter = 0;
		counter++;
		if (counter >= interval) {
			sendFlag = 1;
			counter = 0;
		}
	}
	
	ISR(USART_RX_vect) {
		char command = uart_getc();
		if (command == '1') {
			interval = 1; // 1-Sekunden-Intervall
			} else if (command == '4') {
			interval = 4; // 4-Sekunden-Intervall
			} else if (command == 'A') {
			ackReceived = 1; // ACK empfangen
		}
	}
	
    while (1) 
    {
		if (sendFlag){
			sendFlag = 0;
			if(dht_gettemperaturehumidity(&currentTemp,&currentHumidity)== DHT_ERROR_NOERR)
			{
				//anzeigen
				sprintf(buffer,"T:%dC H:%d%%",currentTemp,currentHumidity);
				lcd_clrscr();
				lcd_puts(buffer);
				
				// Nachricht senden
				retryCount = 0;
				ackReceived = 0;
				while (retryCount < 3 && !ackReceived) {
					sprintf(buffer, "DATE%d|HU%d|SN%d", currentTemp, currentHumidity, seqNumber);
					uart_putc(0x02);
					uart_puts(buffer);
					uart_putc(0x03);
					seqNumber++;
					_delay_ms(1000); // 1 Sekunde warten
					retryCount++;
				}
				
				//LED
				if (!ackReceived) {
					PORTB |= (1 << PORTB0); // LED an
					} else {
					PORTB &= ~(1 << PORTB0); // LED aus
				}
			}
		}
    }
}

