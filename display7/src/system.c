/*
 * system.c
 *
 *  Created on: 30.09.2017
 *      Author: kurt
 */

#include <main.h>

void system_changevalue(int touchbutton);

#define ITEMS_TOPMARGIN 54
#define ITEMS_BOTTOMMARGIN 414
#define SYSTEM_ITEM_ANZ	9
#define SYSTEM_ITEM_ANZ_PWR	7

int system_editline = TB_SYSTEM1;
unsigned char tmp_display_reverse;

char esystem_items[SYSTEM_ITEM_ANZ][20] =
{
		"Language/Sprache",
		"Display Reverse ",
		"Temperat.  Units",
		"DC Voltage Range",
		"DC Current Range",
		"Bands Mode      ",
		"RF Power Range  ",
		"Drive Pwr  Range",
		"Shunt val[mohms]",
};

char dsystem_items[SYSTEM_ITEM_ANZ][20] =
{
		"Language/Sprache   ",
		"Display Reverse    ",
		"Temperat.Einheit   ",
		"DC Spannung Bereich",
		"DC Strom Bereich   ",
		"Band Modus         ",
		"Ausg.Leist.Bereich ",
		"Eing.Leist.Bereich ",
		"Shunt Wert  [mOhm] ",
};

char (*system_items)[20];

void setDisplayReverse()
{
	if(pcfgdata->display_reverse != tmp_display_reverse)
	{
		pcfgdata->display_reverse = tmp_display_reverse;
		setDisplayamKopf();
	}
}

// Zeichne feste Bestandteile des System Menüs
void draw_menu_system()
{
	tmp_display_reverse = pcfgdata->display_reverse;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_bottom();
	tft_print(&verdana_36ptFontInfo, getLang(SYSTEM_Settings), 14, 1, 5, WHITE , BLACK);

	draw_system_buttons();

	system_editline = 0;
	draw_values_system();
	system_editline = TB_SYSTEM1;
}

void draw_system_buttons()
{
int anz = SYSTEM_ITEM_ANZ;
	if(pcfgdata->systemMode == 1) anz = SYSTEM_ITEM_ANZ_PWR;

	draw_touchbutton(&tb_buttons[TB_P1]);
	draw_touchbutton(&tb_buttons[TB_M1]);

	// aktiviere erstes Menü
	for(int i=TB_SYSTEM1; i<(TB_SYSTEM1+anz); i++)
	{
		if(i == TB_SYSTEM1) tb_buttons[i].pressed = 1;
		else tb_buttons[i].pressed = 0;
		draw_touchbutton(&tb_buttons[i]);
	}
}

void init_system_buttons()
{
int anz = SYSTEM_ITEM_ANZ;
	if(pcfgdata->systemMode == 1) anz = SYSTEM_ITEM_ANZ_PWR;

int itemysize = (ITEMS_BOTTOMMARGIN-ITEMS_TOPMARGIN) / anz;
int text_rect_width = 360;

	system_items = esystem_items;
	if(pcfgdata->language == 1) system_items = dsystem_items;

	for(int i=0; i<anz; i++)
	{
		int y = ITEMS_TOPMARGIN + i * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		setup_touchbutton(&tb_buttons[TB_SYSTEM1+i],0,ys,text_rect_width,ye-ys,LIGHTBLUE,YELLOW,BLACK,system_items[i],lucidaConsole_22ptFontInfo, 0,NULL,NULL);
	}
}

// es wurde auf einen Button der System-Zeilen gedrückt
// aktiviere diese Zeile als aktuell zu editierende Zeile
// außerdem bearbeite die Ziffernbuttons
int do_button_system(int touchbutton)
{
	int anz = SYSTEM_ITEM_ANZ;
		if(pcfgdata->systemMode == 1) anz = SYSTEM_ITEM_ANZ_PWR;

	if(touchbutton < TB_P1000 || (touchbutton >= TB_SETUP1 && touchbutton < TB_SYSTEM1)) return 0;	// diese Buttons interessieren uns hier nicht

	if(touchbutton == TB_P1 || touchbutton == TB_M1)
	{
		tb_buttons[TB_M1].pressed = 0;
		tb_buttons[TB_P1].pressed = 0;
		tb_buttons[touchbutton].pressed = 1;
		draw_touchbutton(&tb_buttons[TB_M1]);
		draw_touchbutton(&tb_buttons[TB_P1]);
		system_changevalue(touchbutton);
	}

	if(touchbutton >= TB_SYSTEM1 && touchbutton < (TB_SYSTEM1+anz))
	{
		system_editline = touchbutton;

		for(int i=TB_SYSTEM1; i<(TB_SYSTEM1+anz); i++)
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
				system_editline = i;
			}
		}
	}
	return 1;
}

