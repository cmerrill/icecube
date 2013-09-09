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

bool sleep_deep_if_no_sound(uint16_t level);


#endif /* SLEEP_H_ */
