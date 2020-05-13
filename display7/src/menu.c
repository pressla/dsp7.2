/*
 * menu.c
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#include <main.h>

void show_selectionmenu();
void draw_menu_frame();
void draw_statbutton(int gpio, int x, int y, int activestate, int symsize );
void draw_menu_bar();
void draw_values_bigbar();
void draw_menu_meter();
void draw_values_meter();

int menunum = MENU_STARTUP;
int oldmenunum = 9999;
int first_statdisplay = 1;
int first_touchcal = 1;
int act_coupler = 1;
uint8_t oldsystemMode;

#define SWITCH_YSTART		50
#define SWITCH_LINESPACE	70
#define SWITCH_XROW1		20
#define SWITCH_XROW2		SWITCH_XROW1+250
#define SWITCH_XROW3		SWITCH_XROW2+250

// ============== Hilfsfunktionen zum Umschalten der Menüs bzw des Status =====================
void switchto_emergencymode()
{
	if(state != STATE_EMERGENCYOFF)
	{
		// Notaus wurde gedrückt
		if(menunum == MENU_SELECTION || menunum == MENU_TOUCHCAL || menunum == MENU_STARTUP)
		{
			emergency_silent(EMERG_USER);
			return;
		}

		tb_buttons[TB_EMERG_OFF].pressed = 0;
		draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);
		emergency(EMERG_USER,0,0);
	}
}

void switchto_ON()
{
	if(state == STATE_EMERGENCYOFF && menunum != MENU_STARTUP)
	{
		// es wird versucht wieder einzuschalten
		error = EMERG_NOERROR;
		state = STATE_STANDBY;

		if(menunum != MENU_SELECTION && menunum != MENU_TOUCHCAL)
		{
			tb_buttons[TB_EMERG_OFF].pressed = 1;
			draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);
			tb_buttons[TB_STANDBY].pressed = 1;
			draw_touchbutton(&tb_buttons[TB_STANDBY]);
			tb_buttons[TB_ACTIVE].pressed = 0;
			draw_touchbutton(&tb_buttons[TB_ACTIVE]);
			draw_menu_frame();
		}
	}
}

int switchto_active()
{
	// nicht aktivieren wenn keine Ub da ist
	if(adc_realvals[UMESS] < 11.0)
	{
		blink_touchbutton(&tb_buttons[TB_ACTIVE], pcfgdata->language?"Ub < 11V":"no Ub");
		return 0;
	}

	// nicht aktivieren wenn PTT gedrückt
	if(gpio_stat[PTTIN] == 0)
	{
		blink_touchbutton(&tb_buttons[TB_ACTIVE], "PTT ?");
		return 0;
	}

	tb_buttons[TB_STANDBY].pressed = 0;
	draw_touchbutton(&tb_buttons[TB_STANDBY]);
	tb_buttons[TB_ACTIVE].pressed = 1;
	draw_touchbutton(&tb_buttons[TB_ACTIVE]);
	state = STATE_ACTIVE;

	showstatus = 1;

	return 1;
}

void switchto_standby()
{
	tb_buttons[TB_STANDBY].pressed = 1;
	draw_touchbutton(&tb_buttons[TB_STANDBY]);
	tb_buttons[TB_ACTIVE].pressed = 0;
	draw_touchbutton(&tb_buttons[TB_ACTIVE]);
	state = STATE_STANDBY;

	showstatus = 1;
}

 // ============== Umschalten der Menüs =====================

 // wird aufgerufen sobald der Benutzer wo touched. Position in t_xpos und t_ypos

int process_button(int touchbutton)
{

	if(menunum != oldmenunum)
	{
		first_statdisplay = 1;
		oldmenunum = menunum;
	}

	// Einschalten nach Systemstart
	if(menunum == MENU_STARTUP)
	{
		tb_buttons[TB_STANDBY].pressed = 1;

		if(touchbutton == TB_EMERG_OFF)
		{
			// nach dem Einschalten wurde auf ON gedrückt
			menunum = sf_startmenu;
			state = STATE_EMERGENCYOFF; // nur damit switchto_ON() durchläuft, wird dort wieder überschrieben
			switchto_ON();
		}
		return 1;
	}

	if(touchbutton == TB_EMERG_OFF)
	{
		if(state != STATE_EMERGENCYOFF)
		{
			// Notaus wurde gedrückt
			switchto_emergencymode();
			return 1;
		}
		else
		{
			// es wird versucht wieder einzuschalten
			switchto_ON();
		}
	}

	// der Menübutton ist immer vorhanden, außer wenn man im Selektionsmenü ist
	if(touchbutton == TB_MENU && menunum != MENU_SELECTION)
	{
		if(menunum == MENU_SYSTEM1)
		{
			setDisplayReverse();
		}

		// das Selectionsmenü wurde neu aufgerufen
		// der Tastendruck wird nicht weiter angezeigt weil man ihn eh nicht sieht
		if((menunum == MENU_SETUP || menunum == MENU_CALIB || menunum == MENU_SYSTEM1 || menunum == MENU_SYSTEM2 || menunum == MENU_ANTSW) && setup_modified)
		{
			// im Setupmenü wurde auf MENU gedrückt, das Setup wird also verlassen
			// speichere Werte
			if(menunum == MENU_CALIB)
			{
				 if(calibvalsOK() == 0)
				 {
					 tft_print(&lucidaConsole_18ptFontInfo, getLang(WRONGVALUE), 0,0, 6, RED , WHITE);
					 return 1;
				 }
			}
			messagebox(getLang(save_values),2000);
			copy_ram_to_flash();
			init_bars();

			// wenn sich der Systemmodus geändert hat:
			if(oldsystemMode != pcfgdata->systemMode && menunum == MENU_SYSTEM2)
			{
				char s[30];
				sprintf(s,"SysMode: %s",pcfgdata->systemMode?"SWR":"PA");
				messagebox(s,2000);
				messagebox("Rebooting",1000);
				NVIC_SystemReset();
			}
		}

		menunum = MENU_SELECTION;
		show_selectionmenu();
		return 1;
	}

	// BOTTOM Touch-Zeile
	if(pcfgdata->systemMode == 0)
	{
		// PA-Controller Modus
		// außer im Selectionsmenü und im Touchcal ist die Bottom-Touchzeile immer gleich
		if((touchbutton == TB_STANDBY || touchbutton == TB_ACTIVE) && menunum != MENU_SELECTION && menunum != MENU_TOUCHCAL)
		{
			if(state == STATE_EMERGENCYOFF) return 1;	// nicht aktivieren im Notausmodus

			if(touchbutton == TB_ACTIVE)
			{
				if(gpio_stat[PTTIN] == 0) return 1; // nicht aktivieren wenn PTT gedrückt

				if(!switchto_active())
					return 1;			// es wurde nicht auf Aktiv geschaltet
			}
			if(touchbutton == TB_STANDBY)
			{
				switchto_standby();
			}
			return 1;
		}
	}

	if(pcfgdata->systemMode == 1)
	{
		// PwrSwr Meter Modus
		// außer im Selectionsmenü und im Touchcal ist die Bottom-Touchzeile immer gleich
		if((touchbutton == TB_KOP1 || touchbutton == TB_KOP2 || touchbutton == TB_KOP3) && menunum != MENU_SELECTION && menunum != MENU_TOUCHCAL)
		{
			for(int i=TB_KOP1; i<=TB_KOP3; i++)
				tb_buttons[i].pressed = 0;

			tb_buttons[touchbutton].pressed = 1;

			if(touchbutton == TB_KOP1) act_coupler = 1;
			if(touchbutton == TB_KOP2) act_coupler = 2;
			if(touchbutton == TB_KOP3) act_coupler = 3;
			draw_values();

			for(int i=TB_KOP1; i<=TB_KOP3; i++)
				draw_touchbutton(&tb_buttons[i]);
			return 1;
		}
	}

	if(menunum == MENU_HISTORY)
	{
		if(do_button_history(touchbutton))
			return 1;
	}

	if(menunum == MENU_ANTSW)
	{
		if(do_button_antsw(touchbutton))
			return 1;
	}

	// die Bandauswahl gibt es überall außer im Selectionsmenü, Switchmenü, Setupmenü, Calicmenü und Touchcal
	if(menunum != MENU_SELECTION && menunum != MENU_TOUCHCAL && menunum != MENU_SWITCH && menunum != MENU_SETUP && menunum != MENU_CALIB && menunum != MENU_SYSTEM1 && menunum != MENU_SYSTEM2)
	{
		if(touchbutton >= TB_BAND1 && touchbutton <= TB_BAND12)
		{
			// einer der Band-Touchbuttons wurde gedrückt
			// falls ein ext. Schalter angeschlossen ist ignoriere das
			if(!rotaryswitch_installed())
			{
				// Bandtasten
				if(!sf_autoband)
				{
					for(int i=TB_BAND1; i<=TB_BAND11; i++)
					{
						if(i != touchbutton)
						{
							tb_buttons[i].pressed = 0;
							draw_touchbutton(&tb_buttons[i]);
						}
						else
						{
							tb_buttons[i].pressed = 1;
							draw_touchbutton(&tb_buttons[i]);
							select_band(touchbutton);
						}
					}
				}

				// die AUTO Band Taste schaltet immer hin und her
				if(touchbutton == TB_BAND12)
				{
					if(tb_buttons[TB_BAND12].pressed)
					{
						tb_buttons[TB_BAND12].pressed = 0;
						autoband_off();
					}
					else
					{
						tb_buttons[TB_BAND12].pressed = 1;
						select_band(touchbutton);
					}
					draw_touchbutton(&tb_buttons[TB_BAND12]);
				}
				return 1;
			}
		}
	}

	if(menunum == MENU_SETUP)
	{
		return do_button_setup(touchbutton);
	}

	if(menunum == MENU_CALIB)
	{
		// behandle Standard und Ziffer Buttons
		return do_button_calib(touchbutton);
	}

	if(menunum == MENU_SYSTEM1)
	{
		return do_button_system(touchbutton);
	}

	if(menunum == MENU_SYSTEM2)
	{
		return do_button_system2(touchbutton);
	}

	return 0;
}

// Auswahl eines Menüs
// 3 Reihen zu je 5 Menüs
// Hor: Abstand: 45, Icon: 106
// Vert:Abstand: 45, Icon: 100
// Bildgröße: 106 x 100 Pixel

#define bmp_width	106
#define bmp_height	100
#define bmp_xspace	45
#define bmp_yspace	45

// diese Bezeichnung entspricht den Bild-Dateinamen
char eicons[16][20] =
{
	"overview",
	"powerbar",
	"swrmeter",
	"bridge",
	"history",
	"adcvals",
	"results",
	"status",
	"switch",
	"antsw",
	"limits",
	"calib",
	"touchcal",
	"system1",
	"system2"
	"",
};

char dicons[16][20] =
{
	"Uebersicht",
	"Leistung",
	"Meter",
	"Messbr.",
	"Historie",
	"Rohwerte",
	"Ergebnisse",
	"Status",
	"Schalten",
	"Antenne",
	"limits",
	"Kalibrat",
	"Touch Kal",
	"System1",
	"System2",
	"",
};

char (*icons)[20];

void show_selectionmenu()
{
int x,y,num=0;
char fn[25];

	TFT_Fill_Screen(BLACK);

	icons = eicons;
	if(pcfgdata->language == 1) icons = dicons;

	for(y=bmp_yspace; y<TFT_HEIGHT; y+=(bmp_height+bmp_yspace))
	{
		for(x=bmp_xspace; x<TFT_WIDTH; x+=(bmp_xspace+bmp_width))
		{
			if(icons[num][0] != 0)
			{
				strcpy(fn,eicons[num]);
				int ret = TFT_DrawFlash(fn,x,y);
				strcpy(fn,icons[num]);
				if(ret)
				{
					for(int i=0; i<strlen(fn); i++)
						fn[i] = toupper(fn[i]);
					if(strstr(fn,"EFFIC")) strcpy(fn,"EFFICIENCY");
					if(strstr(fn,"ANTSW")) strcpy(fn,"ANTENNA");

					// damit die Beschriftung mittig ist, messe die Breite des Textes
					int w,h;
					tft_textwidth_pixel(&lucidaConsole_18ptFontInfo, fn, &w, &h);
					int xtext = x + bmp_width/2 - w/2 - 10;

					tft_print(&lucidaConsole_18ptFontInfo, fn, xtext, y+bmp_height, 2, WHITE , BLACK);
				}
			}
			num++;
		}
	}

	int yver = TFT_HEIGHT - 14;
	int bcol = BLACK;
	int fcol = DARKGREY;
	tft_print(&verdana_10ptFontInfo, "Amateur Radio Controller", 10, yver, 3, fcol , bcol);
	tft_print(&verdana_10ptFontInfo, ipadr, 300, yver, 3, fcol , bcol);
	tft_print(&verdana_10ptFontInfo, FWVERSION, 580, yver, 2, fcol , bcol);
	tft_print(&verdana_10ptFontInfo, "by helitron.de", 680, yver, 2, fcol , bcol);
}


// falls das Slectionsmenü offen ist, bearbeite hier die Buttons
void selection_menu(int x, int y)
{
int spalte=0, zeile=0;
int tmp;

	// ermittle Spalte
	tmp = bmp_width + (bmp_xspace*3)/2;
	if(x < tmp) spalte = 0;
	else
	{
		tmp += (bmp_width + bmp_xspace);
		if( x < tmp) spalte = 1;
		else
		{
			tmp += (bmp_width + bmp_xspace);
			if( x < tmp) spalte = 2;
			else
			{
				tmp += (bmp_width + bmp_xspace);
				if( x < tmp) spalte = 3;
				else spalte = 4;
			}
		}
	}

	// ermittle Zeile
	tmp = bmp_height + (bmp_yspace*3)/2;
	if(y < tmp) zeile = 0;
	else
	{
		tmp += (bmp_height + bmp_yspace);
		if( y < tmp) zeile = 1;
		else zeile = 2;
	}

	/*char s[50] = {"                      "};
	sprintf(s,"Z:%d S:%d           ",zeile, spalte);
	tft_print(&lucidaConsole_12ptFontInfo, s, 400, 400, 4, WHITE , BLACK);*/

	// ermittle neue Menünummer
	if(zeile == 0)
	{
		if(spalte == 0) menunum = MENU_OVERVIEW;
		if(spalte == 1) menunum = MENU_BAR;
		if(spalte == 2) menunum = MENU_METER;
		if(spalte == 3) menunum = MENU_BRIDGES;
		if(spalte == 4) menunum = MENU_HISTORY;
	}
	if(zeile == 1)
	{
		if(spalte == 0) menunum = MENU_ADC;
		if(spalte == 1) menunum = MENU_VALUES;
		if(spalte == 2) menunum = MENU_STATUS;
		if(spalte == 3) menunum = MENU_SWITCH;
		if(spalte == 4) menunum = MENU_ANTSW;
	}
	if(zeile == 2)
	{
		if(spalte == 0) menunum = MENU_SETUP;
		if(spalte == 1) menunum = MENU_CALIB;
		if(spalte == 2) menunum = MENU_TOUCHCAL;
		if(spalte == 3) menunum = MENU_SYSTEM1;
		if(spalte == 4) menunum = MENU_SYSTEM2;
	}

	// Zeichne die Fixteile des neuen Fensters
	draw_menu_frame();
	showstatus = 1;
}

