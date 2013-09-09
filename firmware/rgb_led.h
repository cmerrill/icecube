/*
 * rgb_led.h
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */

#ifndef RGB_LED_H_
#define RGB_LED_H_

typedef enum {
	RED = 0b001, GREEN = 0b010, BLUE = 0b100
} rgb_led_color_t;

/*
 * Function prototypes
 */
void rgb_led_init(void);
void rgb_led_set(uint8_t red, uint8_t green, uint8_t blue);
void rgb_led_start_pulse(uint8_t color);
void rgb_led_step(void);

void rgb_led_debug_step(uint8_t index);

#endif /* RGB_LED_H_ */
