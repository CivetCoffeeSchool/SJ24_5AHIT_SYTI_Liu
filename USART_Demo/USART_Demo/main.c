/*
 * USART_Demo.c
 *
 * Created: 2024/10/3 13:19:40
 * Author : A
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
uint8_t receivedChar;

//Funktionsprototyp definieren
void sendCharacter(uint8_t data);

int main(void)
{
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
	
	// Interrupts global aktivieren
	sei();
	
	
    /* Replace with your application code */
    while (1) 
    {
		
		_delay_ms(500);
    }
}
// ISR fur das Emfangen eines Zeichens via UART
ISR(USART_RX_vect){//Vector
	receivedChar = UDR0;
	if(receivedChar == 65)
		sendCharacter()
	receivedChar++;
	sendCharacter(receivedChar);
	//UDR0=receivedChar;//:<
}

// Funktion zum Senden eines Zeichens
void sendCharacter(uint8_t data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void sendString(){
	sendCharacter()
}