void draw_menu_startup()
{
uint16_t bgcol = WHITE;

	TFT_Fill_Screen(bgcol);

	if(pcfgdata->systemMode == 0)
		draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);

	int y = 50;
	tft_print(&verdana_36ptFontInfo, "DSP-7", 330,y, 8, BLUE , bgcol);
	if(pcfgdata->systemMode == 0)
	{
		tft_print(&verdana_36ptFontInfo, "Power Amp Controller", 140,y+70, 6, BLUE , bgcol);
		tft_print(&lucidaConsole_18ptFontInfo, getLang(SWITCHON), 120,y+180, 6, BLUE , bgcol);
	}
	else if(pcfgdata->systemMode == 1)
	{
		y += 20;
		tft_print(&verdana_36ptFontInfo, getLang(MODETITLE1), 320,y+70, 6, BLUE , bgcol);
		tft_print(&verdana_36ptFontInfo, getLang(MODETITLE2), 160,y+130, 6, BLUE , bgcol);
	}
	tft_print(&verdana_10ptFontInfo, "made in Germany by helitron.de", 530,465, 2, BLUE , bgcol);

	TFT_DrawFlash("splash",450,300);

	if(pcfgdata->systemMode == 1)
	{
		// PwrSwr Mode
		delay_1ms(2000);

		tb_buttons[TB_KOP1].pressed = 1;
		menunum = sf_startmenu;
		draw_menu_frame();
	}
}

