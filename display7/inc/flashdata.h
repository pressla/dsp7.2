/*
 * flashdata.h
 *
 *  Created on: 20.09.2017
 *      Author: kurt
 */

#ifndef FLASHDATA_H_
#define FLASHDATA_H_

enum AUXINMODE {
	ROTARYSWITCH = 0,
	PTT4x
};

typedef struct _FDATA {
	uint32_t magic;

	// Touch -> Displaykoordinaten Referenzwerte
	int touch_left;
	int touch_right;
	int touch_top;
	int touch_bottom;

	// Setupwerte
	uint16_t I_limit;
	uint16_t U_limit;
	uint16_t Temp_limit;
	uint16_t Drive_limit;
	uint16_t Time_limit_minutes;
	uint16_t fan_temp;
	uint16_t pttdelay;
	uint16_t u16dummy1;	// für spätere Verwendung
	uint16_t u16dummy2;
	uint16_t u16dummy3;
	uint16_t u16dummy4;
	uint16_t u16dummy5;
	uint16_t u16dummy6;
	uint16_t u16dummy7;
	uint16_t u16dummy8;
	uint16_t u16dummy9;
	uint16_t u16dummy10;

	// Systemeinstellungen
	uint8_t language;
	uint8_t display_reverse;
	uint8_t tempunits;
	uint8_t maxpwrunits;
	uint8_t maxdrvpwrunits;
	uint8_t maxUunits;
	uint8_t maxIunits;
	uint8_t bandmode;
	uint8_t shuntR;
	uint8_t systemMode;		// 0=PA-Controller, 1=PwrSwrMeter
	uint8_t auxInputMode;
	uint8_t civ_adr1;
	uint8_t civ_adr2;
	uint8_t civ_adr3;
	uint8_t civ_adr4;
	uint8_t u8dummy1;
	uint8_t u8dummy2;
	uint8_t u8dummy3;
	uint8_t u8dummy4;
	uint8_t u8dummy5;
	uint8_t u8dummy6;
	uint8_t u8dummy7;
	uint8_t u8dummy8;
	uint8_t u8dummy9;
	uint8_t u8dummy10;

	// Kalibrationswerte
	float ant_W_low;
	float ant_W_high;
	float ant_mV_low;
	float ant_mV_high;
	float flt_W_low;
	float flt_W_high;
	float flt_mV_low;
	float flt_mV_high;
	float drv_W_low;
	float drv_W_high;
	float drv_mV_low;
	float drv_mV_high;
	float fdummy1;	// für spätere Verwendung
	float fdummy2;
	float fdummy3;
	float fdummy4;
	float fdummy5;
	float fdummy6;
	float fdummy7;
	float fdummy8;
	float fdummy9;
	float fdummy10;

	// aktuelle Einstellungen
	uint8_t antsw;
	uint8_t antsw_bandsel[3][11];	// Antennen 1 bis 3 und Bänder 1-11

    unsigned int crc16;
} t_fdata;

void setDefaults();
unsigned int crc16_messagecalc(unsigned char *data, int len);

extern t_fdata *pcfgdata;
extern int config_changed;
extern t_fdata flashdata[3];

#endif /* FLASHDATA_H_ */
