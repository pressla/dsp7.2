/*
 * calib.c
 *
 *  Created on: 28.09.2017
 *      Author: kurt
 */

#include <main.h>

void draw_calib_buttons();
void select_gridfield(int line, int cols);
void calib_changevalue(int touchbutton);
void calib_readvalue();

#define CALIB_LINES		7
#define CALIB_COLS		3
#define CALIB_YTOP  	60
#define CALIB_YBOTTOM	410
#define CALIB_YSPACE	((CALIB_YBOTTOM - CALIB_YTOP)/CALIB_LINES)
#define CALIB_XLEFT		1
#define CALIB_XRIGHT	475
#define CALIB_XSPACE	((CALIB_XRIGHT - CALIB_XLEFT) / CALIB_COLS)

int selline = 1;
int selcol = 1;

char calib_linetext_pa[CALIB_LINES][10] =
{
		"Pwr/Swr",
		"ANT min",
		"ANT max",
		"FLT min",
		"FLT max",
		"IN  min",
		"IN  max",
};

char calib_linetext_pwrswr[CALIB_LINES][10] =
{
		"Pwr/Swr  ",
		"BRG-1 min",
		"BRG-1 max",
		"BRG-2 min",
		"BRG-2 max",
		"BRG-3 min",
		"BRG-3 max",
};

char (*calib_linetext)[10];

// zeichne Calib-Buttons
#define CALHELPY_num 340
#define CALHELPY_val 150

void draw_buttons_calib()
{
	if(selcol == 1)
	{
		TFT_Fill_Rectangle(CALIB_XRIGHT+2,100,TFT_WIDTH-1,410,BLACK);
		draw_touchbutton(&tb_buttons[TB_P1000]);
		draw_touchbutton(&tb_buttons[TB_P100]);
		draw_touchbutton(&tb_buttons[TB_P10]);
		draw_touchbutton(&tb_buttons[TB_P1]);
		draw_touchbutton(&tb_buttons[TB_M1000]);
		draw_touchbutton(&tb_buttons[TB_M100]);
		draw_touchbutton(&tb_buttons[TB_M10]);
		draw_touchbutton(&tb_buttons[TB_M1]);
		tft_print(&lucidaConsole_18ptFontInfo, "x1000 x100  x10   x1", 480, 197, 1, WHITE , BLACK);
		tft_print(&lucidaConsole_12ptFontInfo, getLang(GETCALHELP1), 550, CALHELPY_num, 1, WHITE , BLACK);
		tft_print(&lucidaConsole_12ptFontInfo, getLang(GETCALHELP2), 550, CALHELPY_num+20, 1, WHITE , BLACK);
	}
	else
	{
		TFT_Fill_Rectangle(CALIB_XRIGHT+2,100,TFT_WIDTH-1,410,BLACK);
		draw_touchbutton(&tb_buttons[TB_GETVAL]);
		tft_print(&lucidaConsole_12ptFontInfo, getLang(GETVALHELP1), 550, CALHELPY_val, 1, WHITE , BLACK);
		tft_print(&lucidaConsole_12ptFontInfo, getLang(GETVALHELP2), 550, CALHELPY_val+20, 1, WHITE , BLACK);
	}
}

