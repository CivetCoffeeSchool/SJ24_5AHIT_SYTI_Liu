/*
 * lcd_definitions.h
 * Konfiguration fuer Atmega2560
 */

#ifndef LCD_LCD_DEFINITIONS_H_
#define LCD_LCD_DEFINITIONS_H_

#include <avr/io.h>

#define LCD_PORT         PORTA        /**< port for the LCD lines   */
#define LCD_DATA0_PORT   LCD_PORT     /**< port for 4bit data bit 0 */
#define LCD_DATA1_PORT   LCD_PORT     /**< port for 4bit data bit 1 */
#define LCD_DATA2_PORT   LCD_PORT     /**< port for 4bit data bit 2 */
#define LCD_DATA3_PORT   LCD_PORT     /**< port for 4bit data bit 3 */
#define LCD_DATA0_PIN    PA0          /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN    PA2          /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN    PA4          /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN    PA6          /**< pin for 4bit data bit 3  */
#define LCD_RS_PORT      LCD_PORT     /**< port for RS line         */
#define LCD_RS_PIN       PA1          /**< pin  for RS line         */
#define LCD_RW_PORT      LCD_PORT     /**< port for RW line         */
#define LCD_RW_PIN       PA3          /**< pin  for RW line         */
#define LCD_E_PORT       LCD_PORT     /**< port for Enable line     */
#define LCD_E_PIN        PA5          /**< pin  for Enable line     */

#endif /* LCD_LCD_DEFINITIONS_H_ */
