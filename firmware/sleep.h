/*
 * sleep.h
 *
 *  Created on: Sep 8, 2013
 *      Author: cmerrill
 */

#ifndef SLEEP_H_
#define SLEEP_H_

#include <stdbool.h>

void sleep_init();
void sleep_until_next_step(void);
void sleep_deep(void);

bool sleep_from_sound_level(uint16_t level);


#endif /* SLEEP_H_ */
