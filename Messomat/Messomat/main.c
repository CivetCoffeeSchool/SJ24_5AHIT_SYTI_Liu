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

volatile uint8_t interval = 1;
volatile uint8_t sendFlag = 0;
volatile uint8_t seqNumber = 1;
volatile uint8_t ackReceived = 0;
volatile uint8_t retryCount = 0;

volatile uint8_t currentTemp;
volatile uint8_t currentHumidity;
volatile int8_t errorStatus;
volatile char buffer[20];

void timer1_init(void) {		// Set Timer1 für CTC-Modus
	TCCR1B |= (1 << WGM12);		// CTC-Modus
	OCR1A = 15624;				// 1 Sekunde bei 16 MHz und Prescaler 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);	// Prescaler 1024
	TIMSK1 |= (1 << OCIE1A);	// Output Compare A Interrupt aktivieren
}

void check_input(void){
	char command = uart_getc();
	switch (command) {
		case '1':
			OCR1A = 15626;
			interval = 1;
			break;
		case '4':
			OCR1A = 62504;
			interval = 4;
			break;
		case 'A':
			ackReceived = 1;
			break;
		case 'd':
			sendFlag = 1;
			break;
		case 'q':
			sendFlag = 0;
			break;
	}
}

void measure(void){
	if(dht_gettemperaturehumidity(&currentTemp,&currentHumidity)== DHT_ERROR_NOERR)
	{
		//anzeigen
		sprintf(buffer,"T:%dC H:%d%%",currentTemp,currentHumidity);
		lcd_clrscr();
		lcd_puts(buffer);
			
		// Nachricht senden
		//retryCount = 0;
		//ackReceived = 0;
		seqNumber++;
		//while (retryCount < 3 && !ackReceived) {
			sprintf(buffer, "DATE%d|HU%d|SN%d", currentTemp, currentHumidity, seqNumber);
			uart_putc(0x02);
			uart_puts(buffer);
			uart_putc(0x03);/*
			if (interval == 4) {
				_delay_ms(4000);
				} else if (interval == 1) {
				_delay_ms(1000);
			}
			retryCount++;
			check_input();
		}*/
			
		//LED
		if (!ackReceived) {
			PORTB |= (1 << PORTB0); // LED an
			} else {
			PORTB &= ~(1 << PORTB0); // LED aus
		}
	}
}

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();

	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	timer1_init();
	sei();
	
	sendFlag =1;
    while (1) 
    {
		check_input();
		_delay_ms(50);
    }
}


ISR(TIMER1_COMPA_vect) {
	if (sendFlag == 1){
		measure();
	}
}