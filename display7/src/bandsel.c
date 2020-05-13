/*
 * bandsel.c
 *
 *  Created on: 11.10.2017
 *      Author: kurt
 */

#include <main.h>

void switch_bandrel();

int _selected_band  = 3;	// Index aus den 11 Band-Buttons
uint8_t selected_bandrelais;	// anzuziehendes bandrelais
int oldband = -1;

// insgesamt stehen 7 Bänder zur Verfügung.
// hier die Aufteilung ja nach PA Art

// die Bezeichnung des gewählten Band-Modus
char bandmode_text[NUMBANDMODES][10] =
{
		"SW Mode-1",
		"SW Mode-2",
		"SW Mode-3",
		"SW Mode-4",
		"SW Mode-5",
		"V/U/SHF  ",
		"LOW BANDS",
		"Mono 630m",
		"Mono 6m  ",
		"Mono 4m  ",
		"Mono 2m  ",
		"Mono 70cm",
		"Mono 23cm",
};

// Beschriftung der Relaisfunktion in den Diagnosefenstern
// wobei der jeweils erste Eintrag aktiv ist wenn kein Relais angezogen hat
char band_text_SW[NUMBANDMODES][7][10] =
{
		// Kurzwelle-1
		{
		"160m",
		"80m",
		"60/40m",
		"30/20m",
		"17/15m",
		"12/10m",
		"6m"
		},
		// Kurzwelle-2
		{
		"160m",
		"80/60m",
		"40/30m",
		"20/17m",
		"15m",
		"12/10m",
		"6m"
		},
		// Kurzwelle-3
		{
		"160m",
		"80m",
		"60/40m",
		"30m",
		"20-15m",
		"12-10m",
		"6m"
		},
		// Kurzwelle-4
		{
		"160m",
		"80-40m",
		"30m",
		"20m",
		"17/15m",
		"10/12m",
		"6m"
		},
		// Kurzwelle-5
		{
		"----",
		"160/80m",
		"60/40m",
		"30/20m",
		"17/15m",
		"12/10m",
		"6m",
		},
		// VHF, UHF, SHF
		{
		"144 MHz",
		"145 MHz",
		"145 SAT",
		"220 MHz",
		"430 MHz",
		"435 MHz",
		"70cm SAT",
		},
		// low bands
		{
		"2200m",
		"630m-1",
		"630m-2",
		"160m",
		"80m",
		"60m",
		"40m",
		},
		// monoband 630m
		{
		"630m-1",
		"630m-2",
		"630m-3",
		"630m-4",
		"630m-5",
		"630m-6",
		"630m-7",
		},
		// monoband 6m
		{
		"6m-1",
		"6m-2",
		"6m-3",
		"6m-4",
		"6m-5",
		"6m-6",
		"6m-7",
		},
		// monoband 4m
		{
		"4m-1",
		"4m-2",
		"4m-3",
		"4m-4",
		"4m-5",
		"4m-6",
		"4m-7",
		},
		// monoband 2m
		{
		"2m-1",
		"2m-2",
		"2m-3",
		"2m-4",
		"2m-5",
		"2m-6",
		"2m-7",
		},
		// monoband 70cm
		{
		"70cm-1",
		"70cm-2",
		"70cm-3",
		"70cm-4",
		"70cm-5",
		"70cm-6",
		"70cm-7",
		},
		// monoband 23cm
		{
		"23cm-1",
		"23cm-2",
		"23cm-3",
		"23cm-4",
		"23cm-5",
		"23cm-6",
		"23cm-7",
		},
};

