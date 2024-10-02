/*
 * UE00P.c
 *
 * Created: 2024/9/26 13:29:57
 * Author : A
 */ 

#define F_CPU 16000000

float GetValue();
void ADC_Init();
void Close();
void Open();

#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define ADCRes 1024.0
#define RefVolt 5.0

int open = 1;
int position = 0;
char buffer[16];
static volatile float temp = 0;

int main(void)
{
	EIMSK |= (1<<INT0) | (1<<INT1);
	ADC_Init();
	PORTD |= (1<<PORTD2) | (1<<PORTD3);
	DDRB |= (1<< DDB0) | (1<< DDB1);
	sei();
	PORTB |= (1 << PINB0);
	
	lcd_init(LCD_DISP_ON);
	lcd_puts("|--------------|");
	lcd_gotoxy(0, 1);
	lcd_puts("|--------------|");
	
	while (1)
	{
		if(open == 1){
			float tmp = GetValue();
			float volt = (tmp*(0.0048));
			if(volt<1.5){
				Close();
				_delay_ms(2000);
			}
		}
	}
}

ISR(INT0_vect){
	Open();
}

ISR(INT1_vect){
	Close();
}

ISR(ADC_vect){
	temp = ADCW;
	ADCSRA |= (1<<ADSC);
}

void ADC_Init(){
	ADMUX |= (1<<REFS0);
	ADMUX |= (1<<MUX0);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	ADCSRA |= (1<<ADEN);  
	ADCSRA |= (1<<ADIE); 
	ADCSRA |= (1<<ADSC);
}

float GetValue(){
	float result;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		result = temp;
	}
	return result;
}

void Close(){
	open = 0;
	lcd_gotoxy(0,0);
	lcd_puts("||||||||||||||||");
	lcd_gotoxy(0, 1);
	_delay_ms(1000);
	lcd_puts("||||||||||||||||");
	
	_delay_ms(80);
	PORTB &= ~(1 << PINC0);
	PORTB |= (1 << PINC1);
}

void Open(){
	open = 1;
	lcd_gotoxy(0,0);
	lcd_puts("|--------------|");
	lcd_gotoxy(0, 1);
	_delay_ms(1000);
	lcd_puts("|--------------|");
	
	_delay_ms(80);
	PORTB &= ~(1 << PINC1);
	PORTB |= (1 << PINC0);
}