void draw_menu_overview()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	show_mainbars();
}

void draw_menu_bridges()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	show_bridge_bars();

	// Trennlinien zwischen den Bridges
	TFT_Hor_Line(0, 650, 136, YELLOW, BLACK, 2, 0);
	TFT_Hor_Line(0, 650, 272, YELLOW, BLACK, 2, 0);

	TFT_Vert_Line(230, 0, 410, YELLOW, BLACK, 2, 8);

	// Beschriftung der Zahlenwerte
	int x = 1;
	int xdoppelpunkt = 83;
	int xeinheit = 178;
	int y = 10;
	int yspace = 18;
	int line = 0;
	for(int bg=0; bg<3; bg++)
	{
		line = 0;
		y = 8 + bg * 136;
		tft_print(&verdana_12ptFontInfo, "FWDpk", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "W", xeinheit, y + line++ * yspace, 1, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "REV", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "W", xeinheit, y + line++ * yspace, 1, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "FWDpk", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "dBm", xeinheit, y + line++ * yspace, 2, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "REV", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "dBm", xeinheit, y + line++ * yspace, 2, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "X", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "ohms", xeinheit, y + line++ * yspace, 2, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "SWR", x, y + line * yspace, 3, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, " ", xeinheit, y + line++ * yspace, 4, WHITE , BLACK); // :1 wird mit Value gedruckt
		tft_print(&verdana_12ptFontInfo, "Refl.Att.", x, y + line * yspace, 2, WHITE , BLACK);
		tft_print(&verdana_12ptFontInfo, "dB", xeinheit, y + line++ * yspace, 2, WHITE , BLACK);

		for(int i=0; i<line; i++)
			tft_print(&verdana_12ptFontInfo, ":", xdoppelpunkt, y + i * yspace, 1, WHITE , BLACK);
	}
}

void draw_menu_hist()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	draw_touchbuttons_history();

	hist_draw_frame();
}

char eadc_title[20][30] =
{
	"ICOM Band Selection Voltage",
	"Power Supply Voltage       ",
	"Antenna Reverse Power      ",
	"Temperature Sensor-1       ",
	"Power Supply Current       ",
	"Drive Forward Power        ",
	"PA-Filter Forward Power    ",
	"Drive Reverse Power        ",
	"Temperature Sensor-2       ",
	"Antenna Forward Power      ",
	"PA-Filter Reverse Power    ",
	"Antenna Forward Peak       ",
	"Filter Forward Peak        ",
	"Drive Forward Peak         ",
	"Efficiency                 ",
	"DC Input Power             ",
	"Frequency                  ",
};

char dadc_title[20][30] =
{
	"ICOM Band Umschaltspannung ",
	"Versorgungsspannung        ",
	"Antenne rueckl. Leistung   ",
	"Temperatur Sensor-1        ",
	"Versorgungsstrom           ",
	"Eingangsleistung           ",
	"PA-Filter Ausgangsleistung ",
	"Eingang Rueckl. Leistung   ",
	"Temperatur Sensor-2        ",
	"Antenne Ausgangsleistung   ",
	"PA-Filter rueckl. Leistung ",
	"Antenne Spitzenleistung    ",
	"Filter Spitzenleistung     ",
	"Eingangs-Spitzenleistung   ",
	"Wirkungsgrad               ",
	"DC Eingangsleistung        ",
	"Frequenz                   ",
};

char (*adc_title)[30];

#define DARKVAL	80
#define LIGHTRED RGB16(255,DARKVAL,DARKVAL)
#define LIGHTGREEN RGB16(DARKVAL,255,DARKVAL)
#define LIGHTBL1 RGB16(DARKVAL,DARKVAL,255)
#define LIGHTBL2 RGB16(40,40,255)
#define LIGHTBL3 RGB16(DARKVAL,DARKVAL,255)

uint8_t adc_order[20]  = {UMESS,IMESS,UTEMP,UTEMP2,UBAND,UFWDANT,UANTFWDPEAK,UREVANT,UFWDFLT,UREVFLT,UFWDIN,UREVIN,CALC_EFF,CALC_POWER,MFREQUENCY};
uint16_t col_order[20] = {LIGHTRED,LIGHTRED,LIGHTGREEN,LIGHTGREEN,WHITE,LIGHTBL1,LIGHTBL1,LIGHTBL1,LIGHTBL2,LIGHTBL2,LIGHTBL3,LIGHTBL3,WHITE,LIGHTRED,WHITE};

 // Ausgangsleistung und SWR
void draw_menu_adcvals()
{
	adc_title = eadc_title;
	if(pcfgdata->language == 1) adc_title = dadc_title;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	TFT_Hor_Line(0, 600, 56, YELLOW, BLACK, 2, 0);
	tft_print(&verdana_36ptFontInfo, getLang(RAW_ADC_Values), 14, 1, 5, WHITE , BLACK);

	int y=70;
	int linespace = 28;

	char s[50];
	for(int i=0; i<12; i++)
	{
		sprintf(s,"%s :",adc_title[adc_order[i]]);
		tft_print(&lucidaConsole_18ptFontInfo, s, 10, y + linespace*i, 1, col_order[i] , BLACK);
	}
}

void draw_menu_realvals()
{
	adc_title = eadc_title;
	if(pcfgdata->language == 1) adc_title = dadc_title;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	tft_print(&verdana_36ptFontInfo, getLang(Real_Values), 14, 1, 5, WHITE , BLACK);
	TFT_Hor_Line(0, 600, 50, YELLOW, BLACK, 2, 0);

	int y=55;
	int linespace = 24;

	char s[50];
	for(int i=0; i<15; i++)
	{
		sprintf(s,"%s :",adc_title[adc_order[i]]);
		tft_print(&lucidaConsole_18ptFontInfo, s, 10, y + linespace*i, 1, col_order[i] , BLACK);
	}
}

