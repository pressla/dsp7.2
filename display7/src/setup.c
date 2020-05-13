/*
 * setup.c
 *
 *  Created on: 27.09.2017
 *      Author: kurt
 */

#include <main.h>

#define ITEMS_TOPMARGIN 60
#define ITEMS_BOTTOMMARGIN 400
#define SETUP_ITEM_ANZ	7
#define SETUP_ITEM_MAXLEN	30

int editline = TB_SETUP1;
int setup_modified = 0;

char esetup_items[SETUP_ITEM_ANZ][SETUP_ITEM_MAXLEN] =
{
		"max. I [A]",
		"max. U [V]",
		"max. Temp",
		"max. TX[minutes]",
		"FAN ON temp",
		"PTT Delay [ms]",
		"max. Drive [W]",
};

char dsetup_items[SETUP_ITEM_ANZ][SETUP_ITEM_MAXLEN] =
{
		"max. I [A]      ",
		"max. U [V]      ",
		"max. Temp",
		"max. TX[Minuten]",
		"Luefter EIN Temp",
		"PTT Verz. [ms]",
		"max. Eingang[W] ",
};

char (*setup_items)[SETUP_ITEM_MAXLEN] = esetup_items;


// Zeichne feste Bestandteile des Setup Menüs
void draw_menu_setup()
{
	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_bottom();
	tft_print(&verdana_36ptFontInfo, getLang(LIMITS), 14, 1, 5, WHITE , BLACK);

	draw_setup_buttons();
	tft_print(&lucidaConsole_18ptFontInfo, "x100  x10   x1", 570, 197, 1, WHITE , BLACK);

	editline = 0;
	setup_drawvalues();
	editline = TB_SETUP1;
}

void draw_setup_buttons()
{
int anz = SETUP_ITEM_ANZ;

	if(pcfgdata->systemMode == 1) anz-=2;

	draw_touchbutton(&tb_buttons[TB_P100]);
	draw_touchbutton(&tb_buttons[TB_P10]);
	draw_touchbutton(&tb_buttons[TB_P1]);
	draw_touchbutton(&tb_buttons[TB_M100]);
	draw_touchbutton(&tb_buttons[TB_M10]);
	draw_touchbutton(&tb_buttons[TB_M1]);

	// aktiviere erstes Menü
	for(int i=TB_SETUP1; i<(TB_SETUP1+anz); i++)
	{
		if(i == TB_SETUP1) tb_buttons[i].pressed = 1;
		else tb_buttons[i].pressed = 0;
		draw_touchbutton(&tb_buttons[i]);
	}
}

void init_setup_buttons()
{
int text_rect_width = 320;
int anz = SETUP_ITEM_ANZ;

	if(pcfgdata->systemMode == 1) anz-=2;

	int itemysize = (ITEMS_BOTTOMMARGIN-ITEMS_TOPMARGIN) / anz;

	setup_items = esetup_items;
	if(pcfgdata->language == 1) setup_items = dsetup_items;

	for(int i=0; i<anz; i++)
	{
		int y = ITEMS_TOPMARGIN + i * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		setup_touchbutton(&tb_buttons[TB_SETUP1+i],0,ys,text_rect_width,ye-ys,LIGHTBLUE,YELLOW,BLACK,setup_items[i],lucidaConsole_22ptFontInfo, 0,NULL,NULL);
	}
}

// es wurde auf einen Button der Setup-Zeilen gedrückt
// aktiviere diese Zeile als aktuell zu editierende Zeile
// außerdem bearbeite die Ziffernbuttons
int do_button_setup(int touchbutton)
{
int anz = SETUP_ITEM_ANZ;

	if(pcfgdata->systemMode == 1) anz-=2;

	if(touchbutton <= TB_P1000 || touchbutton == TB_M1000) return 0;	// diese Buttons interessieren uns hier nicht

	tb_buttons[touchbutton].pressed = 1;

	if(touchbutton > TB_P1000 && touchbutton <= TB_M1)
	{
		for(int i=TB_P100; i<=TB_M1; i++)
		{
			if(i == TB_M1000) continue;
			if(i != touchbutton)
			{
				tb_buttons[i].pressed = 0;
				draw_touchbutton(&tb_buttons[i]);
			}
			else
			{
				tb_buttons[i].pressed = 1;
				draw_touchbutton(&tb_buttons[i]);
				setup_changevalue(i);
			}
		}
		setup_modified = 1;
	}

	if(touchbutton >= TB_SETUP1 && touchbutton < (TB_SETUP1+anz))
	{
		editline = touchbutton;

		for(int i=TB_SETUP1; i<(TB_SETUP1+anz); i++)
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
				editline = i;
			}
		}
	}
	return 1;
}

