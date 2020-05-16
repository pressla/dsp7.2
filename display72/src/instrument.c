/*
 * instrument.c
 *
 *  Created on: 03.10.2017
 *      Author: kurt
 */

#include <main.h>

void draw_scale();

int instr_x, instr_y;	// linke obere Ecke
int zgxf, zgyf; 	// Koordinaten des Drehpunkts des Zeigers
int zgxr, zgyr; 	// Koordinaten des Drehpunkts des Zeigers
int ofwd=-9999, orev=-9999;

#define I_DIAMETER	300
// Abstand Zeigerdrehpunkt bezogen auf linke obere Ecke
#define ZEIGERFWDXTOLEFT  	(400-77)
#define ZEIGERFWDYTOTOP		320
#define ZEIGERREVXTOLEFT  	77
#define ZEIGERREVYTOTOP		320
#define ZEIGERLEN			320

// x,y ist die linke obere Ecke
void setup_instrument(int x, int y)
{
	instr_x = x;
	instr_y = y;

	zgxf = x + ZEIGERFWDXTOLEFT;
	zgyf = y + ZEIGERFWDYTOTOP;
	zgxr = x + ZEIGERREVXTOLEFT;
	zgyr = y + ZEIGERREVYTOTOP;

	ofwd = orev = -9999;

	// Zeichne Scheibe
	TFT_DrawFlash("pmeter", x, y);

	// Zeichne Skala
	draw_scale();
}

void delete_zeiger()
{
	TFT_DeleteLastLine(7);
	TFT_DeleteLastLine(6);
	TFT_DeleteLastLine(5);
	TFT_DeleteLastLine(4);
	TFT_DeleteLastLine(3);
	TFT_DeleteLastLine(2);
	TFT_DeleteLastLine(1);
	TFT_DeleteLastLine(0);
}

float getPowerPhi(float pwr)
{
float phi;

	// Umrechnung des Powerwerts in den Winkel
	// erfolgt in 2 Bereichen 0-60 und 70-300W damit sich die Kurve optimal anschmiegt
	// die Berechnung erfolgt mit 0-300W, weshalb der Powerwert zunächst auf 300W normiert wird
	float pwr300 = 300 * (pwr / maxpwr[pcfgdata->maxpwrunits]);	// jetzt sind wir immer im Bereich 0-300W
	if(pwr300 < 65)
		phi = (0.64-log10(pwr300/15.0+1.0))*20.4/0.64+137.1;
	else
		phi = (0.778-log10(pwr300/60.0+1.0))*55.7/0.778+101.8;

	return phi;
}

float getReversePhi(float pwr)
{
float phi;

	// Umrechnung des Powerwerts in den Winkel
	// die Berechnung erfolgt mit 0-60W, weshalb der Powerwert zunächst auf 60W normiert wird
	// die REV Skala muss immer 1/5tel der FWD Skala sein damit die SWR Striche stimmen
	float pwr60 = 60 * (pwr / (maxpwr[pcfgdata->maxpwrunits] / 5));	// jetzt sind wir immer im Bereich 0-60W
	if(pwr60 < 20)
		phi = log10(pwr60/4+1.0)*(47.5-22.5)/0.677+22.5;
	else
		phi =log10(pwr60/15+1.0)*56.5/0.699+22.5;

	return phi;
}

void draw_forward(float pwr)
{
int xze,yze;
float phi;

	phi = getPowerPhi(pwr);

	TFT_PolarToKartesian(ZEIGERLEN, phi, &xze,&yze);
	TFT_DrawLine_aboveY(zgxf,zgyf,zgxf+xze,zgyf+yze, instr_y+275, BLUE, 1,0);
	TFT_DrawLine_aboveY(zgxf+1,zgyf,zgxf+xze+1,zgyf+yze, instr_y+275, BLUE, 1,1);
	TFT_DrawLine_aboveY(zgxf,zgyf+1,zgxf+xze,zgyf+yze+1, instr_y+275, BLUE, 1,2);
	TFT_DrawLine_aboveY(zgxf+1,zgyf+1,zgxf+xze+1,zgyf+yze+1, instr_y+275, BLUE, 1,3);
}