void draw_menu_status()
{
	first_statdisplay = 1;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	TFT_Hor_Line(0, 600, 33, YELLOW, BLACK, 2, 0);
	tft_print(&lucidaConsole_22ptFontInfo, getLang(Line_Status), 14, 1, 4, WHITE , BLACK);

	int x = 20 + 40;
	int y = 45 + 4;
	int linespace = 28;
	char s[30];

	// Eingänge
	tft_print(&lucidaConsole_18ptFontInfo, getLang(ON_switch), x, y, 2, WHITE , BLACK); 	y += linespace;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Emerg_OFF_switch), x, y, 2, WHITE , BLACK); 	y += linespace;
	if(pcfgdata->systemMode == 0)
	{
		tft_print(&lucidaConsole_18ptFontInfo, getLang(STANDBY_switch), x, y, 2, WHITE , BLACK);	y += linespace;
		tft_print(&lucidaConsole_18ptFontInfo, getLang(PTT_from_TRX), x, y, 2, WHITE , BLACK); 	y += linespace;
		tft_print(&lucidaConsole_18ptFontInfo, "ICOM TKEY", x, y, 2, WHITE , BLACK); 	y += linespace;
		for(int i=0; i<7; i++)
		{
			sprintf(s,"%s %s",band_text_SW[pcfgdata->bandmode][i],getLang(SWITCH));
			tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += linespace;
		}
		sprintf(s,"%s %s","Auto Band",getLang(SWITCH));
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += linespace;
	}

	// Ausgänge
	x = 370 + 40;
	y = 45 + 4;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Antenna_1), x, y, 2, WHITE , BLACK); 	y += linespace;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Antenna_2), x, y, 2, WHITE , BLACK); 	y += linespace;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(PS_Enable), x, y, 2, WHITE , BLACK); 	y += linespace;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Power_Relais), x, y, 2, WHITE , BLACK); 	y += linespace;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(FAN), x, y, 2, WHITE , BLACK); 	y += linespace;
	if(pcfgdata->systemMode == 0)
	{
		tft_print(&lucidaConsole_18ptFontInfo, "BIAS", x, y, 2, WHITE , BLACK); 	y += linespace;
		tft_print(&lucidaConsole_18ptFontInfo, getLang(PTT_out), x, y, 2, WHITE , BLACK); 	y += linespace;
		for(int i=1; i<7; i++)
		{
			sprintf(s,"%s Relais",band_text_SW[pcfgdata->bandmode][i]);
			tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += linespace;
		}
	}
}

void draw_menu_switch()
{
	char s[30];

	first_statdisplay = 1;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_bottom();
	TFT_Hor_Line(0, 600, 33, YELLOW, BLACK, 2, 0);
	tft_print(&lucidaConsole_22ptFontInfo, "Outputs: Manual ON/OFF", 14, 1, 8, WHITE , BLACK);

	int x = SWITCH_XROW1 + 55;
	int y = SWITCH_YSTART + 12;

	tft_print(&lucidaConsole_18ptFontInfo, getLang(Antenna_1), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Antenna_2), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(PS_Enable), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Power_Relais), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
	tft_print(&lucidaConsole_18ptFontInfo, getLang(FAN), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;

	if(pcfgdata->systemMode == 0)
	{
		x = SWITCH_XROW2 + 55;
		y = SWITCH_YSTART + 12;

		tft_print(&lucidaConsole_18ptFontInfo, "BIAS", x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		tft_print(&lucidaConsole_18ptFontInfo, getLang(PTT_out), x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][1]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][2]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][3]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;

		x = SWITCH_XROW3 + 55;
		y = SWITCH_YSTART + 12;

		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][4]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][5]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
		sprintf(s,"%s Rel",band_text_SW[pcfgdata->bandmode][6]);
		tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 2, WHITE , BLACK); 	y += SWITCH_LINESPACE;
	}

	x = 650;
	TFT_DrawFlash("attentio", x, 300);
	tft_print(&lucidaConsole_18ptFontInfo, getLang(Security), x-40, 355, 2, RED , BLACK);
	tft_print(&lucidaConsole_18ptFontInfo, getLang(_OFF), x-32, 380, 2, RED , BLACK);
}

void draw_menu_antsw()
{
	TFT_Fill_Screen(BLACK);

	draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);
	draw_touchbutton(&tb_buttons[TB_MENU]);

	if(pcfgdata->systemMode == 0)
	{
		draw_touchbutton(&tb_buttons[TB_ACTIVE]);
		draw_touchbutton(&tb_buttons[TB_STANDBY]);
	}
	else if(pcfgdata->systemMode == 1)
	{
		draw_touchbutton(&tb_buttons[TB_KOP1]);
		draw_touchbutton(&tb_buttons[TB_KOP2]);
		draw_touchbutton(&tb_buttons[TB_KOP3]);
	}

	draw_touchbuttons_antswitch();
	TFT_Hor_Line(0, 600, 56, YELLOW, BLACK, 2, 0);

	draw_antswmenu();
	tft_print(&verdana_36ptFontInfo, getLang(AntSwitch), 14, 1, 5, WHITE , BLACK);
}

void draw_menu_touchcal()
{
	first_touchcal = 1;
	TFT_Fill_Screen(BLACK);
	tft_print(&lucidaConsole_22ptFontInfo, getLang(Touch_Panel_Calibration), 100, 1, 8, WHITE , BLACK);
}

 // zeichnet die festen Bestandteile eines Menüs
 void draw_menu_frame()
 {
	showstatus = 1;
	if(menunum == MENU_OVERVIEW) draw_menu_overview();
	else if(menunum == MENU_BAR) draw_menu_bar();
	else if(menunum == MENU_METER) draw_menu_meter();
	else if(menunum == MENU_BRIDGES) draw_menu_bridges();
	else if(menunum == MENU_HISTORY) draw_menu_hist();
	else if(menunum == MENU_ADC) draw_menu_adcvals();
	else if(menunum == MENU_VALUES) draw_menu_realvals();
	else if(menunum == MENU_STATUS) draw_menu_status();
	else if(menunum == MENU_SWITCH) draw_menu_switch();
	else if(menunum == MENU_ANTSW) draw_menu_antsw();
	else if(menunum == MENU_SETUP) draw_menu_setup();
	else if(menunum == MENU_CALIB) draw_menu_calib();
	else if(menunum == MENU_TOUCHCAL) draw_menu_touchcal();
	else if(menunum == MENU_SYSTEM1) draw_menu_system();
	else if(menunum == MENU_SYSTEM2)
		{
			oldsystemMode = pcfgdata->systemMode;
			draw_menu_system2();
		}
	else menunum = MENU_SELECTION; // wenn das gewählte Menü nicht existiert
 }


 // ============== Einfügen der aktuellen Werte =====================