// Zeichne feste Bestandteile des Calib Menüs
void draw_menu_calib()
{
	calib_linetext = calib_linetext_pa;
	if(pcfgdata->systemMode == 1) calib_linetext = calib_linetext_pwrswr;

	TFT_Fill_Screen(BLACK);
	draw_touchbuttons_bottom();
	tft_print(&verdana_36ptFontInfo, getLang(POWER_SWR_CALIBRATION), 14, 1, 5, WHITE , BLACK);

	draw_buttons_calib();

	for(int line = 0; line < CALIB_LINES; line++)
	{
		int y = CALIB_YTOP + CALIB_YSPACE * line;
		for(int cols = 0; cols < CALIB_COLS; cols++)
		{
			int x = CALIB_XLEFT + CALIB_XSPACE * cols;
			TFT_Rectangle(x, y, x + CALIB_XSPACE-1, y + CALIB_YSPACE-1, WHITE, 1);

			if(line == 0 && cols == 1)
				tft_print(&lucidaConsole_18ptFontInfo, "TX Pwr [W]", x+10, y+12, 0, WHITE , BLACK);
			if(line == 0 && cols == 2)
				tft_print(&lucidaConsole_18ptFontInfo, "Bridge[mV]", x+10, y+12, 0, WHITE , BLACK);
		}
		tft_print(&lucidaConsole_18ptFontInfo, calib_linetext[line], CALIB_XLEFT+17, y+13, 1, WHITE , BLACK);
	}

	TFT_Hor_Line(CALIB_XLEFT, CALIB_XLEFT + CALIB_XSPACE * 3 - 1, CALIB_YTOP + CALIB_YSPACE - 4, WHITE, BLACK, 3,0);

	draw_calib_buttons();
}

float *getfValsPtr(float *fp, int y, int x)
{
	if(x == 1)
	{
		if(y == 1) fp = &pcfgdata->ant_W_low;
		if(y == 2) fp = &pcfgdata->ant_W_high;
		if(y == 3) fp = &pcfgdata->flt_W_low;
		if(y == 4) fp = &pcfgdata->flt_W_high;
		if(y == 5) fp = &pcfgdata->drv_W_low;
		if(y == 6) fp = &pcfgdata->drv_W_high;
	}

	if(x == 2)
	{
		if(y == 1) fp = &pcfgdata->ant_mV_low;
		if(y == 2) fp = &pcfgdata->ant_mV_high;
		if(y == 3) fp = &pcfgdata->flt_mV_low;
		if(y == 4) fp = &pcfgdata->flt_mV_high;
		if(y == 5) fp = &pcfgdata->drv_mV_low;
		if(y == 6) fp = &pcfgdata->drv_mV_high;
	}

	return fp;
}

void drawCalibVppHelp(int line)
{
	float *pval = NULL;
	pval = getfValsPtr(pval, line,1);
	// val hat den Watt-Wert vom Button
	// rechne in Vpp um
	// Veff = Wurzel(P * R)
	// Vpp = 2 * Veff * Wurzel(2)
	// Vpp = 2 * Wurzel(P * R) * Wurzel(2)
	float vpp = 2.0 * sqrt(*pval * 50.0) * sqrt(2);
	char s[30];
	sprintf(s,"%6.1fW -> %6.1fVpp",*pval,vpp);
	tft_print(&lucidaConsole_12ptFontInfo, s, 490, 65, 1, WHITE , BLACK);
}

void draw_calib_buttons()
{
char s[20];
int fcolor,bcolor;

	for(int line = 1; line < CALIB_LINES; line++)
	{
		int y = CALIB_YTOP + CALIB_YSPACE * line;
		for(int cols = 1; cols < CALIB_COLS; cols++)
		{
			int x = CALIB_XLEFT + CALIB_XSPACE * cols;
			fcolor = WHITE; bcolor = BLUE;
			if(line == selline && cols == selcol)
			{
				// aktuell selektierte Zeile/Spalte
				fcolor = BLACK;
				bcolor = YELLOW;
				drawCalibVppHelp(line);
				draw_buttons_calib();
			}
			TFT_Fill_Rectangle(x+2, y+2, x + CALIB_XSPACE-3, y + CALIB_YSPACE-3, bcolor);
			float *val = NULL;
			val = getfValsPtr(val, line,cols);
			if(val != NULL)
			{
				sprintf(s,"%.0f",*val);
				tft_print(&lucidaConsole_22ptFontInfo, s, x+37, y+12, 3, fcolor , bcolor);
			}
		}
	}
}

