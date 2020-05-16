/*
 * ntc.c
 *
 *  Created on: 25.09.2017
 *      Author: kurt
 */

#include <main.h>

// NTC Funktionen, der NTC wird gegen Masse geschaltet

#define RV		8200   // Vorwid
#define VREF	2500

/* Umess (am ADC Eingang) = UV *R[ntc] / (RV + R[ntc])

damit ist der NTC Widerstand:
R[ntc] = Umess * RV / (UV - Umess)
*/

// Widerstandstabelle für den Conrad Sensor 500526
float temptab[] = {
	32650 , //    Ohm bei  0 Grad usw...
	25390 , //    5
	19900 , //    10
	15710 , //    15
	12490 , //    20
	10000 , //    25
	8057  , //    30
	6531  , //    35
	5327  , //    40
	4369  , //    45
	3603  , //    50
	2986  , //    55
	2488  , //    60
	2083  , //    65
	1752  , //    70
	1481  , //    75
	1258  , //    80
	1072  , //    85
	917.7   , //    90
	788.5   , //    95
	680.0   , //    100
	588.6   , //    105
	511.2   , //    110
	445.4   , //    115
	389.3   , //    120
	341.7   , //    125
	300.9   , //    130
	265.4   , //    135
	234.8   , //    140
	208.3   , //    145
	185.3   , //    150
	-1
};

// Berechne die Temperatur in Deg C
float cf_calc_temp(unsigned long uin)
{
	float Rntc;
	int i;
	float x;
	float Umess = (float)uin/1000.0;

	// Umess ist die Spannung am ADC Eingang
	// jetzt berechne daraus den Widerstand des NTCs
	Rntc = Umess * (float)RV / ((float)VREF/1000.0 - Umess);

	// suche den Bereich in der Tabelle
	i=0;
	while(temptab[i]!=-1)
	{
		if(temptab[i] <= Rntc) break;
		i++;
	}

	if(i==0)
	{
		return -10; // kleiner als kleinster Wert
	}

	if(temptab[i]!=-1)
	{
		// Widerstandsbereich gefunden, interpoliere
		x = i - (Rntc - temptab[i])/(temptab[i-1] - temptab[i]);

		// x ist jetzt der interpolierte Tabellenindex
		// rechne ihn in die Temperatur um
		return  x*5.0;
	}

	return 160; // größer als größter Wert
}

float calc_temp(unsigned long uin)
{
	float temp = cf_calc_temp(uin);

	// temp ist in Grad C
	if(pcfgdata->tempunits == 1)
	{
		// rechne in Grad F um
		temp = temp * 1.8 + 32.0;
	}

	return temp;
}
