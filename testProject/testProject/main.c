/*
 * testProject.c
 *
 * Created: 2024/12/7 17:34:15
 * Author : A
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		#include <avr/io.h>

		int main(void) {
			// PB0 als Eingang konfigurieren
			DDRB &= ~(1 << PB0);

			// Internen Pull-Up für PB0 aktivieren
			PORTB |= (1 << PB0);

			while (1) {
				// Abfrage des Pin-Zustands
				if (PINB & (1 << PB0)) {
					// PB0 ist HIGH (Taster nicht gedrückt)
					} else {
					// PB0 ist LOW (Taster gedrückt)
				}
			}
		}

    }
}

