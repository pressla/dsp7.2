/*
 * system2.c
 *
 *  Created on: 07.03.2018
 *      Author: kurt
 */

#include <main.h>

void system_changevalue2(int touchbutton);

#define ITEMS2_TOPMARGIN 54
#define SYSTEM_ITEM2_ANZ	6
#define ITEMS2_BOTTOMMARGIN ITEMS2_TOPMARGIN+SYSTEM_ITEM2_ANZ*40 //414 (414 ist das Maximum ganz unten)

int system_editline2 = TB_SYSTEM_2_1;

char esystem_items2[SYSTEM_ITEM2_ANZ][20] =
{
	"Aux Input Mode  ",
	"CI-V Adr.1 deflt",
	"CI-V Adr.2      ",
	"CI-V Adr.3      ",
	"CI-V Adr.4      ",
	"System Mode     ",
};

char dsystem_items2[SYSTEM_ITEM2_ANZ][20] =
{
	"Zusatzeing.Modus",
	"CI-V Adr.1 Std. ",
	"CI-V Adr.2      ",
	"CI-V Adr.3      ",
	"CI-V Adr.4      ",
	"System Modus    ",
};

char (*system_items2)[20];

// Zeichne feste Bestandteile des System Menüs
void draw_menu_system2()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_bottom();
	tft_print(&verdana_36ptFontInfo, getLang(SYSTEM_Settings), 14, 1, 5, WHITE , BLACK);

	draw_system_buttons2();

	system_editline2 = 0;
	draw_values_system2();
	system_editline2 = TB_SYSTEM_2_1;
}

void draw_system_buttons2()
{
int anz = SYSTEM_ITEM2_ANZ;

	draw_touchbutton(&tb_buttons[TB_P1]);
	draw_touchbutton(&tb_buttons[TB_M1]);

	// aktiviere erstes Menü
	for(int i=TB_SYSTEM_2_1; i<(TB_SYSTEM_2_1+anz); i++)
	{
		if(i == TB_SYSTEM_2_1) tb_buttons[i].pressed = 1;
		else tb_buttons[i].pressed = 0;
		draw_touchbutton(&tb_buttons[i]);
	}
}

void init_system_buttons2()
{
int anz = SYSTEM_ITEM2_ANZ;
int itemysize = (ITEMS2_BOTTOMMARGIN-ITEMS2_TOPMARGIN) / anz;
int text_rect_width = 360;

	system_items2 = esystem_items2;
	if(pcfgdata->language == 1) system_items2 = dsystem_items2;

	for(int i=0; i<anz; i++)
	{
		int y = ITEMS2_TOPMARGIN + i * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		setup_touchbutton(&tb_buttons[TB_SYSTEM_2_1+i],0,ys,text_rect_width,ye-ys,LIGHTBLUE,YELLOW,BLACK,system_items2[i],lucidaConsole_22ptFontInfo, 0,NULL,NULL);
	}
}

// es wurde auf einen Button der System-Zeilen gedrückt
// aktiviere diese Zeile als aktuell zu editierende Zeile
// außerdem bearbeite die Ziffernbuttons
int do_button_system2(int touchbutton)
{
	int anz = SYSTEM_ITEM2_ANZ;

	if(touchbutton < TB_P1000 || (touchbutton >= TB_SETUP1 && touchbutton < TB_SYSTEM_2_1)) return 0;	// diese Buttons interessieren uns hier nicht

	if(touchbutton == TB_P1 || touchbutton == TB_M1)
	{
		tb_buttons[TB_M1].pressed = 0;
		tb_buttons[TB_P1].pressed = 0;
		tb_buttons[touchbutton].pressed = 1;
		draw_touchbutton(&tb_buttons[TB_M1]);
		draw_touchbutton(&tb_buttons[TB_P1]);
		system_changevalue2(touchbutton);
	}

	if(touchbutton >= TB_SYSTEM_2_1 && touchbutton < (TB_SYSTEM_2_1+anz))
	{
		system_editline2 = touchbutton;

		for(int i=TB_SYSTEM_2_1; i<(TB_SYSTEM_2_1+anz); i++)
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
				system_editline2 = i;
			}
		}
	}
	return 1;
}

char sysmodetxt[2][14] =
{
		"PA Controller",
		"Pwr/SWR Meter"
};

char auxtext_en[2][12] =
{
		"Rotary Sw. ",
		"PTT 4 input"
};

char auxtext_de[2][12] =
{
		"Drehschalt.",
		"4xPTT Eing."
};

void draw_values_system2()
{
int anz = SYSTEM_ITEM2_ANZ;

int x = 370;
int xsize = 300;
int ytxt = 8;
int itemysize = (ITEMS2_BOTTOMMARGIN-ITEMS2_TOPMARGIN) / anz;
char s[20] = {0};

	for(int i=TB_SYSTEM_2_1; i<(TB_SYSTEM_2_1+anz); i++)
	{
		int y = ITEMS2_TOPMARGIN + (i-TB_SYSTEM_2_1) * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		if(i == system_editline2 || system_editline2 == 0)
		{
			TFT_Rectangle(x,ys+2,x+xsize,ye-1,WHITE,2);
			if(i == TB_SYSTEM_2_1) strcpy(s,pcfgdata->language?auxtext_de[pcfgdata->auxInputMode]:auxtext_en[pcfgdata->auxInputMode]);
			if(i == TB_SYSTEM_2_2) sprintf(s,"%02X", pcfgdata->civ_adr1);
			if(i == TB_SYSTEM_2_3) sprintf(s,"%02X", pcfgdata->civ_adr2);
			if(i == TB_SYSTEM_2_4) sprintf(s,"%02X", pcfgdata->civ_adr3);
			if(i == TB_SYSTEM_2_5) sprintf(s,"%02X", pcfgdata->civ_adr4);
			if(i == TB_SYSTEM_2_6) strcpy(s,sysmodetxt[pcfgdata->systemMode]);

			strcat(s,"  ");
			tft_print(&lucidaConsole_22ptFontInfo, s, x+20, y+ytxt, 1, WHITE , BLACK);
		}
	}
}

void system_changevalue2(int touchbutton)
{
	if(system_editline2 == TB_SYSTEM_2_1)
	{
		pcfgdata->auxInputMode = (pcfgdata->auxInputMode?0:1);
	}

	if(system_editline2 == TB_SYSTEM_2_2)
	{
		pcfgdata->civ_adr1 = changeValue(touchbutton, pcfgdata->civ_adr1, 0xff);
	}

	if(system_editline2 == TB_SYSTEM_2_3)
	{
		pcfgdata->civ_adr2 = changeValue(touchbutton, pcfgdata->civ_adr2, 0xff);
	}

	if(system_editline2 == TB_SYSTEM_2_4)
	{
		pcfgdata->civ_adr3 = changeValue(touchbutton, pcfgdata->civ_adr3, 0xff);
	}

	if(system_editline2 == TB_SYSTEM_2_5)
	{
		pcfgdata->civ_adr4 = changeValue(touchbutton, pcfgdata->civ_adr4, 0xff);
	}

	if(system_editline2 == TB_SYSTEM_2_6)
	{
		pcfgdata->systemMode = (pcfgdata->systemMode?0:1);
	}

	draw_values_system2();
	setup_modified = 1;
}