// Bearbeitung eines Touches ins Grid
void calib_touch(int x, int y)
{
	// suche Feld
	for(int line = 1; line < CALIB_LINES; line++)
	{
		int ys = CALIB_YTOP + CALIB_YSPACE * line;
		int ye = ys + CALIB_YSPACE;
		for(int cols = 1; cols < CALIB_COLS; cols++)
		{
			int xs = CALIB_XLEFT + CALIB_XSPACE * cols;
			int xe = xs + CALIB_XSPACE;

			if(x>xs && x<xe && y>ys && y<ye)
			{
				// gefunden
				select_gridfield(line,cols);
			}
		}
	}
}

void select_gridfield(int line, int cols)
{
	selline = line;
	selcol = cols;

	draw_calib_buttons();
}

int do_button_calib(int touchbutton)
{
	if(touchbutton < TB_P1000) return 0;	// diese Buttons interessieren uns hier nicht

	tb_buttons[touchbutton].pressed = 1;

	if(touchbutton == TB_GETVAL)
	{
		draw_touchbutton(&tb_buttons[TB_GETVAL]);
		calib_readvalue();
	}
	else
	{
		if(selcol == 1)
		{
			if(touchbutton >= TB_P1000 && touchbutton <= TB_M1)
			{
				for(int i=TB_P1000; i<=TB_M1; i++)
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
						calib_changevalue(i);
					}
				}
			}
		}
	}

	return 1;
}

void calib_changevalue(int touchbutton)
{
float *fp = NULL;
int ival;

	fp = getfValsPtr(fp, selline,selcol);

	ival = (int)(*fp);

	if(touchbutton >= TB_P1 && touchbutton <= TB_M1000)
		setup_modified = 1;

	// stelle den Wert ein
	if(touchbutton == TB_P1) ival++;
	if(touchbutton == TB_P10) ival += 10;
	if(touchbutton == TB_P100) ival += 100;
	if(touchbutton == TB_P1000) ival += 1000;

	if(touchbutton == TB_M1) ival--;
	if(touchbutton == TB_M10) ival -= 10;
	if(touchbutton == TB_M100) ival -= 100;
	if(touchbutton == TB_M1000) ival -= 1000;

	// Begrenze auf max/min Werte
	if(selcol == 1)
	{
		if(ival < 1) ival = 1;
		if(ival > 10000) ival = 10000;
	}

	if(selcol == 2)
	{
		if(ival < 1) ival = 1;
		if(ival > 2499) ival = 2499;
	}

	*fp = (float)ival;

	draw_calib_buttons();
	setup_modified = 1;
}

// füge ADCvalue direkt ein
void calib_readvalue()
{
float *fp = NULL;

	// Pointer auf den Messwert der selektierten Zeile
	fp = getfValsPtr(fp, selline,2);

	switch (selline)
	{
	case 1: *fp = adc_rawvals[UFWDANT]; break;
	case 2: *fp = adc_rawvals[UFWDANT]; break;
	case 3: *fp = adc_rawvals[UFWDFLT]; break;
	case 4: *fp = adc_rawvals[UFWDFLT]; break;
	case 5: *fp = adc_rawvals[UFWDIN]; break;
	case 6: *fp = adc_rawvals[UFWDIN]; break;
	}

	draw_calib_buttons();
	setup_modified = 1;
}

// prüfe ob sinnvolle Werte drin stehen
void checkmV(float *low, float *high)
{
	if(*low >= *high)
	{
		*low = 1500;
		*high = 2500;
	}
}

uint8_t calibvalsOK()
{
	if(pcfgdata->ant_W_low >= pcfgdata->ant_W_high) return 0;
	if(pcfgdata->flt_W_low >= pcfgdata->flt_W_high) return 0;
	if(pcfgdata->drv_W_low >= pcfgdata->drv_W_high) return 0;

	checkmV(&pcfgdata->ant_mV_low,&pcfgdata->ant_mV_high);
	checkmV(&pcfgdata->flt_mV_low,&pcfgdata->flt_mV_high);
	checkmV(&pcfgdata->drv_mV_low,&pcfgdata->drv_mV_high);

	return 1;
}
