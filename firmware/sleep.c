/*
 * sleep.c
 *
 *  Created on: Sep 8, 2013
 *      Author: cmerrill
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "sleep.h"

/*
 * Global constants & variables
 */
static const uint16_t min_awake_level = 100; // 300 mV
static const uint16_t sample_below_reload_value;
static uint16_t samples_below = 800;

/*
 * Initialize watchdog timer to ensure wakeup from sleep mode.
 */
void sleep_init(void) {
	// Enable watchdog timer interrupt
	// Period is ~approx 1s
	WDTCR = _BV(WDIE) + _BV(WDP2) + _BV(WDP1);
}

/*
 * Sleep until the next timer tick or the next ADC signal.
 */
void sleep_until_next_step(void) {
	// Disable the ADC to save power.
	ADCSRA &= ~_BV(ADEN);

	// Set sleep mode to idle mode
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();

	// Sleep!
	sleep_cpu();

	// Disable processor sleep.
	sleep_disable();
}

/*
 * Go into a deep sleep mode
 */
static void sleep_deep(void) {

}

/**
 * If we haven't gotten any signal, go in to a deep sleep mode.
 * This will disable the timer and the ADC.
 */
bool sleep_deep_if_no_sound(uint16_t level) {
	if (samples_below == 0) {
		samples_below = 4;
		sleep_deep();
		return true;
	}
	if (level < min_awake_level) {
		samples_below -= 1;
	}
	else {
		samples_below = sample_below_reload_value;
	}
	return false;
}

/*
 * Watchdog timer wakeup interrupt. Does nothing but exist.
 */
ISR(WDT_vect) {}
