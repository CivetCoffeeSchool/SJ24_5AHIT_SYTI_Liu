/*
 * lcd_definitions.h
 * Konfiguration fuer Atmega2560
 */

#ifndef LCD_LCD_DEFINITIONS_H_
#define LCD_LCD_DEFINITIONS_H_

#include <avr/io.h>

#define LCD_PORT         PORTD        /**< port for the LCD lines   */
#define LCD_DATA0_PORT   LCD_PORT     /**< port for 4bit data bit 0 */
#define LCD_DATA1_PORT   LCD_PORT     /**< port for 4bit data bit 1 */
#define LCD_DATA2_PORT   LCD_PORT     /**< port for 4bit data bit 2 */
#define LCD_DATA3_PORT   LCD_PORT     /**< port for 4bit data bit 3 */
#define LCD_DATA0_PIN    PORTD0          /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN    PORTD0          /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN    PORTD0          /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN    PORTD0          /**< pin for 4bit data bit 3  */
#define LCD_RS_PORT      LCD_PORT     /**< port for RS line         */
#define LCD_RS_PIN       PORTD0          /**< pin  for RS line         */
#define LCD_RW_PORT      LCD_PORT     /**< port for RW line         */
#define LCD_RW_PIN       PORTD0          /**< pin  for RW line         */
#define LCD_E_PORT       LCD_PORT     /**< port for Enable line     */
#define LCD_E_PIN        PORTD0          /**< pin  for Enable line     */

#endif /* LCD_LCD_DEFINITIONS_H_ */