char languagetext[2][10] =
{
		"English",
		"Deutsch"
};

char disprevtext[2][10] =
{
		"Normal ",
		"Reverse"
};

char tempunitstext[2][15] =
{
		"Celsius   ",
		"Fahrenheit"
};

char maxpwrunitstext[4][10] =
{
		"250  ",
		"1000 ",
		"2500 ",
		"10000"
};

char maxdrvpwrunitstext[4][10] =
{
		"6  ",
		"15  ",
		"30 ",
		"50 "
};

char maxUunitstext[4][10] =
{
		"50   ",
		"100  ",
		"1000 ",
		"4000 "
};

char maxIunitstext[4][10] =
{
		"10   ",
		"50   ",
		"100  ",
		"200  "
};

char shuntRtext[4][10] =
{
		"1   ",
		"2.5 ",
		"5   ",
		"10  "
};

float shuntValue[4] =
{
	1,
	2.5,
	5,
	10
};

void draw_values_system()
{
int anz = SYSTEM_ITEM_ANZ;
	if(pcfgdata->systemMode == 1)  anz = SYSTEM_ITEM_ANZ_PWR;

int x = 370;
int xsize = 260;
int ytxt = 8;
int itemysize = (ITEMS_BOTTOMMARGIN-ITEMS_TOPMARGIN) / anz;
char s[20] = {0};

	for(int i=TB_SYSTEM1; i<(TB_SYSTEM1+anz); i++)
	{
		int y = ITEMS_TOPMARGIN + (i-TB_SYSTEM1) * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		if(i == system_editline || system_editline == 0)
		{
			TFT_Rectangle(x,ys+2,x+xsize,ye-1,WHITE,2);
			if(i == TB_SYSTEM1) strcpy(s,languagetext[pcfgdata->language]);
			if(i == TB_SYSTEM2) strcpy(s,disprevtext[tmp_display_reverse]);
			if(i == TB_SYSTEM3) strcpy(s,tempunitstext[pcfgdata->tempunits]);
			if(i == TB_SYSTEM4) strcpy(s,maxUunitstext[pcfgdata->maxUunits]);
			if(i == TB_SYSTEM5) strcpy(s,maxIunitstext[pcfgdata->maxIunits]);
			if(i == TB_SYSTEM6) strcpy(s,bandmode_text[pcfgdata->bandmode]);
			if(i == TB_SYSTEM7) strcpy(s,maxpwrunitstext[pcfgdata->maxpwrunits]);
			if(i == TB_SYSTEM8) strcpy(s,maxdrvpwrunitstext[pcfgdata->maxdrvpwrunits]);
			if(i == TB_SYSTEM9) strcpy(s,shuntRtext[pcfgdata->shuntR]);
			strcat(s,"  ");
			tft_print(&lucidaConsole_22ptFontInfo, s, x+20, y+ytxt, 1, WHITE , BLACK);
		}
	}
}

unsigned char changeValue(int touchbutton, unsigned char v, unsigned char max)
{
	if(touchbutton == TB_P1)
	{
		if(v == max) v = 0;
		else v++;
	}
	if(touchbutton == TB_M1)
	{
		if(v == 0) v = max;
		else v--;
	}

	return v;
}

void system_changevalue(int touchbutton)
{
	if(system_editline == TB_SYSTEM1)
	{
		pcfgdata->language = (pcfgdata->language?0:1);
		init_touchbuttons();
	}

	if(system_editline == TB_SYSTEM2)
	{
		tmp_display_reverse = (tmp_display_reverse?0:1);
	}

	if(system_editline == TB_SYSTEM3)
	{
		pcfgdata->tempunits = (pcfgdata->tempunits?0:1);
	}

	if(system_editline == TB_SYSTEM4)
		pcfgdata->maxUunits = changeValue(touchbutton, pcfgdata->maxUunits, 3);

	if(system_editline == TB_SYSTEM5)
		pcfgdata->maxIunits = changeValue(touchbutton, pcfgdata->maxIunits, 3);

	if(system_editline == TB_SYSTEM6)
	{
		pcfgdata->bandmode = changeValue(touchbutton, pcfgdata->bandmode, NUMBANDMODES-1);
		init_touchbuttons();
	}

	if(system_editline == TB_SYSTEM7)
		pcfgdata->maxpwrunits = changeValue(touchbutton, pcfgdata->maxpwrunits, 3);

	if(system_editline == TB_SYSTEM8)
		pcfgdata->maxdrvpwrunits = changeValue(touchbutton, pcfgdata->maxdrvpwrunits, 3);

	if(system_editline == TB_SYSTEM9)
		pcfgdata->shuntR = changeValue(touchbutton, pcfgdata->shuntR, 3);

	draw_values_system();
	setup_modified = 1;
}
