/*
 * beat_detect.c
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */
#include <avr/io.h>
#include "beat_detect.h"

void beat_detect_init(void) {
	ADMUX = _BV(MUX1) + _BV(MUX0);
	ADCSRA = _BV(ADPS2) + _BV(ADPS0);
	ADCSRB = 0;
	DIDR0 = _BV(ADC3D);
}

void beat_detect_start_conversion(void) {
	ADCSRA |= _BV(ADEN) + _BV(ADSC);
}

uint16_t beat_detect_get_last_conversion(void) {
	uint16_t out = ADCL;
	out += ((uint16_t)ADCH) << 8;
	return out;
}

bool beat_detected(uint16_t level) {
	return false;
}
