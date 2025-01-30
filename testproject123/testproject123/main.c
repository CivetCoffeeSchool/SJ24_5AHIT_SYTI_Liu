/*
 * testproject123.c
 *
 * Created: 2025/1/23 5:43:51
 * Author : A
 */ 


#define BAUDRATE 9600
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"

volatile char buffer[20];

void init_timer(void){
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<CS11) | (1<<CS12);
	TIMSK1 |= (1<<OCIE1A);
	OCR1A |= 15624;
}

void init_tasta(void){
	//pin pd2 3
	DDRD &= ~(1<<DDD2);
	DDRD &= ~(1<<DDD3);
	PORTD |= (1<<PORTD2);
	PORTD |= (1<<PORTD3);
	//interrupt
	EICRA |= (1<<ISC01);
	EIMSK |= (1<<INT0)
	EICRA |= (1<<ISC11);
	EIMSK |= (1<<INT1);
}

void init_pci(void){
	DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3));
	PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1)|(1<<PCINT2)|(1<<PCINT3);
	PCICR |= (1<<PCIE0);
}


int main(void)
{
	cli;
	init_timer();
	init_tasta();
	sei;
	
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts();
	
	uart_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
	sprintf(buffer,"myString");
	uart_puts(buffer);
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

ISR(TIMER1_COMPA_vect){
	
}

ISR(PCINT0){
	if(~PINB & pinToCheck){(1<<PINB4)
		
	}
}