void draw_values_overviewbar()
{
float watt=0,peakwatt=0,swr=0;

	if(pcfgdata->systemMode == 0 || act_coupler == 1)
	{
		watt = pwrswr_antenna.fwd_watt;
		peakwatt = pwrswr_antenna.fwd_peakwatt;
		swr = pwrswr_antenna.swr;
	}
	else if(pcfgdata->systemMode == 1 && act_coupler == 2)
	{
		watt = pwrswr_filter.fwd_watt;
		peakwatt = pwrswr_filter.fwd_peakwatt;
		swr = pwrswr_filter.swr;
	}
	else if(pcfgdata->systemMode == 1 && act_coupler == 3)
	{
		watt = pwrswr_input.fwd_watt;
		peakwatt = pwrswr_input.fwd_peakwatt;
		swr = pwrswr_input.swr;
	}

	fill_bar(&bar_out_power, watt, peakwatt);
	fill_bar(&bar_out_swr, swr,-1);
	if(pcfgdata->systemMode == 0)
	{
		fill_bar(&bar_in_power, pwrswr_input.fwd_watt,pwrswr_input.fwd_peakwatt);
		fill_bar(&bar_efficiency, adc_realvals[CALC_EFF],-1);
	}
	fill_bar(&bar_temperature1, adc_realvals[UTEMP],-1);
	fill_bar(&bar_temperature2, adc_realvals[UTEMP2],-1);
	fill_bar(&bar_voltage, adc_realvals[UMESS],-1);
	fill_bar(&bar_current, adc_realvals[IMESS],-1);
	fill_bar(&bar_dc_power, adc_realvals[CALC_POWER],-1);
}

 void draw_value_bridges()
{
	char s1[20] = {0};
	char s2[20] = {0};
	char s3[20] = {0};
	char s4[20] = {0};
	char s5[20] = {0};
	t_pwrswr *p = &pwrswr_antenna;
	int x = 100;
	int restclear = 158;
	int y = 1;
	int yspace = 18;
	int line = 0;

	for(int bg=0; bg<3; bg++)
	{
		line = 0;
		y = 8 + bg * 136;
		if(bg == 0) p = &pwrswr_antenna;
		else if(bg == 1) p = &pwrswr_filter;
		else if(bg == 2) p = &pwrswr_input;

		sprintf(s1,"%6.1f",(float)((int)(p->fwd_peakwatt+0.5)));
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s1, x, y + line++ * yspace, 1, GREEN , BLACK, restclear);
		sprintf(s1,"%6.1f",p->rev_watt);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s1, x, y + line++ * yspace, 1, RED , BLACK, restclear);

		// wenn keine TX Leistung, dann folgende Daten nicht anzeigen
		if(p->fwd_watt >= 0.1)
		{
			sprintf(s5,"%6.1f",p->fwd_dBmpeak);
			sprintf(s1,"%6.1f",p->rev_dBm);
			sprintf(s2,"%3.0f-%3.0f",p->imp_min,p->imp_max);
			sprintf(s3,"%7.2f :1",p->swr);
			sprintf(s4,"%6.1f",p->refl_attenuation+0.05);
		}
		else
		{
			sprintf(s5,"------");
			sprintf(s1,"------");
			sprintf(s2,"-------");
			sprintf(s3,"------");
			sprintf(s4,"------");
		}
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s5, x, y + line++ * yspace, 1, GREEN , BLACK, restclear);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s1, x, y + line++ * yspace, 1, RED , BLACK, restclear);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s2, x-12, y + line++ * yspace, 1, YELLOW , BLACK, restclear);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s3, x, y + line++ * yspace, 1, YELLOW , BLACK, restclear);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s4, x, y + line++ * yspace, 1, YELLOW , BLACK, restclear);
	}

	fill_bar(&bar_bg_antfwd, pwrswr_antenna.fwd_watt, pwrswr_antenna.fwd_peakwatt);
	fill_bar(&bar_bg_antrev, pwrswr_antenna.rev_watt,-1);
	fill_bar(&bar_bg_fltfwd, pwrswr_filter.fwd_watt,pwrswr_filter.fwd_peakwatt);
	fill_bar(&bar_bg_fltrev, pwrswr_filter.rev_watt,-1);
	fill_bar(&bar_bg_drvfwd, pwrswr_input.fwd_watt,pwrswr_input.fwd_peakwatt);
	fill_bar(&bar_bg_drvrev, pwrswr_input.rev_watt,-1);
}

void draw_values_adcvals()
{
int y=70;
int linespace = 28;
char s[50];

	for(int i=0; i<12; i++)
	{
		sprintf(s,"%5.3f V",(float)adc_rawvals[adc_order[i]]/1000.0);
		tft_print(&lucidaConsole_18ptFontInfo, s, 495, y + linespace*i, 1, col_order[i] , BLACK);
	}
}

void draw_values_realvals()
{
int x = 470;
int y=55;
int linespace = 24;
char s[50];
int i=0;

	sprintf(s,"%6.1f V",adc_realvals[UMESS]);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f A",adc_realvals[IMESS]);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

  	if(adc_realvals[UTEMP] > -9)
  		sprintf(s,"%6.1f DegC",adc_realvals[UTEMP]);
  	else
  		sprintf(s,"   --- DegC");
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

  	if(adc_realvals[UTEMP2] > -9)
		sprintf(s,"%6.1f DegC",adc_realvals[UTEMP2]);
	else
		sprintf(s,"   --- DegC");
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"  %4.0f m",adc_realvals[UBAND]);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_antenna.fwd_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_antenna.fwd_peakwatt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_antenna.rev_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_filter.fwd_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_filter.rev_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_input.fwd_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W",pwrswr_input.rev_watt);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f W[dc]",adc_realvals[CALC_POWER]);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

	sprintf(s,"%6.1f %%",adc_realvals[CALC_EFF]);
  	tft_print(&lucidaConsole_18ptFontInfo, s, x, y, 1, col_order[i++] , BLACK);
  	y += linespace;

  	sprintf(s,"%7.1f kHz",adc_realvals[MFREQUENCY]);
	tft_print(&lucidaConsole_18ptFontInfo, s, x-14, y, 1, col_order[i++] , BLACK);
	y += linespace;
}

void draw_statbutton(int gpio, int x, int y, int activestate, int symsize )
{
static int oldstat[MAXGPIOS];
char bmpgn[10], bmprd[10];

	if(symsize == 0)
	{
		strcpy(bmpgn,"greenbut");
		strcpy(bmprd,"redbut");
	}
	else
	{
		strcpy(bmpgn,"gn48x48");
		strcpy(bmprd,"rd48x48");
	}


	if(first_statdisplay)
	{
		for(int i=0; i<MAXGPIOS; i++)
			oldstat[i] = -1;
		first_statdisplay = 0;
	}

	int status = gpio_stat[gpio];
	if(status != oldstat[gpio])
	{
		if(status)
		{
			if(activestate) TFT_DrawFlash(bmpgn, x, y);
			else TFT_DrawFlash(bmprd, x, y);
		}
		else
		{
			if(activestate) TFT_DrawFlash(bmprd, x, y);
			else TFT_DrawFlash(bmpgn, x, y);
		}

		oldstat[gpio] = status;
	}
}

void draw_values_status()
{
	int x = 20;
	int y = 45;
	int linespace = 28;

	draw_statbutton(SW_ONOFF,x,y,0,0);	y += linespace;
	draw_statbutton(SW_RESET,x,y,0,0);	y += linespace;
	if(pcfgdata->systemMode == 0)
	{
		draw_statbutton(SW_STANDBY,x,y,0,0);	y += linespace;
		draw_statbutton(PTTIN,x,y,0,0);	y += linespace;
		draw_statbutton(TKEY,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B1,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B2,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B3,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B4,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B5,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B6,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_B7,x,y,0,0);	y += linespace;
		draw_statbutton(ROTS_AUTO,x,y,0,0);	y += linespace;
	}

	x = 370;
	y = 45;

	draw_statbutton(ANTSEL1,x,y,1,0);	y += linespace;
	draw_statbutton(ANTSEL2,x,y,1,0);	y += linespace;
	draw_statbutton(PWRREL_OFF,x,y,1,0);	y += linespace;
	draw_statbutton(PWRREL_ON,x,y,1,0);	y += linespace;
	draw_statbutton(FAN_ONOFF,x,y,1,0);	y += linespace;
	if(pcfgdata->systemMode == 0)
	{
		draw_statbutton(BIAS_ONOFF,x,y,1,0);	y += linespace;
		draw_statbutton(PTTOUT,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_1,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_2,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_3,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_4,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_5,x,y,1,0);	y += linespace;
		draw_statbutton(BANDREL_6,x,y,1,0);	y += linespace;
	}
}

