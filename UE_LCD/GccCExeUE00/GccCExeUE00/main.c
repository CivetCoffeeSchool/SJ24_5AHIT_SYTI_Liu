/*
 * GccCExeUE00.c
 *
 * Created: 2024/9/25 20:54:12
 * Author : A
 */ 
#define  F_CPU 16000000

#include <avr/io.h>
#include "lcd.h"
#include "lcd_definitions.h"


int main(void)
{
    /* Replace with your application code */
	// LCD in er Betriebsart "LCD_DISP_ON_BLINK" initalisierien - 1x!
	lcd_init(LCD_DISP_ON_BLINK);
	
	lcd_puts("Hello, world!");//2*16(0-15)
	/* Replace with your application code */
    while (1) 
    {
    }
}

