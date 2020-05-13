/*
 * controller.c
 *
 *  Created on: 11.10.2017
 *      Author: kurt
 */

#include <main.h>

// Automatik, Ablaufsteuerung und Sicherheitsfunktionen

char isTuning();

uint8_t state = STATE_POWERUP;
uint8_t oldstate = STATE_POWERUP;
uint8_t error = EMERG_NOERROR;
uint8_t old_pttstat = 255;
uint32_t uptime_ref1 = 0;
uint32_t uptime_ref2 = 0;

void control()
{
	if(state == STATE_EMERGENCYOFF)
	{
		// im Notstatus regagieren wir nur auf die ON Taste (in menu.c)
		// sobald diese gedrückt wird, geht der Status auf Standby
		uptime_ref1 = 0;
		uptime_ref2 = 0;
		oldstate = state;
		return;
	}

	if(oldstate == STATE_POWERUP && state == STATE_POWERUP)
	{
		// Gerät wurde eingeschaltet,
		// oder ist in Notaus gegangen
		// alles soll deaktiviert sein
		// es wird der Splashscreen und der ON Button dargestellt

		uptime_ref1 = 0;
		uptime_ref2 = 0;

		for(int i=ANTSEL1; i<MAXGPIOS; i++)
			gpio_stat[i] = 0;

		// LCD Beleuchtung einschalten
		gpio_stat[LCD_BACKLIGHT_ONOFF] = 1;

		// PA-DC Versorgung abgeschaltet, Bias aus, PTTout aus
		gpio_stat[PWRREL_OFF] = 1;

		oldstate = state;
		return;
	}

	if((oldstate == STATE_POWERUP || oldstate == STATE_EMERGENCYOFF) && state == STATE_STANDBY)
	{
		// ON Button wurde gedrückt: wir sind jetzt in Standby
		// Schalte das DC Netzteil auf die PA
		// Schalte Bänder und Antenne
		// PTT und BIAS bleibt aus
		gpio_stat[PWRREL_ON] = 1;	// Schalte ON Relais ein, bei "Aktiv" wird es wieder abgeschaltet

		gpio_stat[PWRREL_OFF] = 0;
		gpio_stat[BIAS_ONOFF] = 0;
		gpio_stat[PTTOUT] = 0;
	}

	if(oldstate == STATE_ACTIVE && state == STATE_STANDBY)
	{
		// Standby Button wurde gedrückt
		// lasse Netzteil an, deaktiviere Bias und PTT
		gpio_stat[BIAS_ONOFF] = 0;
		gpio_stat[PTTOUT] = 0;
	}

	if(oldstate == STATE_STANDBY && state == STATE_ACTIVE && pcfgdata->systemMode == 0)
	{
		// es wurde gerade AKTIV gedrückt,
		// schalte das PowerON Relais wieder aus, da die Versorgung in Selbsthaltung ist
		gpio_stat[PWRREL_ON] = 0;
	}

	if(state == STATE_ACTIVE)
	{
		// wir sind aktiv, folge der PTT Steuerung
		uint8_t pttstat = gpio_stat[PTTIN];
		if(pttstat != old_pttstat)
		{
			if(pttstat == 0)
			{
				// der Transceiver geht auf TX
				gpio_stat[BIAS_ONOFF] = 1;
				delay_1ms(100);
				gpio_stat[PTTOUT] = 1;
				setup_touchbutton_presscolor(&tb_buttons[TB_ACTIVE], RED);
			}
			else
			{
				gpio_stat[BIAS_ONOFF] = 0;
				gpio_stat[PTTOUT] = 0;
				setup_touchbutton_presscolor(&tb_buttons[TB_ACTIVE], GREEN);
			}
			if(menunum != MENU_SELECTION)
				draw_touchbutton(&tb_buttons[TB_ACTIVE]);
			old_pttstat = pttstat;
		}
	}
	else
	{
		txtime = 0;
	}

	// Lüfterkontrolle
	do_fan();

	// Überhitzung-Notabschaltung
	if(adc_realvals[UTEMP] > pcfgdata->Temp_limit && menunum != MENU_SETUP)
	{
		// merke aktuelle Zeit
		if(uptime_ref1 == 0) uptime_ref1 = uptime;

		// nach 10s langer Überhitzung schalte ab
		if((uptime - uptime_ref1) > 10000)
			emergency(EMERG_TEMP,pcfgdata->Temp_limit,adc_realvals[UTEMP]);
	}
	else
		uptime_ref1 = 0;

	if(adc_realvals[UTEMP2] > pcfgdata->Temp_limit && menunum != MENU_SETUP)
	{
		// merke aktuelle Zeit
		if(uptime_ref2 == 0) uptime_ref2 = uptime;

		// nach 10s langer Überhitzung schalte ab
		if((uptime - uptime_ref2) > 10000)
			emergency(EMERG_TEMP2,pcfgdata->Temp_limit,adc_realvals[UTEMP2]);
	}
	else
		uptime_ref2 = 0;

	// zu hoher Strom-Notabschaltung
	if(adc_realvals[IMESS] > pcfgdata->I_limit)
		emergency(EMERG_I,pcfgdata->I_limit,adc_realvals[IMESS]);

	// zu hohe Spannung-Notabschaltung
	if(adc_realvals[UMESS] > pcfgdata->U_limit)
		emergency(EMERG_U,pcfgdata->U_limit,adc_realvals[UMESS]);

	// zu hohe Steuerleistung-Notabschaltung
	if(pwrswr_input.fwd_watt > pcfgdata->Drive_limit && pcfgdata->systemMode == 0 && state == STATE_ACTIVE && gpio_stat[PTTIN] == 0)
	{
		if(fwd_drv_fail > 250)
			emergency(EMERG_DRV,pcfgdata->Drive_limit,pwrswr_input.fwd_watt);
	}
	else
		fwd_drv_fail = 0;

	// SWR Notabschaltung
	// um kurze Glitches zu ignorieren, muss der Fehler für min. 250ms bestehen

	// zu hohes Antennen SWR - Notabschaltung
	if(pwrswr_antenna.swr > 3 && gpio_stat[PTTIN] == 0 && state == STATE_ACTIVE)
	{
		if(swr_ant_fail >= 125)
			emergency(EMERG_ANTSWR,3,pwrswr_antenna.swr);
	}
	else
		swr_ant_fail = 0;

	// zu hohes Filter SWR - Notabschaltung
	if(pwrswr_filter.swr > 5 && gpio_stat[PTTIN] == 0 && state == STATE_ACTIVE)
	{
		if(swr_filter_fail >= 250)
			emergency(EMERG_FILTERSWR,5,pwrswr_filter.swr);
	}
	else
		swr_filter_fail = 0;

	// zu lange auf TX -> Notabschaltung
	if(state != STATE_ACTIVE || gpio_stat[PTTIN] == 1)
		txtime = 0;

	if(state == STATE_ACTIVE && ((txtime/1000)/60) > pcfgdata->Time_limit_minutes)
		emergency(EMERG_TXTIME,pcfgdata->Time_limit_minutes,(txtime/1000)/60);

	oldstate = state;

	// Funktionen welche zu einem neuen State führen

	// wenn keine Ub mehr da ist schalte von aktiv auf standby zurück
	if(adc_realvals[UMESS] < 11.0 && state == STATE_ACTIVE)
		switchto_standby();

	// prüfe Icom Tuning
	if(isTuning() && state == STATE_ACTIVE)
	{
		switchto_standby();	// wenn getunt wird gehe in Standby
		// bei der nächsten Loop wird oldstate=active und state=standby oben bearbeitet
	}
}

