/*
 * main.c
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "rgb_led.h"
#include "beat_detect.h"
#include "sleep.h"

/*
 * Global variable & constant definitions.
 */
static const uint16_t max_count = 390; // Sample at ~80 Hz
static uint16_t loop_counter = 390; // Initialize to max_count
static volatile bool sample_ADC = false;
static volatile bool ADC_done = false;
static volatile bool step_LED = true;


/*
 * Prototypes
 */
int main(void);
static void main_loop_init();


/*
 * Initialize system functions
 */
void init(void) {
	rgb_led_init();
	rgb_led_debug_step(1);
	beat_detect_init();
	rgb_led_debug_step(2);
	sleep_init();
	rgb_led_debug_step(3);
	main_loop_init();
	rgb_led_debug_step(4);
}

/*
 * Entry Point into program;
 */
int main(void){
	init();
	rgb_led_set(0,0,0);

	// Loop forever -- interrupt controlled from now on.
	sei();
	while(1) {
		uint16_t sample;

		if (sample_ADC) {
			sample_ADC = false;
			ADC_done = false;
			beat_detect_start_conversion();
		}

		if (step_LED) {
			step_LED = false;
			rgb_led_step();
		}

		if (ADC_done) {
			ADC_done = false;
			sample = beat_detect_get_last_conversion();
			if (beat_detected(sample)) {
				//FIXME: Add cooler color options here.
				rgb_led_start_pulse(RED);
			}

			if(!(sleep_deep_if_no_sound(sample))) {
				sleep_until_next_step();
			}
		}
		else {
			sleep_until_next_step();
		}
	}
	return 0;
}

/*
 * Initialize main loop interrupt settings
 */
static void main_loop_init(void) {
	TIMSK |= _BV(TOIE0);
	ADCSRA |= _BV(ADIE);
}

/*
 * Main control interrupt
 */
ISR(TIMER0_OVF_vect) {
	if (loop_counter == 0) {
		loop_counter = max_count;
		// Tell main loop so sample ADC and step the LED
		sample_ADC = true;
		step_LED = true;
	}

	// Decrement loop counter
	loop_counter--;
}

/*
 * ADC sample complete interrupt
 */
ISR(ADC_vect) {
	ADC_done = true;
}
