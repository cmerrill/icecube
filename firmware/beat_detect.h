/*
 * beat_detect.h
 *
 *  Created on: Sep 7, 2013
 *      Author: cmerrill
 */

#ifndef BEAT_DETECT_H_
#define BEAT_DETECT_H_

#include <stdbool.h>

void beat_detect_init(void);
void beat_detect_start_conversion(void);
uint16_t beat_detect_get_last_conversion(void);

bool beat_detected(uint16_t level);

#endif /* BEAT_DETECT_H_ */
