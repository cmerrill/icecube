/*
 * rgb_led.c
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */
#include <avr/io.h>
#include "rgb_led.h"

/*
 * Global constants
 */
static const struct {
	volatile uint8_t * const red;
	volatile uint8_t * const green;
	volatile uint8_t * const blue;
} rgb_led = {.red = &OCR0A, .green = &OCR1A, .blue = &OCR1A};

static const uint8_t debug_colors[] = {
		RED,
		GREEN,
		BLUE,
		RED | GREEN,
		GREEN | BLUE,
		BLUE | RED,
		RED | GREEN | BLUE
};

/*
 * Global variables
 */
static uint8_t led_fade = 0;

/*
 * Initialize the RGB LED PWM timer units
 */
void rgb_led_init(void) {
	// Setup IO port parameters
	PORTB |= (1 << 4) | (1 << 1) | (1 << 0);
	DDRB |= (1 << 4) | (1 << 1) | (1 << 0);

	// Setup timer parameters
	// Fast PWM mode, inverted output (set on compare match)
	// Internal system clock source, no divider
	TCCR0A = _BV(COM0A1) + _BV(COM0A0) + _BV(WGM00) + _BV(WGM01);
	TCCR0B = _BV(CS00);
	// T1 is special because it is designed for super fast PWM
	// Look at data sheet for settings details.
	// We need to make sure that T1 channel A & B are set the same
	// due to a hardware bug. See errata for ATTiny45 before rev D.
	TCCR1  = _BV(PWM1A) + _BV(COM1A1) + _BV(COM1A0) + _BV(CS10);
	GTCCR |= _BV(PWM1B) + _BV(COM1B1) + _BV(COM1B0);
	OCR1C = 255;

	// Set default brightness to "off"
	OCR0A = 0;
	OCR1A = 0;
	OCR1B = 0;
}


/*
 * Set RGB LED color & brightness
 */
void rgb_led_set(uint8_t red, uint8_t green, uint8_t blue) {
	*(rgb_led.red)  = red;
	*(rgb_led.green) = green;
	*(rgb_led.blue)  = blue;
}

/*
 * Trigger a pulse on specific colors
 */
void rgb_led_start_pulse(uint8_t color) {
	led_fade |= color;
}

/*
 * Adjust the brightness to follow a fade pattern
 */
static uint8_t next_fade_brightness(uint8_t last_output) {
	const uint8_t max_brightness = 254; // This should be even
	uint8_t output;

	if (last_output == 0) {
		output = max_brightness;
		return output;
	}
	else {
		return last_output - 4;
	}

}

/*
 * Step through the fade routine for each color.
 */
void rgb_led_step(void) {
	if (led_fade & RED) {
		*(rgb_led.red) = next_fade_brightness(*(rgb_led.red));
		if (*(rgb_led.red) == 0) led_fade &= ~RED;
	}
	if (led_fade & GREEN) {
		*(rgb_led.green) = next_fade_brightness(*(rgb_led.green));
		if (*(rgb_led.green) == 0) led_fade &= ~GREEN;
	}
	if (led_fade & BLUE) {
		*(rgb_led.blue) = next_fade_brightness(*(rgb_led.blue));
		if (*(rgb_led.blue) == 0) led_fade &= ~BLUE;
	}
}

void rgb_led_debug_step(uint8_t index) {
	uint8_t color = debug_colors[index % (sizeof(debug_colors)/sizeof(debug_colors[0]))];
	uint8_t red = (color & RED) ? 255 : 0;
	uint8_t green = (color & GREEN) ? 255 : 0;
	uint8_t blue = (color & BLUE) ? 255 : 0;
	rgb_led_set(red,green,blue);
}


