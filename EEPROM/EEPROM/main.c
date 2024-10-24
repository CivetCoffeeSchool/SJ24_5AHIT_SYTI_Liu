/*
 * EEPROM.c
 *
 * Created: 2024/10/24 14:00:39
 * Author : A
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "lcd_definitions.h"

#define F_CPU 16000000UL
uint16_t EEMEM eeprom_stored_number;
volatile uint16_t random_number = 0;

void init_taster() {
	// Taster als Eingang konfigurieren
	DDRD &= ~(1 << DDD2);  // PD2 als Eingang für T1
	DDRD &= ~(1 << DDD3);  // PD3 als Eingang für T2
	
	// Interner Pull-up für die Taster
	PORTD |= (1 << DDD2);  // Pull-up aktivieren für T1
	PORTD |= (1 << DDD3);  // Pull-up aktivieren für T2
	
	// Externe Interrupts aktivieren
	EICRA |= (1 << ISC01) | (1 << ISC11);  // Trigger auf fallende Flanke für INT0 und INT1
	EIMSK |= (1 << INT0) | (1 << INT1);    // INT0 und INT1 aktivieren
	sei();  // Globale Interrupts aktiviere
}

void display_number(uint16_t number) {
	char buffer[16];
	lcd_clrscr();  // LCD leeren
	sprintf(buffer, "Number: %d", number);
	lcd_puts(buffer);
}

ISR(INT0_vect){//generate new number
	random_number =rand() % 1000;
	display_number(random_number);  // Zufallszahl zwischen 0 und 999 auf dem LCD anzeigen
}

ISR(INT1_vect){//save current number
	eeprom_update_word(&eeprom_stored_number, random_number);  // Zahl im EEPROM speichern
	
	// Bestätigung auf dem LCD anzeigen
	lcd_clrscr();
	lcd_puts("Saved!");
	_delay_ms(1000);  // Kurz "Saved!" anzeigen
	display_number(random_number);  // Zahl wieder anzeigen
}

int main(void)
{
    init_taster();
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();  // LCD löschen
	
	uint16_t stored_number = eeprom_read_word(&eeprom_stored_number);
	display_number(stored_number);  // Letzte gespeicherte Zahl anzeigen
	
    while (1) 
    {
    }
}



