/*
 * language.h
 *
 *  Created on: 30.09.2017
 *      Author: kurt
 */

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

char *getLang(int textnr);
char *getLangadd(int textnr, char *addtxt);

enum _LANG {
	Output_Power = 0,
	W_RFout,
	Output_SWR,
	Drive_Power,
	W_RFin,
	Efficiency, // 5
	Temperature1,
	Degree,
	PS_Voltage,
	PS_Current,
	PS_Power,  // 10
	POWER_SWR_CALIBRATION,
	no_data_set_DEFAULTs,
	save_values,
	RAW_ADC_Values,
	Real_Values, // 15
	Touch_Top_Left_Corner,
	Touch_Bottom_Right_Corner,
	touch_and_test_position,
	EXIT,
	SYSTEM_Settings, // 20
	SD_to_Flash,
	clear_flash,
	SDFINISHED,
	BTACTIVE,
	BTEMERGOFF,
	BTMENU,
	Line_Status,
	ON_switch,
	Emerg_OFF_switch,
	STANDBY_switch,
	PTT_from_TRX,
	SWITCH,
	Antenna_1,
	Antenna_2,
	PS_Enable,
	Power_Relais,
	FAN,
	PTT_out,
	Security,
	_OFF,
	Touch_Panel_Calibration,
	HISTORY,
	ANTFWDPWR,
	ANTREVPWR,
	FLTFWDPWR,
	FLTREVPWR,
	DRVFWDPWR,
	DRVREVPWR,
	LIMITS,
	AntSwitch,
	FORWARD,
	REVERSE,
	ONTEXT,
	NOTABSCHALTUNG,
	SWITCHON,
	ERRORTEXT_REASON,
	ERRORTEXT_TEMP,
	ERRORTEXT_USER,
	ERRORTEXT_MAXI,
	ERRORTEXT_MAXU,
	ERRORTEXT_MAXDRV,
	ERRORTEXT_MAXTXTIME,
	Temperature2,
	ERRORTEXT_TEMP2,
	WRONGVALUE,
	COUPLER1,
	COUPLER2,
	COUPLER3,
	MODETITLE1,
	MODETITLE2,
	GETVALMV,
	GETVALHELP1,
	GETVALHELP2,
	GETCALHELP1,
	GETCALHELP2,
	W_RFoutPeak,
	ERRORTEXT_MAXFLT,
	ERRORTEXT_MAXANT,
	ANTENNA,
	FILTERBANK,
	NEWCFG,
	LASTLANGENTRY
};

#endif /* LANGUAGE_H_ */
