/*
 * history.c
 *
 *  Created on: 09.10.2017
 *      Author: kurt
 */

#include <main.h>

void hist_drawcoord();
void hist_hilfslinien();
char *hist_Ylabel(int v);
void hist_drawvalues(uint16_t *arr, uint16_t rgb);

// die Screen Koordinaten den Datenfläche
// die Beschriftungen gehen darüber hinaus
#define hist_left 60
#define hist_top 40
#define hist_width  500
#define hist_height 300

// Wertebereich der Datenfläche
float hist_maxpwr = 2500;
// in X-Richtung ist jedes Pixel 1 Sekunde

// Array für History Daten
uint16_t antpwr_arr[hist_width];
uint16_t antpeakpwr_arr[hist_width];
uint16_t dcpwr_arr[hist_width];

int xspeed = 123;	// ms pro Pixel, wird am Anfang durch einen simulierten Tastendruck eingestellt

#define HIST_MAINLINEWIDTH 4

void hist_draw_frame()
{
	hist_maxpwr = getMaxPower();

	hist_drawcoord();
}

int hist_ValueToScreen(float val)
{
	// Skalieren auf Screen Koordinaten
	int ival = lin_log(val, hist_maxpwr, hist_height);
	return hist_top + hist_height - ival;
}

char *makeTimeString(int32_t ms)
{
static char s[10];

	int sec = -ms/1000L;	// Sekunden

	if(sec < 60)
		sprintf(s,"%ds",sec);
	else
	{
		int min = sec/60;
		int restsec = sec - (min*60);
		if(restsec)
			sprintf(s,"%dm%d",min,restsec);
		else
			sprintf(s,"%dm",min);
	}

	return s;
}

void hist_drawcoord()
{
int ydiff = 8;	// Verschiebung für Label Text

	// Lösche Diagrammfläche
	TFT_Fill_Rectangle(0, 0, TFT_WIDTH-151, 369, BLACK);

	// Titel
	tft_print(&lucidaConsole_22ptFontInfo, getLang(HISTORY), 150, 1, 4, WHITE , BLACK);

	// Hauptachsen
	TFT_Hor_Line(hist_left-5-HIST_MAINLINEWIDTH, hist_left+hist_width, hist_top+hist_height, WHITE, BLACK, HIST_MAINLINEWIDTH,0);
	TFT_Vert_Line(hist_left-HIST_MAINLINEWIDTH, hist_top-5, hist_top+hist_height+5+HIST_MAINLINEWIDTH, WHITE, BLACK, HIST_MAINLINEWIDTH,0);

	// Hauptachsen Beschriftung
	tft_print(&lucidaConsole_14ptFontInfo, "W"/*getLang(W_RFout)*/, 0, hist_top-20, 2, RED , BLACK);
	tft_print(&lucidaConsole_14ptFontInfo, getLang(W_RFoutPeak), 30, hist_top-20, 2, BLUE , BLACK);
	tft_print(&lucidaConsole_14ptFontInfo, "W [DC]", hist_left + hist_width, hist_top-20, 2, GREEN , BLACK);
	tft_print(&lucidaConsole_14ptFontInfo, "t", hist_left + hist_width + 10, hist_top+hist_height - 8, 2, WHITE , BLACK);

	// Hilfslinien horizontal und vertikal
	hist_hilfslinien();

	// Beschriftung der horizontalen Hilfslinien
	for(int i=0; i<5; i++)
	{
		int pwr = h_power[pcfgdata->maxpwrunits][i];
		// HF Power
		tft_print(&lucidaConsole_12ptFontInfo, hist_Ylabel(pwr), 0, hist_ValueToScreen(pwr)-ydiff, 1, WHITE , BLACK);
		// DC Power (doppelte Größe)
		tft_print(&lucidaConsole_12ptFontInfo, hist_Ylabel(pwr*2), hist_left + hist_width + 8, hist_ValueToScreen(pwr)-ydiff, 1, WHITE , BLACK);
	}

	// Beschriftung der vertikalen Hilfslinien
	int32_t millisecond = 0;
	for(int i=0; i<hist_width; i++)
	{
		if((i%60) == 0 && i != 0)
		{
			char *hp = makeTimeString(millisecond);
			int w,h;
			tft_textwidth_pixel(&lucidaConsole_12ptFontInfo, hp, &w, &h);
			tft_print(&lucidaConsole_12ptFontInfo, hp, hist_left + i - w/2, hist_top + hist_height+7, 1, WHITE , BLACK);
		}
		millisecond -= xspeed;
	}

	if(xspeed == 123)
		do_button_history(TB_HIST1000);
}

static char hist_ylab[30] = {0};

char *hist_Ylabel(int v)
{

	if(v<1000)
		sprintf(hist_ylab,"%d",v);
	else
	{
		int vi = (v/1000.0);
		int rest = v - vi*1000;
		sprintf(hist_ylab,"%dk%d",vi,rest);
		for(int i=strlen(hist_ylab)-1; i>0; i--)
		{
			if(hist_ylab[i] == 0 || hist_ylab[i] == 'k') break;
			if(hist_ylab[i] == '0') hist_ylab[i] = 0;
		}
	}

	return hist_ylab;
}

