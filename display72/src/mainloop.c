/*
 * mainloop.c
 *
 *  Created on: 20.09.2017
 *      Author: kurt
 */

#include <main.h>

char s[30];

void mainloop()
{
static uint8_t oldptt = 1;

	// Funktionen welche immer laufen müssen
	process_touchbuttons();		// frage Touch ab
	adc_calculate_values();		// schreibe rohe und reale Werte in Arrays
	read_GPIOs();				// lese GPIOs in deren Struktur
	set_GPIO();					// wenn sich an der GPIO Struktur was geändert hat, setzte die Ausgänge entsprechend
	wifi();						// Kommunikation mit dem ESP8266
	set_LED();					// stelle onboard LED ein
	process_extbuttons();		// reagiere auf die externen Taster

	// wenn die PTT gerade gedrückt wurde, mache sofort CIV
	if(oldptt == 1 && gpio_stat[PTTIN] == 0)
	{
		civ_period = 50; // aber lasse ausreiched Zeit für einen laufenden Vorgang
	}
	oldptt = gpio_stat[PTTIN];

	if(civ_period == 0)
	{
		civ_request_frequency();// fordere Frequenz von einem Icom an
		civ_period = 500;
	}

	// Antennen und Bandumschaltung
	if(menunum != MENU_ANTSW && menunum != MENU_SWITCH &&menunum != MENU_STARTUP)
	{
		bandselection();		// mache die Bandselection, außer wenn man gerade im Antennenauswahlmenü ist
		antennaswitch();		// schalte die Antennen
	}

	// nur wenn das GUI aktiv ist
	if(menunum != MENU_STARTUP)
	{
		draw_values();				// Update der Bildschirmmenüs
		hist_insertdata();			// Update der History
	}

	if(menunum == MENU_SWITCH)	// im Switchmenü gibt es keinerlei Automatic
		return;

	control();					// Automaticfunktionen, Sicherheitsfunktionen
	statusline();
	check_wifi_commands();

	if(storeStartMenuFlag)
	{
		storeStartMenuFlag = 0;
		store_startmenu();
	}
}