// Beschriftung der Band-Buttons
char bandbuttons_text[NUMBANDMODES][BANDBUTTONS_ANZ][10] =
{
		{
		"160m",
		" 80m",
		" 60m",
		" 40m",
		" 30m",
		" 20m",
		" 17m",
		" 15m",
		" 12m",
		" 10m",
		"  6m",
		"AUTO"
		},
		{
		"160m",
		" 80m",
		" 60m",
		" 40m",
		" 30m",
		" 20m",
		" 17m",
		" 15m",
		" 12m",
		" 10m",
		"  6m",
		"AUTO"
		},
		{
		"160m",
		" 80m",
		" 60m",
		" 40m",
		" 30m",
		" 20m",
		" 17m",
		" 15m",
		" 12m",
		" 10m",
		"  6m",
		"AUTO"
		},
		{
		"160m",
		" 80m",
		" 60m",
		" 40m",
		" 30m",
		" 20m",
		" 17m",
		" 15m",
		" 12m",
		" 10m",
		"  6m",
		"AUTO"
		},
		{
		"160m",
		" 80m",
		" 60m",
		" 40m",
		" 30m",
		" 20m",
		" 17m",
		" 15m",
		" 12m",
		" 10m",
		"  6m",
		"AUTO"
		},
		{
		" 144 MHz",
		" 145 MHz",
		" 145 SAT",
		" 220 MHz",
		" 430 MHz",
		" 435 MHz",
		"70cm SAT",
		" ",
		" ",
		" ",
		" ",
		"    AUTO"
		},
		{
		" 2200m",
		"630m-1",
		"630m-2",
		"  160m",
		"   80m",
		"   60m",
		"   40m",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"630m-1",
		"630m-2",
		"630m-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"6m-1",
		"6m-2",
		"6m-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"4m-1",
		"4m-2",
		"4m-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"2m-1",
		"2m-2",
		"2m-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"70cm-1",
		"70cm-2",
		"70cm-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
		{
		"23cm-1",
		"23cm-2",
		"23cm-3",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		" ",
		"  AUTO"
		},
};

// wird aufgerufen sobald ein Band Touch gedrückt wurde,
// aber nur wenn kein ext. Drehschalter vorhanden ist
void select_band(int button)
{
	if(button == TB_BAND12)
	{
		store_autoband(1);
		oldband = -1;
	}
	else
	{
		store_autoband(0);
		_selected_band = button - TB_BAND1;
	}
}

void activate_bandtouchbutton()
{
	for(int i=TB_BAND1; i<=TB_BAND11; i++)
		tb_buttons[i].pressed = 0;

	tb_buttons[TB_BAND1 + _selected_band].pressed = 1;

	for(int i=TB_BAND1; i<=TB_BAND11; i++)
		if(menunum >= MENU_OVERVIEW && menunum < MENU_SWITCH) draw_touchbutton(&tb_buttons[i]);

	tb_buttons[TB_BAND12].pressed = sf_autoband;
	if(menunum >= MENU_OVERVIEW && menunum < MENU_SWITCH) draw_touchbutton(&tb_buttons[TB_BAND12]);
}

void autoband_off()
{
	store_autoband(0);
	activate_bandtouchbutton();
}

// ermittle Index der BandButtons entsprechend der Frequenz
int getBandindex(float freq)
{
	if(pcfgdata->bandmode <= 4)
	{
		// Kurzwelle
		if(freq < 2.1) return 0;// 160m
		if(freq < 4) return 1;	// 80m
		if(freq < 6) return 2;	// 60m
		if(freq < 8) return 3;	// 40m
		if(freq < 11) return 4;	// 30m
		if(freq < 15) return 5;	// 20m
		if(freq < 19) return 6;	// 17m
		if(freq < 22) return 7;	// 15m
		if(freq < 26) return 8;	// 12m
		if(freq < 31) return 9;	// 10m
		return 10;				// 6m
	}
	if(pcfgdata->bandmode == 5)
	{
		if(freq < 145) return 0;
		if(freq < 145.8) return 1;
		if(freq < 219) return 2;
		if(freq < 420) return 3;
		if(freq < 435) return 4;
		if(freq < 438) return 5;
		return 6;
	}
	if(pcfgdata->bandmode == 6)
	{
		if(freq < 0.4) return 0; // 2200m
		if(freq < 0.475) return 1; // 630-1
		if(freq < 0.5) return 2; // 630-2
		if(freq < 2) return 3;   // 160
		if(freq < 4) return 4;	 // 80
		if(freq < 6) return 5;	 // 60
		return 6;				 // 40
	}
	return 10;
}


// wird laufend aufgerufen, schaltet auf das gewünschte Band
void bandselection()
{
static int oldrbsel = -1;
static char f = 1;
static uint32_t oldcivfreq = 0;
static int rot_autoband = -1;

	// Falls ein Drehschalter installiert ist, frage diesen ab
	int rbandsel = rotaryswitch_installed();

	if(rbandsel)
	{
		if(rot_autoband != rbandsel)
		{
			if(rbandsel != ROTS_AUTO && sf_autoband == 1)
			{
				store_autoband(0);
				oldcivfreq = 0;
				tb_buttons[TB_BAND12].pressed = 0;
				if(menunum >= MENU_OVERVIEW && menunum < MENU_SWITCH)
					draw_touchbutton(&tb_buttons[TB_BAND12]);
			}

			if(rbandsel == ROTS_AUTO && sf_autoband == 0)
			{
				store_autoband(1);
				oldcivfreq = 0;
				oldband = -1;
				tb_buttons[TB_BAND12].pressed = 1;
				if(menunum >= MENU_OVERVIEW && menunum < MENU_SWITCH)
					draw_touchbutton(&tb_buttons[TB_BAND12]);
			}

			rot_autoband = rbandsel;
		}

		if(rbandsel == ROTS_AUTO) rbandsel = 0;

		if(pcfgdata->bandmode <= 4)
		{
			switch(rbandsel)
			{
			case ROTS_B1:  	_selected_band = 0; break;
			case ROTS_B2:   _selected_band = 1; break;
			case ROTS_B3: 	_selected_band = 3; break;
			case ROTS_B4: 	_selected_band = 5; break;
			case ROTS_B5: 	_selected_band = 7; break;
			case ROTS_B6: 	_selected_band = 9; break;
			case ROTS_B7: 	_selected_band = 10; break;
			}
		}
		else
		{
			switch(rbandsel)
			{
			case ROTS_B1:  	_selected_band = 0; break;
			case ROTS_B2:   _selected_band = 1; break;
			case ROTS_B3: 	_selected_band = 2; break;
			case ROTS_B4: 	_selected_band = 3; break;
			case ROTS_B5: 	_selected_band = 4; break;
			case ROTS_B6: 	_selected_band = 5; break;
			case ROTS_B7: 	_selected_band = 6; break;
			}
		}

		// wenn sich der Rot.Schalter gedreht hat, so passe auch die Darstellung der Band Touchbuttons an
		if(oldrbsel != rbandsel)
		{
			oldrbsel = rbandsel;
			activate_bandtouchbutton();
		}
	}
	else
	{
		oldrbsel = -1;
		// beim ersten Mal nach dem Start aktiviere den Touchbutton
		if(f)
		{
			f = 0;
			activate_bandtouchbutton();
		}
	}

	// beim ersten Mal nach dem Start aktiviere den Touchbutton
	if(f)
	{
		f = 0;
		activate_bandtouchbutton();
	}

	uint32_t civfreq = getCIVfreq(); // immer aufrufen, auch ohne Automode, da es von der Statuszeile gebraucht wird

	if(sf_autoband && !rbandsel)
	{
		float frequency = 0;	// Icom Frequenz in MHz

		// wenn von CIV Banddaten kommen so haben diese Priorität vor der Bandspannung
		if(civfreq == 0 && oldcivfreq != 0)
		{
			// CIV ist soeben ausgegangen, schalte Automode aus, lasse die Bandwahl so wie sie ist
			// User muss den Automode selbst wieder einschalten
			// ********************************
			// ACHTUNG: Der TRX darf NICHT im CIV-Transceive Modus stehen, sonst gibt
			// es Fehlübertragungen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// ********************************

			// Änderung vom 4.9.18: lasse Automodus doch an, weil es lästig ist wenn er sich wegen
			// Übertragungsfehlern ausschaltet
			/*
			store_autoband(0);
			oldcivfreq = 0;
			tb_buttons[TB_BAND12].pressed = 0;
			if(menunum >= MENU_OVERVIEW && menunum < MENU_SWITCH) draw_touchbutton(&tb_buttons[TB_BAND12]);
			*/
			return;	// keine Frequenz, lasse alles so wie es ist
		}
		else if(civfreq != 0)
		{
			// CI/V Frequenz
			frequency = civfreq / 1000000;	// eingestellte MHz
		}
		else
		{
			if(pcfgdata->bandmode <= 4)
			{
				// Band Eingang nehmen und Frequenz simulieren, favorisiere Standardbänder
				int autoBand = (int)adc_realvals[UBAND];

				if(autoBand == 160) 	frequency = 1.9;
				if(autoBand == 80) 		frequency = 3.6;
				if(autoBand == 6040) 	frequency = 7.1;
				if(autoBand == 30) 		frequency = 10.05;
				if(autoBand == 20) 		frequency = 14.1;
				if(autoBand == 1715) 	frequency = 21.1;
				if(autoBand == 1210) 	frequency = 28.1;
				if(autoBand == 6) 		frequency = 50.5;
			}
		}

		if(sf_autoband)
		{
			oldcivfreq = civfreq;

			// frequency hat die eingestellte Frequenz
			// ermittle jetzt den zu aktivierenden Button
			_selected_band = getBandindex(frequency);

			// und aktiviere diesen
			if(_selected_band != oldband)
			{
				oldband = _selected_band;
				activate_bandtouchbutton();
			}
		}
	}

	// _selected_band hat hier den Index der 11 Bänder
	// wähle entsprechend der Bandbetriebsart das Bandrelais
	if(pcfgdata->bandmode == 0)
	{
		// Kurzwelle-1
		/*
		"160m",
		"80m",
		"60/40m",
		"30/20m",
		"17/15m",
		"12/10m",
		"6m"
		*/
		if(_selected_band == 0 /*160m*/) 	 selected_bandrelais = BANDRELAIS_OFF;
		else if(_selected_band == 1 /*80m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2 /*60m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 3 /*40m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 4 /*30m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 5 /*20m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 6 /*17m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 7 /*15m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 8 /*12m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 9 /*10m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 10 /*6m*/) selected_bandrelais = BANDRELAIS_6;
	}
	else if(pcfgdata->bandmode == 1)
	{
		// Kurzwelle-2
		/*
		"160m",
		"80/60m",
		"40/30m",
		"20/17m",
		"15m",
		"12/10m",
		"6m"
		*/
		if(_selected_band == 0 /*160m*/) 	 selected_bandrelais = BANDRELAIS_OFF;
		else if(_selected_band == 1 /*80m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2 /*60m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 3 /*40m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 4 /*30m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 5 /*20m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 6 /*17m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 7 /*15m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 8 /*12m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 9 /*10m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 10 /*6m*/) selected_bandrelais = BANDRELAIS_6;
	}
	else if(pcfgdata->bandmode == 2)
	{
		// Kurzwelle-3
		/*
		"160m",
		"80m",
		"60/40m",
		"30m",
		"20-15m",
		"12-10m",
		"6m"
		*/
		if(_selected_band == 0 /*160m*/) 	 selected_bandrelais = BANDRELAIS_OFF;
		else if(_selected_band == 1 /*80m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2 /*60m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 3 /*40m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 4 /*30m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 5 /*20m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 6 /*17m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 7 /*15m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 8 /*12m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 9 /*10m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 10 /*6m*/) selected_bandrelais = BANDRELAIS_6;
	}
	else if(pcfgdata->bandmode == 3)
	{
		// Kurzwelle-4
		/*
		"160m",
		"80-40m",
		"30m",
		"20m",
		"17/15m",
		"10/12m",
		"6m"
		*/
		if(_selected_band == 0 /*160m*/) 	 selected_bandrelais = BANDRELAIS_OFF;
		else if(_selected_band == 1 /*80m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2 /*60m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 3 /*40m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 4 /*30m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 5 /*20m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 6 /*17m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 7 /*15m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 8 /*12m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 9 /*10m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 10 /*6m*/) selected_bandrelais = BANDRELAIS_6;
	}
	else if(pcfgdata->bandmode == 4)
	{
		// Kurzwelle-5
		/*
		"----",
		"160/80m",
		"60/40m",
		"30/20m",
		"17/15m",
		"12/10m",
		"6m"
		*/
		if(_selected_band == 0 /*160m*/) 	 selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 1 /*80m*/) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2 /*60m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 3 /*40m*/) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 4 /*30m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 5 /*20m*/) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 6 /*17m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 7 /*15m*/) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 8 /*12m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 9 /*10m*/) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 10 /*6m*/) selected_bandrelais = BANDRELAIS_6;
	}
	else if(pcfgdata->bandmode >= 5)
	{
		// LF und UKW
		if(_selected_band == 0) 	 selected_bandrelais = BANDRELAIS_OFF;
		else if(_selected_band == 1) selected_bandrelais = BANDRELAIS_1;
		else if(_selected_band == 2) selected_bandrelais = BANDRELAIS_2;
		else if(_selected_band == 3) selected_bandrelais = BANDRELAIS_3;
		else if(_selected_band == 4) selected_bandrelais = BANDRELAIS_4;
		else if(_selected_band == 5) selected_bandrelais = BANDRELAIS_5;
		else if(_selected_band == 6) selected_bandrelais = BANDRELAIS_6;
	}

	// und schalte die Relais
	switch_bandrel();
}

void switch_bandrel()
{
	gpio_stat[BANDREL_1] = 0;
	gpio_stat[BANDREL_2] = 0;
	gpio_stat[BANDREL_3] = 0;
	gpio_stat[BANDREL_4] = 0;
	gpio_stat[BANDREL_5] = 0;
	gpio_stat[BANDREL_6] = 0;

	switch(selected_bandrelais)
	{
	case BANDRELAIS_OFF: // lasse alle Relais ausgeschaltet, das ist dann das 160m Band
		break;
	case BANDRELAIS_1:
		gpio_stat[BANDREL_1] = 1;
		break;
	case BANDRELAIS_2:
		gpio_stat[BANDREL_2] = 1;
		break;
	case BANDRELAIS_3:
		gpio_stat[BANDREL_3] = 1;
		break;
	case BANDRELAIS_4:
		gpio_stat[BANDREL_4] = 1;
		break;
	case BANDRELAIS_5:
		gpio_stat[BANDREL_5] = 1;
		break;
	case BANDRELAIS_6:
		gpio_stat[BANDREL_6] = 1;
		break;
	}
}