void switch_outputs(int x, int y)
{
	int line = 0;
	int col = 0;
	static int switchbut_pressed = 0;

	if(x == -1 && y == -1)
	{
		// nichts ist gedrückt
		switchbut_pressed = 0;
		return;
	}

	if(switchbut_pressed == 1) return;	// es ist bereits gedrückt

	// hier neuer Druck
	switchbut_pressed = 1;

	for(line=0; line<7; line++)
	{
		if(y < (line*SWITCH_LINESPACE+SWITCH_YSTART*2))
			break;
	}

	if(x > SWITCH_XROW2) col = 1;
	if(x > SWITCH_XROW3) col = 2;

	if(col == 0)
	{
		switch (line)
		{
		case 0: gpio_stat[ANTSEL1] = (gpio_stat[ANTSEL1]?0:1); break;
		case 1: gpio_stat[ANTSEL2] = (gpio_stat[ANTSEL2]?0:1); break;
		case 2: gpio_stat[PWRREL_OFF] = (gpio_stat[PWRREL_OFF]?0:1); break;
		case 3: gpio_stat[PWRREL_ON] = (gpio_stat[PWRREL_ON]?0:1); break;
		case 4: gpio_stat[FAN_ONOFF] = (gpio_stat[FAN_ONOFF]?0:1); break;
		}
	}
	else if(col == 1)
	{
		switch (line)
		{
		case 0: gpio_stat[BIAS_ONOFF] = (gpio_stat[BIAS_ONOFF]?0:1); break;
		case 1: gpio_stat[PTTOUT] = (gpio_stat[PTTOUT]?0:1); break;
		case 2: gpio_stat[BANDREL_1] = (gpio_stat[BANDREL_1]?0:1); break;
		case 3: gpio_stat[BANDREL_2] = (gpio_stat[BANDREL_2]?0:1); break;
		case 4: gpio_stat[BANDREL_3] = (gpio_stat[BANDREL_3]?0:1); break;
		}
	}
	else
	{
		switch (line)
		{
		case 0: gpio_stat[BANDREL_4] = (gpio_stat[BANDREL_4]?0:1); break;
		case 1: gpio_stat[BANDREL_5] = (gpio_stat[BANDREL_5]?0:1); break;
		case 2: gpio_stat[BANDREL_6] = (gpio_stat[BANDREL_6]?0:1); break;
		}
	}
}

