/*
 * GCCCExecuatbleProjectUE00.c
 *
 * Created: 2024/9/19 13:30:43
 * Author : A
 */ 
#define  F_CPU 16000000

#include <avr/io.h>
#include "lcd.h"


int main(void)
{
	// LCD in er Betriebsart "LCD_DISP_ON_BLINK" initalisierien - 1x!
	lcd_init(LCD_DISP_ON_BLINK);
	
	lcd_puts("Hello, world!");//2*16(0-15)
    /* Replace with your application code */
    while (1) 
    {
    }
}

