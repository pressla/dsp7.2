/*
 * bandsel.h
 *
 *  Created on: 19.12.2017
 *      Author: kurt
 */

#ifndef BANDSEL_H_
#define BANDSEL_H_

#define NUMBANDMODES 13

char bandbuttons_text[NUMBANDMODES][BANDBUTTONS_ANZ][10];
extern char bandmode_text[NUMBANDMODES][10];
extern char band_text_SW[NUMBANDMODES][7][10];

enum _BANDSEL {
	BANDRELAIS_OFF = 0,
	BANDRELAIS_1,
	BANDRELAIS_2,
	BANDRELAIS_3,
	BANDRELAIS_4,
	BANDRELAIS_5,
	BANDRELAIS_6,
};

extern uint8_t selected_bandrelais;

#endif /* BANDSEL_H_ */
