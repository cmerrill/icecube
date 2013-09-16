/*
 * beat_detect.c
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */
#include <avr/io.h>
#include "beat_detect.h"

#define BUFFER_LENGTH 11U
#define BUFFER_MIDDLE_INDEX (BUFFER_LENGTH/2)

void beat_detect_init(void) {
	ADMUX = _BV(MUX1) + _BV(MUX0);
	ADCSRA = _BV(ADPS2) + _BV(ADPS0);
	ADCSRB = 0;
	DIDR0 = _BV(ADC3D);
}

void beat_detect_start_conversion(void) {
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADSC);
}

uint16_t beat_detect_get_last_conversion(void) {
	uint16_t out = ADCL;
	out += ((uint16_t)ADCH) << 8;
	return out;
}

/*
 * Super simple "are we at a peak" function.
 * In the future use a threshold to avoid tiny bumps
 */
static bool compare_levels(uint16_t v1, uint16_t v2) {
	return (v1 < v2);
}

/*
 * Beat detection algorithm.
 * This is super simple -- can probably be improved by looking for peaks and not just
 */
bool beat_detected(uint16_t level) {
	static uint16_t buffer[BUFFER_LENGTH];
	static uint8_t current_index;

	uint16_t max = 0;
	uint8_t i;
	uint8_t center_index = (current_index < BUFFER_MIDDLE_INDEX) ?
			(current_index + BUFFER_MIDDLE_INDEX) :
			(current_index - BUFFER_MIDDLE_INDEX);

	// Store current buffer data
	buffer[current_index] = level;

	for (i = 0; i < BUFFER_LENGTH; i++) {
		if (i == center_index) continue;

		if (buffer[i] > max) {
			max = buffer[i];
		}
	}

	// Lets go move the pointer through the buffer.
	current_index++;
	if (current_index >= BUFFER_LENGTH) {
		current_index = 0;
	}

	return compare_levels(max, buffer[center_index]);
}
