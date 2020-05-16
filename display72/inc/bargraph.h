/*
 * bargraph.h
 *
 *  Created on: 22.09.2017
 *      Author: kurt
 */

#ifndef BARGRAPH_H_
#define BARGRAPH_H_

#include <font.h>

enum _BARSTYLE {
	BARSTYLE_BLOCK = 1,
	BARSTYLE_FULL  = 2,
	BARSTYLE_LIN   = 4,
	BARSTYLE_LOG   = 8,
	BARSTYLE_SWR   = 16
};

typedef struct _BARLABEL {
	int posx;		// in%
	int leftshift;
	char text[10];
} t_barlabel;

typedef struct _BARGRAPH {
	int minx,maxx;
	int miny,maxy;
	FONT_INFO *titlefont;
	int titlex, titley;
	char titletext[30];	// oder NULL wenn kein Titel
	int style;			// eines von _BARSTYLE
	// X-Beschriftung
	char labeltext[10];
	FONT_INFO *labelfont;
	t_barlabel barlabel[10];
	int labelanz;
	// Wertangabe rechts
	FONT_INFO *valuefont;
	int valuexdiff;
	int valueydiff;
	// folgende Angaben bezogen auf den realen Messwert
	float yellow;			// ab hier gelb (links davon grün)
	float red;				// ab hier rot
	float val100precent; 	// Maßstab, dieser Wert gibt 100% an (0 ist Null)
	// interne Fixwerte
	int greenmaxx;
	int yellowmaxx;
	float last_value;
	float last_peakvalue;
} t_bargraph;

void show_bar(t_bargraph *barinfo);
void show_bridge_bars();
void fill_bar(t_bargraph *bi, float value, float peakval);

#endif /* BARGRAPH_H_ */