// wird laufend aufgerufen zur Wiederherstellung, daher hier keinen Text ausdrucken, nur das Gitter
void hist_hilfslinien()
{
	// Hilfslinien Horizontal
	for(int i=0; i<5; i++)
	{
		int pwr = h_power[pcfgdata->maxpwrunits][i];
		TFT_Hor_Line(hist_left, hist_left+hist_width, hist_ValueToScreen(pwr), RGB16(0x10,0x20,0x20), BLACK, 1, 0);
	}

	// Hilfslinien vertikal, alle 60 Sekunden
	for(int i=0; i<hist_width; i++)
	{
		if((i%60) == 0 && i != 0)
			TFT_Vert_Line(hist_left+i, hist_top-4, hist_top+hist_height, RGB16(0x10,0x20,0x20), BLACK, 2, 0);
	}
}
/*
 * diese Funktion wird mit voller Geschwindigkeit aufgerufen,
 * die Darstellung erfolgt aber nur 1x pro Sekunde
 * dazwischen mittle die Werte, und zwar alle 10ms, also max. 100 Werte
 */
float antpwr_mittel = 0;
float antpwr_peak = 0;
float dcpwr_mittel = 0;
int mittelcnt = 0;

float htest = 0;
float htest1 = 0;

void hist_insertdata()
{
static char f = 1;

	if(f)
	{
		for(int i=0; i<hist_width; i++)
		{
			dcpwr_arr[i] = antpwr_arr[i] = antpeakpwr_arr[i] = hist_ValueToScreen(0);
		}

		f=0;
	}

	if(hist_10ms == 0)
	{
		// 10ms sind abgelaufen, füge den aktuellen Messwert zum Mittel hinzu
		if(pwrswr_antenna.fwd_watt < hist_maxpwr)
			antpwr_mittel += pwrswr_antenna.fwd_watt;
		else
			antpwr_mittel += hist_maxpwr;

		if(pwrswr_antenna.fwd_peakwatt > antpwr_peak)
			antpwr_peak = pwrswr_antenna.fwd_peakwatt;

		dcpwr_mittel += adc_realvals[CALC_POWER];


		mittelcnt++;
		hist_10ms = 10;
	}

	show_histdata();
}

void show_histdata()
{
	if(hist_1s == 0)
	{
		// xspeed s sind abgelaufen, berechne Wert

		if(menunum == MENU_HISTORY)
		{
			// Lösche aktuelle Linie
			hist_drawvalues(antpwr_arr, BLACK);
			hist_drawvalues(antpeakpwr_arr, BLACK);
			hist_drawvalues(dcpwr_arr, BLACK);

			// restauriere Hilfslinien
			hist_hilfslinien();
		}

		// schiebe Array
		memmove(antpwr_arr+1,antpwr_arr,(hist_width-1)*sizeof(uint16_t));
		memmove(antpeakpwr_arr+1,antpeakpwr_arr,(hist_width-1)*sizeof(uint16_t));
		memmove(dcpwr_arr+1,dcpwr_arr,(hist_width-1)*sizeof(uint16_t));

		// füge neuen Wert ein
		antpwr_arr[0] = hist_ValueToScreen(antpwr_mittel / (float)mittelcnt);
		antpeakpwr_arr[0] = hist_ValueToScreen(antpwr_peak);
		dcpwr_arr[0] = hist_ValueToScreen(dcpwr_mittel / (float)mittelcnt / 2.0);  // durch 2.0 weil die Skala doppelt so groß skaliert ist wie die RF Skala

		if(menunum == MENU_HISTORY)
		{
			// Zeichne neue Werte
			hist_drawvalues(dcpwr_arr, GREEN);
			hist_drawvalues(antpwr_arr, RED);
			hist_drawvalues(antpeakpwr_arr, BLUE);
		}

		antpwr_mittel = 0;
		antpwr_peak = 0;
		dcpwr_mittel = 0;
		mittelcnt = 0;
		hist_1s = xspeed;
	}
}

void hist_drawvalues(uint16_t *arr, uint16_t rgb)
{
uint16_t ys,ye;
int linewidth = 2;

	ys = *arr++;
	for(int x=1; x<hist_width; x++)
	{
		ye = *arr++;
		// das die X-Schrittweite 1 px is, reicht die VerticalLine
		if(ys == ye)
		{
			// keine Änderung der Höhe, es reicht das Pixelsetzen
			TFT_Fill_Rectangle(hist_left+x, ys, hist_left+x+linewidth-1, ye+linewidth-1, rgb);
		}
		else
		{
			TFT_Vert_Line(hist_left+x, ys, ye, rgb, BLACK, linewidth, 0);
			ys = ye;
		}
	}
}

int do_button_history(int touchbutton)
{
	if(touchbutton >= TB_HIST100 && touchbutton <= TB_HIST1000)
	{
		tb_buttons[TB_HIST100].pressed = 0;
		tb_buttons[TB_HIST250].pressed = 0;
		tb_buttons[TB_HIST500].pressed = 0;
		tb_buttons[TB_HIST1000].pressed = 0;

		tb_buttons[touchbutton].pressed = 1;
		draw_touchbutton(&tb_buttons[TB_HIST100]);
		draw_touchbutton(&tb_buttons[TB_HIST250]);
		draw_touchbutton(&tb_buttons[TB_HIST500]);
		draw_touchbutton(&tb_buttons[TB_HIST1000]);

		int newxspeed = 100;
		if(touchbutton == TB_HIST250) newxspeed = 250;
		if(touchbutton == TB_HIST500) newxspeed = 500;
		if(touchbutton == TB_HIST1000) newxspeed = 1000;

		if(newxspeed != xspeed)
		{
			xspeed = newxspeed;
			hist_drawcoord();
		}

		return 1;
	}

	return 0;
}