void draw_reverse(float pwr)
{
int xze,yze;
float phi;

	// die REV Skala hat genau 1/5 der Maximalpower von der Fwd-Skala
	phi = getReversePhi(pwr);

	TFT_PolarToKartesian(ZEIGERLEN, phi, &xze,&yze);
	TFT_DrawLine_aboveY(zgxr,zgyr,zgxr+xze,zgyr+yze, instr_y+275, BLUE, 1,4);
	TFT_DrawLine_aboveY(zgxr+1,zgyr,zgxr+xze+1,zgyr+yze, instr_y+275, BLUE, 1,5);
	TFT_DrawLine_aboveY(zgxr,zgyr+1,zgxr+xze,zgyr+yze+1, instr_y+275, BLUE, 1,6);
	TFT_DrawLine_aboveY(zgxr+1,zgyr+1,zgxr+xze+1,zgyr+yze+1, instr_y+275, BLUE, 1,7);
}

void draw_needle()
{
float v = pwrswr_antenna.fwd_watt;
float vr = pwrswr_antenna.rev_watt;

int ifwd = (int)v, irev = (int)(vr*10.0);

	if(abs(ifwd-ofwd)>2 || abs(irev-orev)>3)	// entflackern
	{
		delete_zeiger();
		draw_forward(v);
		draw_reverse(vr);
		ofwd = ifwd;
		orev = irev;
	}
}

void printFwdScaleValue(float pwr, int dx, int dy)
{
int x,y;
char s[20];

	TFT_PolarToKartesian(ZEIGERLEN+15, getPowerPhi(pwr), &x,&y);
	sprintf(s,"%d",(int)pwr);
	tft_print(&verdana_10ptFontInfo, s, zgxf+x+dx, zgyf+y+dy, 1, BLACK , WHITE);
}

void printRevScaleValue(float pwr, int dx, int dy)
{
int x,y;
char s[20];

	TFT_PolarToKartesian(ZEIGERLEN+10, getReversePhi(pwr), &x,&y);
	sprintf(s,"%d",(int)pwr);
	int w,h;
	tft_textwidth_pixel(&verdana_10ptFontInfo, s, &w, &h);
	tft_print(&verdana_10ptFontInfo, s, zgxr+x+dx-w, zgyr+y+dy, 1, BLACK , WHITE);
}

// Beschritung der Skalen
void draw_scale()
{

	// Maximalwerte
	float maxfwdpwr = maxpwr[pcfgdata->maxpwrunits];
	float maxrevpwr = maxfwdpwr / 5;	// die Rev Skala hat 1/5 des Maxwerts der Fwd Skala

	printFwdScaleValue(maxfwdpwr,10,20);
	printRevScaleValue(maxrevpwr,-20,20);

	if(pcfgdata->maxpwrunits == 0)
	{
		// 250 W Bereich
		printFwdScaleValue(10,0,0);
		printFwdScaleValue(20,0,0);
		printFwdScaleValue(50,-5,5);
		printFwdScaleValue(100,-5,5);

		printRevScaleValue(1,0,0);
		printRevScaleValue(2,0,0);
		printRevScaleValue(5,0,0);
		printRevScaleValue(10,0,0);
		printRevScaleValue(20,0,0);
	}

	if(pcfgdata->maxpwrunits == 1)
	{
		// 1000 W Bereich
		printFwdScaleValue(20,0,0);
		printFwdScaleValue(50,0,0);
		printFwdScaleValue(100,0,0);
		printFwdScaleValue(200,-3,3);
		printFwdScaleValue(350,-5,5);
		printFwdScaleValue(650,-5,5);

		printRevScaleValue(5,0,0);
		printRevScaleValue(10,0,0);
		printRevScaleValue(20,0,0);
		printRevScaleValue(50,0,0);
		printRevScaleValue(80,0,0);
	}

	if(pcfgdata->maxpwrunits == 2)
	{
		// 250 W Bereich
		printFwdScaleValue(100,0,0);
		printFwdScaleValue(200,0,0);
		printFwdScaleValue(500,-5,5);
		printFwdScaleValue(1000,-5,5);

		printRevScaleValue(10,0,0);
		printRevScaleValue(20,0,0);
		printRevScaleValue(50,0,0);
		printRevScaleValue(100,0,0);
		printRevScaleValue(200,0,0);
	}

	if(pcfgdata->maxpwrunits == 3)
	{
		// 1000 W Bereich
		printFwdScaleValue(200,0,0);
		printFwdScaleValue(500,0,0);
		printFwdScaleValue(1000,0,0);
		printFwdScaleValue(2000,-3,3);
		printFwdScaleValue(3500,-5,5);
		printFwdScaleValue(6500,-5,5);

		printRevScaleValue(50,0,0);
		printRevScaleValue(100,0,0);
		printRevScaleValue(200,0,0);
		printRevScaleValue(500,0,0);
		printRevScaleValue(800,0,0);
	}
}
