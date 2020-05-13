/*
 * powerlog.c
 *
 *  Created on: 09.10.2017
 *      Author: kurt
 */

#include <main.h>

// Funktionen zur logarithmischen Aufteilung einer Pwr-Anzeige

// Aufteilung der Powerbereiche
// Vollausschlag:
int maxpwr[4] = {250,1000,2500,10000};
int maxdrvpwr[4] = {6,15,30,50};

// erster Index: pcfgdata->maxpwrunits
// zweiter Index: 0-4 sind gleichmäßig aufgeteilte Stufen zur Displaybeschriftung
int h_power[8][5] =
{
		{10, 25, 50, 100, 200},
		{25, 100, 200, 400, 750},
		{50, 150, 400, 1000, 2000},
		{100, 400, 1000, 3000, 7500},

		// und hier für die Drive Power
		{1,2,3,4,5}, // 6
		{1,2,4,7,10}, // 15
		{1,5,10,15,25}, // 30
		{1,5,10,20,35} // 50
};

float h_swr[5] = {1.2, 1.5, 2, 3, 5};

float maxUunits[4] = {50,100,1000,4000};
float maxIunits[4] = {10,50,100,200};

// logaritmiert einen Power-Wert und skaliert ihn
// Messwertbereich: maxval
// Ergebnisbereich: maxres
int lin_log_raw(float value, float maxval, int maxres, float abflachung)
{
	float fp =log10(value/abflachung + 1);
	fp = fp * (float)maxres / (log10(maxval/abflachung + 1));
	return (int)fp;
}

int lin_log(float value, float maxval, int maxres)
{
	float fp = lin_log_raw(value, maxval, maxres, 40);	// Abflachung 40 für Power Darstellung
	return (int)fp;
}

int lin_log_percent_swr(float value, float maxval)
{
	// value-1 weil ganz links 1:1 ist (und nicht 0)
	float fp = lin_log_raw(value-1, maxval, 100, 0.1);	// Abflachung für SWR Darstellung
	return (int)fp;
}

// logarithmiert einen Wert und rechnet in % um 0-100
int log_percent(float value, float maxval)
{
	return lin_log(value, maxval, 100);	// skaliert in den Bereich 0 - 100 %
}

int getMaxPower()
{
	return maxpwr[pcfgdata->maxpwrunits];
}

int getMaxDrvPower()
{
	return maxdrvpwr[pcfgdata->maxdrvpwrunits];
}

int CelsiusToFahrenheit(int c)
{
	float f =  (float)c * 1.8 + 32.0;

	return (int)f;
}
