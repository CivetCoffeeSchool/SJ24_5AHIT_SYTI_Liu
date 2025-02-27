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
#include <avr/eeprom.h>
#include "dht.h"
#include "lcd.h"
#include "uart.h"

//Reservierungen
#define STATUS_LED_PIN  PORTB0
#define FAN_PIN         PORTB1
#define BTN_T1_PIN      PORTD2
#define BTN_T2_PIN      PORTD3
#define EEPROM_SIZE     22  // 2 header + 10*2 data

// Globale Statuse
volatile uint8_t interval = 1;
volatile uint8_t sendFlag = 0;
volatile uint8_t seqNumber = 1;
volatile uint8_t ackReceived = 0;
volatile uint8_t retryCount = 0;
volatile uint8_t measureTimeFlag = 0;
volatile uint8_t fanStatus = 0; // 0=off, 1=on
volatile uint8_t connectionLost = 0;

// EEPROM Specific
uint8_t EEMEM eepromStorage[EEPROM_SIZE];

//Sensor Daten
volatile int8_t currentTemp;
volatile int8_t currentHumidity;
volatile char displayBuffer[20];
volatile char txBuffer[32];

void timer1_init(void) {		// Set Timer1 für CTC-Modus
	TCCR1B |= (1 << WGM12);		// CTC-Modus
	OCR1A = 15624;				// 1 Sekunde bei 16 MHz und Prescaler 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);	// Prescaler 1024
	TIMSK1 |= (1 << OCIE1A);	// Output Compare A Interrupt aktivieren
}

ISR(TIMER1_COMPA_vect) {
	measureTimeFlag = 1;
}

void update_display(uint8_t stopped) {
	lcd_clrscr();
	if(stopped) {
		lcd_puts("**ME gestoppt**");
	} else {
		// Measurements
		sprintf(displayBuffer, "T:%dC H:%d%%", currentTemp, currentHumidity);
		lcd_puts(displayBuffer);
		
		// Fan status & interval
		lcd_gotoxy(0,1);
		sprintf(displayBuffer, "F:%s I:%ds %s",
			fanStatus ? "ON " : "OFF",
			interval,
			connectionLost ? "ERR!" : "    ");
		lcd_puts(displayBuffer);
	}
}

void send_data(){
	sprintf(txBuffer, "DATE%d|HU%d|SN%d", currentTemp, currentHumidity, seqNumber);
	uart_putc(0x02);
	uart_puts(txBuffer);
	uart_putc(0x03);
	retryCount++;
}


//	EEPROM: erste Stelle sag die neuest gespeicherten Daten aus, die zweite die Seriennummer
//	folgenden 10*2 positionen bilden ein Kreis
void store_in_eeprom(){
	uint8_t pos = eeprom_read_byte(&eepromStorage[0]);
	if(pos >= 10) pos = 0;
	
	eeprom_write_byte(&eepromStorage[pos*2 + 2], currentTemp);
	eeprom_write_byte(&eepromStorage[pos*2 + 3], currentHumidity);
	eeprom_write_byte(&eepromStorage[0], pos + 1);
	eeprom_write_byte(&eepromStorage[1], seqNumber);

}

void resend_eeprom_data(){
	uint8_t start_pos = eeprom_read_byte(&eepromStorage[0]);
	uint8_t start_seq = eeprom_read_byte(&eepromStorage[1]) > 10
					? eeprom_read_byte(&eepromStorage[1]) - 10
					: 0;
	
	for(uint8_t i=0; i<10; i++) {
		uint8_t pos = (start_pos + i) % 10;
		int8_t temp = eeprom_read_byte(&eepromStorage[pos*2 + 2]);
		int8_t hum = eeprom_read_byte(&eepromStorage[pos*2 + 3]);
		
		sprintf(txBuffer, "DATE%d|HU%d|SN%d", temp, hum, start_seq + i);
		uart_putc(0x02);
		uart_puts(txBuffer);
		uart_putc(0x03);
	}
}

void check_input(void){
	char command = uart_getc();
	switch (command) {
		case 0x06:
			ackReceived = 1;
			if(connectionLost) {
				connectionLost = 0;
				resend_eeprom_data();
			}
			break;
		case '1':
			OCR1A = 15624;
			interval = 1;
			TCNT1 = 0;// Reset timer
			update_display(0);
			break;
		case '4':
			OCR1A = 62499;
			interval = 4;
			TCNT1 = 0;  // Reset timer
			update_display(0);
			break;
		case 'r':  // Reset command
			retryCount = 0;
			ackReceived = 0;
			connectionLost = 0;
			PORTB &= ~(1 << STATUS_LED_PIN);
		case 'd':
			sendFlag = 1;
			retryCount = 0;  // Reset retry counter
			update_display(0);
			break;
		case 'q':
			sendFlag = 0;
			PORTB &= ~(1 << FAN_PIN);
			fanStatus = 0;
			update_display(1);
			break;
		case 'e':
			PORTB |= (1 << FAN_PIN);
			fanStatus = 1;
			update_display(0);
			break;
		case 'a':
			PORTB &= ~(1 << FAN_PIN);
			fanStatus = 0;
			update_display(0);
			break;
		case 's':
			uart_putc(0x02);
			uart_puts(fanStatus ? "FAN1" : "FAN0");
			uart_putc(0x03);
			break;
		
	}
}



int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	timer1_init();
	
	//I/O-Konfigurationen
	DDRB |=(1<< STATUS_LED_PIN)|(1<<FAN_PIN);//DDB0/DDB1
	PORTB &= ~((1 << PORTB0) | (1 << PORTB1));// Sicherstellen dass 2 LEDs am Anfang ausgeschaltet sind
	//Buttons (unnoetig)
	DDRD &= ~((1 << BTN_T1_PIN)|(1 << BTN_T2_PIN));
	PORTD |= (1 << BTN_T1_PIN)|(1 << BTN_T2_PIN); // pull-ups einschalten
	
	sei();
	update_display(1);
	
	while(1){
		check_input();
		if(ackReceived) {
			PORTB &= ~(1 << STATUS_LED_PIN);  // Turn off LED
			seqNumber++;
			retryCount = 0;
			ackReceived = 0;
		}
		
		if(measureTimeFlag) {
			measureTimeFlag = 0;
			if(dht_gettemperaturehumidity(&currentTemp, &currentHumidity) == DHT_ERROR_NOERR) {
				store_in_eeprom(); //alle Messungen werden gespeichert
				update_display(0);
				
				if (sendFlag) {
					if (retryCount < 3) {
						send_data();
						} else {
						PORTB |= (1 << STATUS_LED_PIN);  // Turn on LED after 3 retries
						connectionLost = 1;
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


