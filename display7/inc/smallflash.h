/*
 * smallflash.h
 *
 *  Created on: 01.01.2018
 *      Author: kurt
 */

#ifndef SMALLFLASH_H_
#define SMALLFLASH_H_

typedef struct _SFDATA {
	/*
	 * var0 Bitaufteilung:
	 * 0 ... immer 0 = Kennung
	 * 1 ... Automode on oder off
	 */
	uint8_t var0;
} t_smalldata;

void store_autoband(uint8_t onoff);
void restore_smallflash_data();
void store_startmenu();

extern int sf_autoband;
extern int sf_startmenu;

#endif /* SMALLFLASH_H_ */
