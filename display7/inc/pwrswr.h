/*
 * pwrswr.h
 *
 *  Created on: 25.09.2017
 *      Author: kurt
 */

#ifndef PWRSWR_H_
#define PWRSWR_H_

void calc_PwrBridges();

#define SWRSMOOTHLEN 10

typedef struct
{
	float fwd_dBm;			// gemessene Leistung in dBm
	float fwd_dBmpeak;		// gemessene Spitzenleistung in dBm
	float fwd_watt;			// gemessene Leistung in Watt
	float fwd_peakwatt;		// gemessene Spitzenleistung in Watt
	float rev_dBm;			// gemessene Leistung in dBm
	float rev_watt;			// gemessene Leistung in Watt
	float swr;				// das berechnete SWR
	float imp_min;			// minimale Lastimpedanz
	float imp_max;			// maximale Lastimpedanz
	float refl_attenuation;	// Reflexionsdämpfung
	float smoothbuf[SWRSMOOTHLEN];
} t_pwrswr;

extern t_pwrswr pwrswr_input;
extern t_pwrswr pwrswr_filter;
extern t_pwrswr pwrswr_antenna;

#endif /* PWRSWR_H_ */
