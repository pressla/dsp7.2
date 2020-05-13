/*
 * antsel.c
 *
 *  Created on: 15.10.2017
 *      Author: kurt
 */

#include <main.h>

void showSelectedBandsForAntenna();

char ant_etxt[3][40] =
{
    "Select an antenna",
	"then touch/highlight the bands",
	"for this antenna"
};

char ant_dtxt[3][40] =
{
    "Waehle eine Antenne",
	"dann aktiviere die Baender",
	"fuer diese Antenne"
};

int do_button_antsw(int touchbutton)
{
	if(touchbutton == 9999) return 1;	// nicht benötigter Button

	if(touchbutton >= TB_ANTSW1 && touchbutton <= TB_ANTSW3)
	{
		tb_buttons[TB_ANTSW1].pressed = 0;
		tb_buttons[TB_ANTSW2].pressed = 0;
		tb_buttons[TB_ANTSW3].pressed = 0;

		tb_buttons[touchbutton].pressed = 1;
		draw_touchbutton(&tb_buttons[TB_ANTSW1]);
		draw_touchbutton(&tb_buttons[TB_ANTSW2]);
		draw_touchbutton(&tb_buttons[TB_ANTSW3]);

		pcfgdata->antsw = touchbutton - TB_ANTSW1;	// merke aktuelle Schaltstellung

		int px = 10;
		int py = 200;

		if(touchbutton == TB_ANTSW1) TFT_DrawFlash("a1",px,py);
		if(touchbutton == TB_ANTSW2) TFT_DrawFlash("a2",px,py);
		if(touchbutton == TB_ANTSW3) TFT_DrawFlash("a3",px,py);

		TFT_Fill_Rectangle(px+309, py,px+320,py+120,BLACK);

		showSelectedBandsForAntenna();

		return 1;
	}

	// und die Bandselection
	if(touchbutton == TB_BAND12) return 1; // ignorieren

	if(touchbutton >= TB_BAND1 && touchbutton <= TB_BAND11)
	{
		if(tb_buttons[touchbutton].pressed == 1)
		{
			// Band wird deaktiviert für diese Antenne
			pcfgdata->antsw_bandsel[pcfgdata->antsw][touchbutton-TB_BAND1] = 0;
		}
		else
		{
			// Band wird aktiviert für diese Antenne
			pcfgdata->antsw_bandsel[pcfgdata->antsw][touchbutton-TB_BAND1] = 1;
		}
		showSelectedBandsForAntenna();

		setup_modified = 1;
		return 1;
	}

	return 0;
}

void draw_touchbuttons_antswitch()
{
	for(int i=0; i<(BANDBUTTONS_ANZ-1); i++)
	{
		 draw_touchbutton(&tb_buttons[TB_BAND1+i]);
	}
	for(int i=TB_ANTSW1; i<=TB_ANTSW3; i++)
	{
		 draw_touchbutton(&tb_buttons[i]);
	}
}

void showSelectedBandsForAntenna()
{
	// aktiviere die entsprechenden Buttons
	for(int i=0; i<(BANDBUTTONS_ANZ-1); i++)
	{
		if(pcfgdata->antsw_bandsel[pcfgdata->antsw][i])
			tb_buttons[TB_BAND1+i].pressed = 1;
		else
			tb_buttons[TB_BAND1+i].pressed = 0;

		draw_touchbutton(&tb_buttons[TB_BAND1+i]);
	}

	// und schreibe es zur Grafik
	TFT_Fill_Rectangle(320, 200,TFT_WIDTH-152,320,BLACK);
	char s[100] = {0};
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<(BANDBUTTONS_ANZ-1); j++)
		{
			if(pcfgdata->antsw_bandsel[i][j]) sprintf(s+strlen(s),"%s ",bandbuttons_text[pcfgdata->bandmode][j]);
		}

		tft_print(&verdana_10ptFontInfo, s, 330, 207 + i*46, 2, WHITE , BLACK);
		*s = 0;
	}
}

// Grundmenü
void draw_antswmenu()
{
	char *p1 = ant_etxt[0];
	char *p2 = ant_etxt[1];
	char *p3 = ant_etxt[2];

	if(pcfgdata->language == 1)
	{
		p1 = ant_dtxt[0];
		p2 = ant_dtxt[1];
		p3 = ant_dtxt[2];
	}

	int tx = 20;
	int ty = 80;
	int dy = 30;

	tft_print(&lucidaConsole_18ptFontInfo, p1, tx, ty, 1, YELLOW, BLACK);
	tft_print(&lucidaConsole_18ptFontInfo, p2, tx, ty+dy*1, 1, YELLOW, BLACK);
	tft_print(&lucidaConsole_18ptFontInfo, p3, tx, ty+dy*2, 1, YELLOW, BLACK);

	do_button_antsw(pcfgdata->antsw + TB_ANTSW1);	// zeige aktivierte Antenne
}

// wird aus der Mainloop aufgerufen und schaltet die Antennen
void antennaswitch()
{
	/*
	 * Antennenauswahl je nach Bandzuweisung
	 * _selected_band hat das aktuell mit den Buttons ausgewählte Band
	 */

	// durchsuche die Antenne nach diesem AntBandbutton
	for(int ant=0; ant<3; ant++)
	{
		if(pcfgdata->antsw_bandsel[ant][_selected_band] == 1)
		{
			// es soll Antenne ant benutzt werden, aktiviere diese
			pcfgdata->antsw = ant;
		}
	}

	if(pcfgdata->antsw == 0)
	{
		// alle Relais ein -> Antenne-1 (diese ist bei abgefallenen Relais aktiv)
		gpio_stat[ANTSEL1] = 0;
		gpio_stat[ANTSEL2] = 0;
	}
	else if(pcfgdata->antsw == 1)
	{
		// Relais-1 ein, Relais-2 aus -> Antenne-2
		gpio_stat[ANTSEL1] = 0;
		gpio_stat[ANTSEL2] = 1;
	}
	else if(pcfgdata->antsw == 2)
	{
		// beide Relais aus -> Antenne-3
		gpio_stat[ANTSEL1] = 1;
		gpio_stat[ANTSEL2] = 1;
	}
}
