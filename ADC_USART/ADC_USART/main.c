/*
 * ADC_USART.c
 *
 * Created: 2024/10/17 13:22:15
 * Author : A
 */ 
#define F_CPU 16000000
#define BAUD 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

volatile uint16_t currentValue;//ISR verwendung von Variablen mit volatile
uint16_t tmpValue;

//Funktionsprototyp definieren
void sendCharacter(uint8_t data);
void sendString(uint8_t* str);
void ADC_init();
uint16_t ADC_Read();


int main(void)
{
	// poti an A0 ADC nur ueber Port C verfuegbar
	uint16_t adc_value;
	// Interrupts global deaktivieren => kann man sich sparen, da default 0
	cli();
	
	// Baudrate auf 9600 setzen => Wichtig: muss bei beiden gleich sein!
	// 9600 und 103 gilt fuer immer
	UBRR0=103;
	
	// Senden und Empfangen mit TXEN0 bzw, RXEN0 aktivieren
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	// Interrupt-basiertes Emfangen mit RXCIE0 aktivieren
	UCSR0B |= (1<<RXCIE0);
	
	//Di Verwendung von 8 Datenbits konfigurieren
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	
	ADC_init(void);
	// Interrupts global aktivieren
	sei();
	
	char buffer[10];
    /* Replace with your application code */
    while (1) 
    {
		//Atomic Block schützt die Zuweisung vor unerwünschter Unterbrechung bei deren Ausführung
		ATOMIC_BLOCK(ATOMIC_FORCEON){
			tmpValue =currentValue;
		}
		 
		 // Convert ADC value to string
		 sprintf(buffer, "ADC-Value: %d", tmpVal);
		 
		 // Send the ADC value via USART
		 sendString((uint8_t*)buffer);
    }
}

// Funktion zum Senden eines Zeichens
void sendCharacter(uint8_t data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

// Funktion zum Sender einer Zeichenkette
void sendString(uint8_t* str) {
	while (*str) { // Iterate over the string until null terminator is reached
		sendCharacter(*str++); // Send each character and increment the pointer
	}
}

void ADC_init(void) {
	ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin
	//REFS0 ist immer zu setzen
	
	//ADEN:		ADC aktivieren
	//ADATE:	Free Running mode
	//ADSC:		ADC starten(nur einmal erforderlich)
	//ADIE:		Interrupt fuer ADC aktivieren
	//ADPS 0-2:	Prescaler fuer ADC setzen (ergebensich 125kHz)
	ADCSRA = (1 << ADEN) | (1<<ADATE)
	ADCSRA |= (1<<ADSC);
	ADCSRA |= (1<<ADIE);
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)


// 	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1<<ADPS2);
	//ADCSRA |= (1<<ADSC);
	//singleconversion
}

uint16_t ADC_Read(void) {
	// Start single conversion
	ADCSRA |= (1 << ADSC);
	// Wait for conversion to complete
	while (ADCSRA & (1 << ADSC));
	return ADCW; // Return ADC value
}

//Wenn eine Messung abgeschlossen ist, wechselt der microC in diese ISR (unterbricht die Ausfuehrung der Arbeitschleife)
ISR(ADC_vect){
	currentValue = ADCW;
}