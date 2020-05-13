/*
 * instrument.h
 *
 *  Created on: 03.10.2017
 *      Author: kurt
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

void setup_instrument(int x, int y);
void draw_forward(float pwr);
void draw_reverse(float pwr);
void delete_zeiger();
void draw_needle();

#endif /* INSTRUMENT_H_ */