char conttext[2][20] =
{
	"ON",
	"EIN"
};

char txontext[2][20] =
{
	"TX-ON",
	"TXein"
};

void setup_drawvalues()
{
int x = 325;
int ytxt = 15;
char s[20] = {0};
int anz = SETUP_ITEM_ANZ;

	if(pcfgdata->systemMode == 1) anz-=2;
	int itemysize = (ITEMS_BOTTOMMARGIN-ITEMS_TOPMARGIN) / anz;

	for(int i=TB_SETUP1; i<(TB_SETUP1+anz); i++)
	{
		int y = ITEMS_TOPMARGIN + (i-TB_SETUP1) * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		if(i == editline || editline == 0)
		{
			TFT_Fill_Rectangle(x,ys+2,x+130,ye-1,YELLOW);
			if(i == TB_SETUP1) sprintf(s,"% 5d",pcfgdata->I_limit);
			if(i == TB_SETUP2) sprintf(s,"% 5d",pcfgdata->U_limit);
			if(i == TB_SETUP3) sprintf(s,"% 5d",pcfgdata->Temp_limit);
			if(i == TB_SETUP4) sprintf(s,"% 5d",pcfgdata->Time_limit_minutes);
			if(i == TB_SETUP5)
			{
				if(pcfgdata->fan_temp == 7)
					sprintf(s,"%*s",5,"AUTO");
				else if(pcfgdata->fan_temp == 8)
					sprintf(s,"%*s",5,conttext[pcfgdata->language]);
				else if(pcfgdata->fan_temp == 9)
					sprintf(s,"%*s",5,txontext[pcfgdata->language]);
				else
					sprintf(s,"% 5d",pcfgdata->fan_temp);
			}
			if(i == TB_SETUP6) sprintf(s,"% 5d",pcfgdata->pttdelay);
			if(i == TB_SETUP7) sprintf(s,"% 5d",pcfgdata->Drive_limit);
			tft_print(&lucidaConsole_22ptFontInfo, s, x+10, y+ytxt, 1, BLUE , YELLOW);
		}
	}
}

void setup_changevalue(int touchbutton)
{
uint16_t *val = &pcfgdata->I_limit;
int16_t ival;

	if(editline == TB_SETUP2) val = &pcfgdata->U_limit;
	if(editline == TB_SETUP3) val = &pcfgdata->Temp_limit;
	if(editline == TB_SETUP4) val = &pcfgdata->Time_limit_minutes;
	if(editline == TB_SETUP5) val = &pcfgdata->fan_temp;
	if(editline == TB_SETUP6) val = &pcfgdata->pttdelay;
	if(editline == TB_SETUP7) val = &pcfgdata->Drive_limit;

	ival = *val;

	if(touchbutton >= TB_P1 && touchbutton <= TB_M100 && touchbutton <= TB_P1000)
		setup_modified = 1;

	// stelle den Wert ein
	if(touchbutton == TB_P1) ival++;
	if(touchbutton == TB_P10) ival += 10;
	if(touchbutton == TB_P100) ival += 100;

	if(touchbutton == TB_M1) ival--;
	if(touchbutton == TB_M10) ival -= 10;
	if(touchbutton == TB_M100) ival -= 100;

	// Begrenze auf max/min Werte
	if(editline == TB_SETUP1)
	{
		if(ival < 1) ival = 1;
		if(ival > 150) ival = 150;
	}
	if(editline == TB_SETUP2)
	{
		if(ival < 10) ival = 10;
		if(ival > 150) ival = 150;
	}
	if(editline == TB_SETUP3)
	{
		if(ival < 10) ival = 10;
		if(ival > 80) ival = 80;
	}
	if(editline == TB_SETUP4)
	{
		if(ival < 1) ival = 1;
		if(ival > 30) ival = 30;
	}
	if(editline == TB_SETUP5)
	{
		if(ival < 7) ival = 7;
		if(ival > 80) ival = 80;
	}
	if(editline == TB_SETUP6)
	{
		if(ival < 0) ival = 0;
		if(ival > 999) ival = 999;
	}
	if(editline == TB_SETUP7)
	{
		if(ival < 1) ival = 1;
		if(ival > 50) ival = 50;
	}

	*val = ival;

	setup_drawvalues();
}