// Notabschaltung
void emergency_silent(uint8_t errnum)
{
	error = errnum;

	// alles AUS
	for(int i=ANTSEL1; i<MAXGPIOS; i++)
		gpio_stat[i] = 0;

	gpio_stat[PWRREL_OFF] = 1;

	state = STATE_EMERGENCYOFF;
}

void emergency(uint8_t errnum, float soll, float ist)
{
	error = errnum;

	// alles AUS
	for(int i=ANTSEL1; i<MAXGPIOS; i++)
		gpio_stat[i] = 0;

	gpio_stat[PWRREL_OFF] = 1;

	draw_menu_emergency(soll, ist);

	state = STATE_EMERGENCYOFF;
}

// prüft ob ein Icom gerade beim Tunen ist
// Pin 1 is the TKEY pin, when the ‘TUNE’ button is pressed, it’s pulled low by the tuner to tell the radio to start transmitting.
// Pin 2 is the TSTR pin to/from the microprocessor.
// Under normal circumstances the START pin is pulled high when a remote tuner is attached to tell the radio that the tuner is present.
// When you press ‘TUNE’ the radio pulls the pin low telling the tuner to start the matching sequence.
char isTuning()
{
	// TKEY:
	// + = es wird nicht getunt
	// GND = es wird getunt
	if(gpio_stat[TKEY] == 0) return 1;
	return 0;
}