void draw_values_switch()
{
	int x = SWITCH_XROW1;
	int y = SWITCH_YSTART;

	draw_statbutton(ANTSEL1,x,y,1,1);	y += SWITCH_LINESPACE;
	draw_statbutton(ANTSEL2,x,y,1,1);	y += SWITCH_LINESPACE;
	draw_statbutton(PWRREL_OFF,x,y,1,1);	y += SWITCH_LINESPACE;
	draw_statbutton(PWRREL_ON,x,y,1,10);	y += SWITCH_LINESPACE;
	draw_statbutton(FAN_ONOFF,x,y,1,1);	y += SWITCH_LINESPACE;

	if(pcfgdata->systemMode == 0)
	{
		x = SWITCH_XROW2;
		y = SWITCH_YSTART;

		draw_statbutton(BIAS_ONOFF,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(PTTOUT,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(BANDREL_1,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(BANDREL_2,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(BANDREL_3,x,y,1,1);	y += SWITCH_LINESPACE;


		x = SWITCH_XROW3;
		y = SWITCH_YSTART;

		draw_statbutton(BANDREL_4,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(BANDREL_5,x,y,1,1);	y += SWITCH_LINESPACE;
		draw_statbutton(BANDREL_6,x,y,1,1);	y += SWITCH_LINESPACE;
	}
}

int calstat = 0;
int newtop, newbottom, newleft, newright;

void touchcal(int x, int y, int rawx, int rawy)
{
static int oldx=-1, oldy=-1;
int rectsize = 2;
static int doexit = 0;

	if(first_touchcal)
	{
		oldx = -1;
		oldy = -1;
		calstat = 0;
		first_touchcal = 0;
		doexit = 0;
	}

	if(x == -1 && y == -1)
	{
		// es wurde losgelassen
		if(calstat == 1) calstat = 2;
		if(calstat == 3) calstat = 4;
		if(calstat == 6) calstat = 7;
		return;
	}

	if(calstat == 2)
	{
		// linke obere Ecke wurde gedrückt
		newtop = rawy;
		newleft = rawx;
		doexit = 0;
		calstat = 3;
	}
	if(calstat == 5)
	{
		// rechte untere Ecke wurde gedrückt
		newbottom = rawy;
		newright = rawx;
		doexit = 0;
		calstat = 6;
		if(set_touchref(newleft,newright,newtop,newbottom) == 0)
		{
			// ungültige Position, beginne von vorne
			calstat = 0;
		}
	}

	if(calstat == 8)
	{
		// prüfe Exit
		if(y >= (TFT_HEIGHT-50) && x > 300 && x < 500)
		{
			if(++doexit > 10)
			{
				menunum = MENU_SELECTION;
				messagebox("save values",1000);
				copy_ram_to_flash();
				show_selectionmenu();
				return;
			}
		}
		else
			doexit = 0;

		// in die Mitte des Symbols stellen
		x -= rectsize/2;
		y -= rectsize/2;

		// lösche altes Symbol
		if(oldx != -1 && oldy<420&& oldy > 30)
		{
			TFT_DrawLine(0,oldy,799,oldy, BLACK, 2);
			TFT_DrawLine(oldx,30,oldx,420, BLACK, 2);
		}

		// zeichne neues
		if(y<420 && y > 30)
		{
			TFT_DrawLine(0,y,799,y, WHITE, 2);
			TFT_DrawLine(x,30,x,420, WHITE, 2);
		}

		char s[50];
		int txtx = 580;
		sprintf(s,"x:%04d y:%04d",rawx,rawy);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s, txtx, 421, 2, WHITE , BLACK, TFT_WIDTH-1);
		sprintf(s,"l:%04d r:%04d",pcfgdata->touch_left,pcfgdata->touch_right);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s, txtx, 441, 2, WHITE , BLACK, TFT_WIDTH-1);
		sprintf(s,"t:%04d b:%04d",pcfgdata->touch_top,pcfgdata->touch_bottom);
		tft_print_restclear(&lucidaConsole_12ptFontInfo, s, txtx, 461, 2, WHITE , BLACK, TFT_WIDTH-1);

		oldx = x;
		oldy = y;
	}
}

void draw_values_touch()
{
	if(pcfgdata->display_reverse != 0)
	{
		if(calstat == 0)
		{
			TFT_Fill_Screen(BLACK);
			TFT_Fill_Rectangle(0,0,10,10,WHITE);
			tft_print(&lucidaConsole_22ptFontInfo, getLang(Touch_Top_Left_Corner), 120, 1, 1, WHITE , BLACK);
			calstat = 1;
		}
		if(calstat == 4)
		{
			TFT_Fill_Screen(BLACK);
			TFT_Fill_Rectangle(TFT_WIDTH-10,TFT_HEIGHT-10,TFT_WIDTH-1,TFT_HEIGHT-1,WHITE);
			tft_print(&lucidaConsole_22ptFontInfo, getLang(Touch_Bottom_Right_Corner), 120, 1, 1, WHITE , BLACK);
			calstat = 5;
		}
	}
	else
	{
		if(calstat == 4)
		{
			TFT_Fill_Screen(BLACK);
			TFT_Fill_Rectangle(0,0,10,10,WHITE);
			tft_print(&lucidaConsole_22ptFontInfo, getLang(Touch_Top_Left_Corner), 120, 1, 1, WHITE , BLACK);
			calstat = 5;
		}
		if(calstat == 0)
		{
			TFT_Fill_Screen(BLACK);
			TFT_Fill_Rectangle(TFT_WIDTH-10,TFT_HEIGHT-10,TFT_WIDTH-1,TFT_HEIGHT-1,WHITE);
			tft_print(&lucidaConsole_22ptFontInfo, getLang(Touch_Bottom_Right_Corner), 120, 1, 1, WHITE , BLACK);
			calstat = 1;
		}
	}

	if(calstat == 7)
	{
		TFT_Fill_Screen(BLACK);
		tft_print(&lucidaConsole_22ptFontInfo, getLang(touch_and_test_position), 100, 1, 1, WHITE , BLACK);
		tft_print(&verdana_36ptFontInfo, getLang(EXIT), (TFT_WIDTH*3)/8, TFT_HEIGHT-50, 8, BLACK , GREEN);
		calstat = 8;
	}
}

#define MENUREFRESHTIME_MS	100
uint32_t menutime = 0;
 void draw_values()
 {
	 // nicht zu oft
	 if(uptime < (menutime + MENUREFRESHTIME_MS)) return;
	 menutime = uptime;

	if(menunum == MENU_OVERVIEW) draw_values_overviewbar();
	else if(menunum == MENU_BAR) draw_values_bigbar();
	else if(menunum == MENU_METER) draw_values_meter();
	else if(menunum == MENU_BRIDGES) draw_value_bridges();
	// else if(menunum == MENU_HISTORY) // wird immer im Hintergrund gemacht
	else if(menunum == MENU_ADC) draw_values_adcvals();
	else if(menunum == MENU_VALUES) draw_values_realvals();
	else if(menunum == MENU_STATUS) draw_values_status();
	else if(menunum == MENU_SWITCH) draw_values_switch();
	else if(menunum == MENU_TOUCHCAL) draw_values_touch();
	//else if(menunum == MENU_SYSTEM) draw_values_system(); // nicht hier, wird in system.c direkt gemacht
 }

void draw_menu_emergency(float soll, float ist)
{
char s[100];

	tb_buttons[TB_EMERG_OFF].pressed = 0;
	draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);
	tb_buttons[TB_ACTIVE].pressed = 0;
	draw_touchbutton(&tb_buttons[TB_ACTIVE]);
	tb_buttons[TB_STANDBY].pressed = 1;
	draw_touchbutton(&tb_buttons[TB_STANDBY]);

	tft_print(&lucidaConsole_18ptFontInfo, getLang(NOTABSCHALTUNG), 0,0, 6, RED , WHITE);

	int x = 0;
	int y = 24;
	switch(error)
	{
	case EMERG_TEMP:
		tft_print(&lucidaConsole_18ptFontInfo, getLang(ERRORTEXT_TEMP), x,y, 2, RED , WHITE);
		break;
	case EMERG_TEMP2:
		tft_print(&lucidaConsole_18ptFontInfo, getLang(ERRORTEXT_TEMP), x,y, 2, RED , WHITE);
		break;
	case EMERG_USER:
		tft_print(&lucidaConsole_18ptFontInfo, getLang(ERRORTEXT_USER), x,y, 2, RED , WHITE);
		break;
	case EMERG_I:
		sprintf(s,"%s: %.1f > %.1f",getLang(ERRORTEXT_MAXI),ist,soll);
		tft_print(&lucidaConsole_18ptFontInfo, s, x,y, 2, RED , WHITE);
		break;
	case EMERG_U:
		tft_print(&lucidaConsole_18ptFontInfo, getLang(ERRORTEXT_MAXU), x,y, 2, RED , WHITE);
		break;
	case EMERG_DRV:
		sprintf(s,"%s: %.1f > %.1f",getLang(ERRORTEXT_MAXDRV),ist,soll);
		tft_print(&lucidaConsole_18ptFontInfo, s, x,y, 2, RED , WHITE);
		break;
	case EMERG_FILTERSWR:
		sprintf(s,"%s: %.1f > %.1f",getLang(ERRORTEXT_MAXFLT),ist,soll);
		tft_print(&lucidaConsole_18ptFontInfo, s, x,y, 2, RED , WHITE);
		break;
	case EMERG_ANTSWR:
		sprintf(s,"%s: %.1f > %.1f",getLang(ERRORTEXT_MAXANT),ist,soll);
		tft_print(&lucidaConsole_18ptFontInfo, s, x,y, 2, RED , WHITE);
		break;
	case EMERG_TXTIME:
		tft_print(&lucidaConsole_18ptFontInfo, getLang(ERRORTEXT_MAXTXTIME), x,y, 2, RED , WHITE);
		break;
	}
}

// ============= BIG BAR =====================

int bb_xleft = 40;
int bb_xright = 320;
int bb_y = 290;
int bb_yspace = 30;
int bb_xfan = 430;

void draw_menu_bar()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();
	show_bigbars();

	first_statdisplay = 1;

	int xspaceforicon = 36;
	int line = 0;

	tft_print(&verdana_18ptFontInfo, getLangadd(PS_Voltage,":"), bb_xleft, bb_y + line++ * bb_yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, getLangadd(PS_Current,":"), bb_xleft, bb_y + line++ * bb_yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, getLangadd(Temperature1,":"), bb_xleft, bb_y + line++ * bb_yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, getLangadd(Temperature2,":"), bb_xleft, bb_y + line++ * bb_yspace, 4, WHITE , BLACK);

	if(pcfgdata->systemMode == 0)
	{
		// Pamp Mode
		line = 0;
		tft_print(&verdana_18ptFontInfo, getLang(PTT_from_TRX), bb_xfan+xspaceforicon, bb_y + line++ * bb_yspace, 2, WHITE , BLACK);
		tft_print(&verdana_18ptFontInfo, getLang(PTT_out), bb_xfan+xspaceforicon, bb_y + line++ * bb_yspace, 2, WHITE , BLACK);
		tft_print(&verdana_18ptFontInfo, getLang(FAN), bb_xfan+xspaceforicon, bb_y + line++ * bb_yspace, 2, WHITE , BLACK);
	}
	else if(pcfgdata->systemMode == 1)
	{
		// PwrSwr Mode
		TFT_Rectangle(bb_xleft-10, bb_y-15, bb_xleft+440, bb_y + line * bb_yspace, RGB16(128,128,50), 2);
	}
}

void draw_values_bigbar()
 {
	 int y = bb_y-6;
	 int line = 0;
	 char s[50],st1[20],st2[20];

	 if(pcfgdata->systemMode == 0)
	 {
		 // Pamp mode
		 draw_statbutton(PTTIN, bb_xfan, y + line++ * bb_yspace, 0,0);
		 draw_statbutton(PTTOUT, bb_xfan, y + line++ * bb_yspace, 1,0);
		 draw_statbutton(FAN_ONOFF, bb_xfan, y + line * bb_yspace, 1,0);
	 }

	 fill_bar(&bar_big_power, pwrswr_antenna.fwd_watt, pwrswr_antenna.fwd_peakwatt);
	 fill_bar(&bar_big_swr, pwrswr_antenna.swr,-1);

	y += 6;
	line = 0;

	if(adc_realvals[UTEMP] > -9)
		sprintf(st1,"%6.1f",adc_realvals[UTEMP]);
	else
		sprintf(st1,"  --- ");

	if(adc_realvals[UTEMP2] > -9)
			sprintf(st2,"%6.1f",adc_realvals[UTEMP2]);
		else
			sprintf(st2,"  --- ");

	int rshift = 210;
	sprintf(s,"%6.1f V  ",adc_realvals[UMESS]);
	tft_print(&lucidaConsole_18ptFontInfo, s, bb_xleft+rshift, y + line++ * bb_yspace, 1, YELLOW , BLACK);
	sprintf(s,"%6.1f A  ",adc_realvals[IMESS]);
	tft_print(&lucidaConsole_18ptFontInfo, s, bb_xleft+rshift, y + line++ * bb_yspace, 1, YELLOW , BLACK);
	sprintf(st1+strlen(st1)," ~%s ", pcfgdata->tempunits?"F":"C");
	tft_print(&lucidaConsole_18ptFontInfo, st1, bb_xleft+rshift, y + line++ * bb_yspace, 1, YELLOW , BLACK);
	sprintf(st2+strlen(st2)," ~%s ", pcfgdata->tempunits?"F":"C");
	tft_print(&lucidaConsole_18ptFontInfo, st2, bb_xleft+rshift, y + line++ * bb_yspace, 1, YELLOW , BLACK);
 }

// ============= METER =================

void draw_menu_meter()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_mainmenu();

	setup_instrument(10,50);

	int by = 350;
	tft_print(&verdana_18ptFontInfo, getLang(FORWARD), 1, by, 4, WHITE, BLACK);
	tft_print(&verdana_18ptFontInfo, getLang(REVERSE), 1, by+30, 4, WHITE, BLACK);
	tft_print(&verdana_18ptFontInfo, "SWR:", 350, by, 4, WHITE, BLACK);

	int xleft = 425;
	int xspaceforicon = 38;
	int y = 50;
	int yspace = 30;
	int line = 0;

	if(pcfgdata->systemMode == 0)
	{
		// Pamp mode
		tft_print(&verdana_18ptFontInfo, getLang(PTT_from_TRX), xleft+xspaceforicon, y + line++ * yspace, 4, WHITE , BLACK);
		tft_print(&verdana_18ptFontInfo, getLang(PTT_out), xleft+xspaceforicon, y + line++ * yspace, 4, WHITE , BLACK);
		tft_print(&verdana_18ptFontInfo, getLang(FAN), xleft+xspaceforicon, y + line++ * yspace, 4, WHITE , BLACK);
	}

	line++;
	y += 6;

	tft_print(&verdana_18ptFontInfo, "DC:", xleft, y + line++ * yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, "DC:", xleft, y + line++ * yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, "DC:", xleft, y + line++ * yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, "T1:", xleft, y + line++ * yspace, 4, WHITE , BLACK);
	tft_print(&verdana_18ptFontInfo, "T2:", xleft, y + line++ * yspace, 4, WHITE , BLACK);

	draw_values_meter();
}

