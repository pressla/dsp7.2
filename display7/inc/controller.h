/*
 * controller.h
 *
 *  Created on: 18.10.2017
 *      Author: kurt
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

enum _SYSSTAT_ {
	STATE_POWERUP = 0,
	STATE_STANDBY,
	STATE_ACTIVE,
	STATE_EMERGENCYOFF,
};

enum _EMERGENCY_ {
	EMERG_NOERROR = 0,
	EMERG_TEMP,
	EMERG_TEMP2,
	EMERG_USER,
	EMERG_I,
	EMERG_U,
	EMERG_DRV,
	EMERG_TXTIME,
	EMERG_FILTERSWR,
	EMERG_ANTSWR,
	EMERG_MAXERROR
};

void emergency(uint8_t errnum, float soll, float ist);
void emergency_silent(uint8_t errnum);
int tempAutoCheck(float t1, int ptt);

extern uint8_t state;
extern uint8_t error;
extern int fan_request;

#endif /* CONTROLLER_H_ */
