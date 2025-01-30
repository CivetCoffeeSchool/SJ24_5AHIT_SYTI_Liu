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
volatile uint8_t sendFlag = 1;
volatile uint8_t seqNumber = 1;
volatile uint8_t ackReceived = 0;
volatile uint8_t retryCount = 0;
volatile uint8_t measureTimeFlag = 0;

volatile int8_t currentTemp;
volatile int8_t currentHumidity;
volatile char displayBuffer[20];
volatile char txBuffer[32];

volatile int8_t errorStatus;


void timer1_init(void) {		// Set Timer1 für CTC-Modus
	TCCR1B |= (1 << WGM12);		// CTC-Modus
	OCR1A = 15624;				// 1 Sekunde bei 16 MHz und Prescaler 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);	// Prescaler 1024
	TIMSK1 |= (1 << OCIE1A);	// Output Compare A Interrupt aktivieren
}

ISR(TIMER1_COMPA_vect) {
	measureTimeFlag = 1;
}

void check_input(void){
	char command = uart_getc();
	switch (command) {
		case 0x06:
			ackReceived = 1;
			break;
		case '1':
			OCR1A = 15624;
			interval = 1;
			TCNT1 = 0;// Reset timer
			break;
		case '4':
			OCR1A = 62499;
			interval = 4;
			TCNT1 = 0;  // Reset timer
			break;
		case 'd':
			sendFlag = 1;
			retryCount = 0;  // Reset retry counter
			break;
		case 'q':
			sendFlag = 0;
			break;
	}
}

void update_display(){
	sprintf(displayBuffer,"T:%dC H:%d%%",currentTemp,currentHumidity);
	lcd_clrscr();
	lcd_puts(displayBuffer);
}

void send_data(){
	sprintf(txBuffer, "DATE%d|HU%d|SN%d", currentTemp, currentHumidity, seqNumber);
	uart_putc(0x02);
	uart_puts(txBuffer);
	uart_putc(0x03);
	retryCount++;
}

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	timer1_init();
	
	DDRB |=(1<<DDB0);
	
	sei();
	
	while(1){
		check_input();
		if(ackReceived) {
			PORTB &= ~(1 << PORTB0);  // Turn off LED
			seqNumber++;
			retryCount = 0;
			ackReceived = 0;
		}
		
		if(measureTimeFlag) {
			measureTimeFlag = 0;
			if(dht_gettemperaturehumidity(&currentTemp, &currentHumidity) == DHT_ERROR_NOERR) {
				update_display();
				
				if (sendFlag) {
					if (retryCount < 3) {
						send_data();
						} else {
						PORTB |= (1 << PORTB0);  // Turn on LED after 3 retries
					}
				}
			}
		}
		// Handle LED state
		/*if (retryCount >= 3 && !ackReceived) {
			PORTB |= (1 << PORTB0);
		}*/
		
	}
}