void draw_values_meter()
{
int xleft = 425;
int y = 50-6;
int yspace = 30;
int line = 0;
char s[20];

	draw_needle();

	int by = 350;
	sprintf(s,"%.0f W  ",pwrswr_antenna.fwd_watt);
	tft_print(&lucidaConsole_18ptFontInfo, s, 200, by, 2, YELLOW, BLACK);
	sprintf(s,"%.0f W  ",pwrswr_antenna.rev_watt);
	tft_print(&lucidaConsole_18ptFontInfo, s, 200, by+30, 2, RED, BLACK);
	sprintf(s,"%.2f:1   ",pwrswr_antenna.swr);
	tft_print(&lucidaConsole_18ptFontInfo, s, 450, by, 2, YELLOW, BLACK);

	if(pcfgdata->systemMode == 0)
	{
		// Pamp mode
		draw_statbutton(PTTIN, xleft, y + line++ * yspace, 0,0);
		draw_statbutton(PTTOUT, xleft, y + line++ * yspace, 1,0);
		draw_statbutton(FAN_ONOFF, xleft, y + line++ * yspace, 1,0);
	}

	line++;
	y += 12;
	xleft += 50;

	sprintf(s,"%6.1f V",adc_realvals[UMESS]);
	tft_print(&lucidaConsole_18ptFontInfo, s, xleft, y + line++ * yspace, 1, YELLOW , BLACK);
	sprintf(s,"%6.1f A",adc_realvals[IMESS]);
	tft_print(&lucidaConsole_18ptFontInfo, s, xleft, y + line++ * yspace, 1, YELLOW , BLACK);
	sprintf(s,"%6.1f W",adc_realvals[CALC_POWER]);
	tft_print(&lucidaConsole_18ptFontInfo, s, xleft, y + line++ * yspace, 1, YELLOW , BLACK);

	if(adc_realvals[UTEMP] > -9)
		sprintf(s,"%6.1f ",adc_realvals[UTEMP]);
	else
		sprintf(s,"   --- ");

	sprintf(s+strlen(s),"~%s ", pcfgdata->tempunits?"F":"C");
	tft_print(&lucidaConsole_18ptFontInfo, s, xleft, y + line++ * yspace, 1, YELLOW , BLACK);

	if(adc_realvals[UTEMP2] > -9)
		sprintf(s,"%6.1f ",adc_realvals[UTEMP2]);
	else
		sprintf(s,"   --- ");

	sprintf(s+strlen(s),"~%s ", pcfgdata->tempunits?"F":"C");
	tft_print(&lucidaConsole_18ptFontInfo, s, xleft, y + line++ * yspace, 1, YELLOW , BLACK);
}

uint8_t showstatus = 0;

void statusline()
{
char s[150] = {0};
int x1 = 240;
int x2 = 370;
int x3 = 500;
int y = TFT_HEIGHT-15;
static uint8_t old_antsw = 255;
static uint8_t oldcivavail = 255;
static uint8_t old_selected_bandrelais = 255;
static uint8_t old_civadr = 255;
static uint8_t old_inputmode = 255;
static uint32_t old_freq = -1;
int bcol = RGB16(40,40,40);
uint32_t civadr_khz = civ_freq/1000;

	if(menunum == MENU_TOUCHCAL || menunum == MENU_STARTUP) return;

	if(showstatus || pcfgdata->antsw != old_antsw || civ_avail != oldcivavail || selected_bandrelais != old_selected_bandrelais || old_freq != civadr_khz
			|| old_civadr != civ_adr || old_inputmode != pcfgdata->auxInputMode)
	{
		TFT_Fill_Rectangle(201, y, TFT_WIDTH-1, TFT_HEIGHT, bcol);

		sprintf(s,"%s: %d",getLang(ANTENNA),pcfgdata->antsw+1);
		tft_print(&verdana_12ptFontInfo, s, x1, y, 1, WHITE, bcol);

		if(pcfgdata->systemMode == 0)
		{
			sprintf(s,"%s: %d", getLang(FILTERBANK),selected_bandrelais+1);
			tft_print(&verdana_12ptFontInfo, s, x2, y, 1, WHITE, bcol);
		}

		if(pcfgdata->auxInputMode == PTT4x)
		{
			s[0] = 0;
			if(gpio_stat[ROTS_B1] == 0) sprintf(s,"TRX: 1");
			if(gpio_stat[ROTS_B2] == 0) sprintf(s,"TRX: 2");
			if(gpio_stat[ROTS_B3] == 0) sprintf(s,"TRX: 3");
			if(gpio_stat[ROTS_B4] == 0) sprintf(s,"TRX: 4");

			if(civ_avail == 1)
			{
				sprintf(s+strlen(s),"    %8.6f MHz    CIV: %02X",((float)civ_freq)/1000000,civ_adr);
			}
			tft_print(&verdana_12ptFontInfo, s, x3, y, 1, WHITE, bcol);
		}
		else
		{
			if(civ_avail == 1)
			{
				sprintf(s,"    %8.6f MHz    CIV: %02X",((float)civ_freq)/1000000,civ_adr);
				tft_print(&verdana_12ptFontInfo, s, x3, y, 1, WHITE, bcol);
			}
		}
	}


	old_antsw = pcfgdata->antsw;
	oldcivavail = civ_avail;
	old_selected_bandrelais = selected_bandrelais;
	old_freq = civadr_khz;
	old_civadr = civ_adr;
	old_inputmode = pcfgdata->auxInputMode;
	showstatus = 0;
}

void old_statusline()
{
char s[30] = {0};
int x = 570;
int y = 5;
static uint8_t old_antsw = 255;
static uint8_t oldcivavail = 255;
static uint8_t old_selected_bandrelais = 255;

	if(menunum < MENU_OVERVIEW || menunum > MENU_STATUS) return;

	if(showstatus || pcfgdata->antsw != old_antsw || civ_avail != oldcivavail || selected_bandrelais != old_selected_bandrelais)
	{
		TFT_Fill_Rectangle(x, y, 649, y+12, BLACK);

		sprintf(s,"A%d",pcfgdata->antsw+1);

		sprintf(s+strlen(s)," B%d", selected_bandrelais+1);

		if(civ_avail == 1)
			sprintf(s+strlen(s),"  CIV");
	}

	tft_print(&verdana_10ptFontInfo, s, x, y, 1, WHITE, BLACK);

	old_antsw = pcfgdata->antsw;
	oldcivavail = civ_avail;
	old_selected_bandrelais = selected_bandrelais;
	showstatus = 0;
}
