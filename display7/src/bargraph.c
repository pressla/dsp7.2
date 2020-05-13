/*
 * bargraph.c
 *
 *  Created on: 22.09.2017
 *      Author: kurt
 */

#include <main.h>
#include <math.h>

void show_bar(t_bargraph *bi)
{
	int pvalyellow = 0;
	int pvalred = 0;
	int ylabeldiff = 8;

	// Berechne diverse feste Referenzwerte
	if(bi->style & BARSTYLE_LOG)
	{
		if(bi->style & BARSTYLE_SWR)
		{
			// log Swr
			pvalyellow = lin_log_percent_swr(bi->yellow, bi->val100precent);
			pvalred = lin_log_percent_swr(bi->red, bi->val100precent);
		}
		else
		{
			// log Pwr
			pvalyellow = log_percent(bi->yellow, bi->val100precent);
			pvalred = log_percent(bi->red, bi->val100precent);
		}
	}
	else
	{
		// lin
		pvalyellow = (int)(bi->yellow * 100.0 / bi->val100precent);
		pvalred = (int)(bi->red * 100.0 / bi->val100precent);
	}

	bi->greenmaxx = bi->minx + (pvalyellow * (bi->maxx - bi->minx)) / 100;
	bi->yellowmaxx = bi->minx + (pvalred * (bi->maxx - bi->minx)) / 100;

	// zeichne den Rahmen
	TFT_Rectangle(bi->minx-1, bi->miny-1, bi->maxx+1, bi->maxy+1, LIGHTGREY, 2);

	// Titel
	if(bi->titlefont == &verdana_22ptFontInfo)
		tft_print(bi->titlefont, bi->titletext, bi->titlex, bi->titley, 4, WHITE, BLACK);
	else if(bi->titlefont == &verdana_18ptFontInfo)
		tft_print(bi->titlefont, bi->titletext, bi->titlex, bi->titley, 3, WHITE, BLACK);
	else
		tft_print(bi->titlefont, bi->titletext, bi->titlex, bi->titley, 1, WHITE, BLACK);

	// Einheit
	tft_print(bi->labelfont, bi->labeltext, bi->maxx+5, bi->maxy+ylabeldiff, 1, BARLABELBLUE, BLACK);

	// Label
	for(int i=0; i<bi->labelanz; i++)
	{
		int x = bi->minx + bi->barlabel[i].posx*(bi->maxx-bi->minx)/100;
		tft_print(bi->labelfont, bi->barlabel[i].text, x + bi->barlabel[i].leftshift, bi->maxy+ylabeldiff, 1, BARLABELBLUE, BLACK);

		TFT_Vert_Line(x, bi->maxy+2, bi->maxy+5, BARLABELBLUE, BLACK, 4, 0);
	}
}

void draw_bar_rect(int xs, int ys, int xe, int ye, unsigned int rgb, int style, int maxx)
{
//int blockwidth = 8;
//int blockspace = 4;

	// nur volle Farben, ist evt schöner als Blocks
	TFT_Fill_Rectangle(xs,ys,xe,ye,rgb);
/*
	if(style & BARSTYLE_FULL)
	{
		TFT_Fill_Rectangle(xs,ys,xe,ye,rgb);
	}
	else
	{
		for(int i=xs; i<xe; i+=(blockwidth+blockspace))
		{
			if((i+blockwidth) > maxx)
				TFT_Fill_Rectangle(i,ys,maxx,ye,rgb);
			else
				TFT_Fill_Rectangle(i,ys,i+blockwidth,ye,rgb);

			TFT_Fill_Rectangle(i+blockwidth,ys,i+blockwidth+blockspace,ye,BLACK);
		}
	}
	*/
}

// Zeichne den Balken mit Länge "value"
// wobei value der echte Messwert ist
// der hier mithilfe des Maximalwerts skaliert wird
void fill_bar(t_bargraph *bi, float value, float peakval)
{
	int pval = 0;
	int pkval = 0;

	if(bi->last_value == value && bi->last_peakvalue == peakval) return;

	// rechne den Messwert in % um
	if(bi->style & BARSTYLE_LOG)
	{
		if(bi->style & BARSTYLE_SWR)
			pval = lin_log_percent_swr(value, bi->val100precent);	// log mit SWR Skalierung
		else
		{
			pval = log_percent(value, bi->val100precent);			// log Power Skalierung
			pkval = log_percent(peakval, bi->val100precent);
		}
	}
	else
		pval = (int)(value * 100.0 / bi->val100precent);	// linear

	// Skaliere auf Balkengröße
	int xsize = bi->maxx - bi->minx;
	int xvalue = bi->minx + (pval * xsize) / 100;	// der Balken geht von 0 bis hierher
	int xpkvalue = bi->minx + (pkval * xsize) / 100;

	// begrenze Maximalwert
	if(xvalue > (bi->maxx-1))
		xvalue = (bi->maxx-1);

	// Zeichne Balken

	if(xvalue > (bi->minx+1))
	{
		if(bi->last_value != value)
		{
			if(value <= bi->yellow)
				draw_bar_rect(bi->minx+1, bi->miny+1, xvalue, bi->maxy-1, GREEN, bi->style, bi->maxx-3);
			else if(value <= bi->red)
			{
				draw_bar_rect(bi->minx+1, bi->miny+1, bi->greenmaxx, bi->maxy-1, GREEN, bi->style, bi->maxx-3);
				draw_bar_rect(bi->greenmaxx+1, bi->miny+1, xvalue, bi->maxy-1, YELLOW, bi->style, bi->maxx-3);
			}
			else
			{
				draw_bar_rect(bi->minx+1, bi->miny+1, bi->greenmaxx, bi->maxy-1, GREEN, bi->style, bi->maxx-3);
				draw_bar_rect(bi->greenmaxx+1, bi->miny+1, bi->yellowmaxx, bi->maxy-1, YELLOW, bi->style, bi->maxx-3);
				draw_bar_rect(bi->yellowmaxx+1, bi->miny+1, xvalue, bi->maxy-1, RED, bi->style, bi->maxx-3);
			}

			// und lösche den Rest
			if((bi->maxx-1) > xvalue)
				TFT_Fill_Rectangle(xvalue, bi->miny+1, bi->maxx-1, bi->maxy-1, BLACK);
		}

		// Peak
		if(peakval > 0 && xpkvalue > 5 && xpkvalue < (bi->maxx-8))
		{
			TFT_Fill_Rectangle(xpkvalue-5, bi->miny+1, xpkvalue, bi->maxy-1, BLUE);
		}
	}
	else
	{
		// lösche Balken
		TFT_Fill_Rectangle(bi->minx+1, bi->miny+1, bi->maxx-1, bi->maxy-1, BLACK);
	}

	// Value
	char s[20];
	if(peakval < 0) peakval = value;
	if(value >= 0)
	{
		if(bi == &bar_big_swr || bi == &bar_out_swr)
			sprintf(s,"%7.2f",peakval);
		else
			sprintf(s,"%6.1f",peakval);
	}
	else
		sprintf(s," ---  ");

	tft_print(bi->valuefont, s, bi->maxx + bi->valuexdiff+2, bi->miny+bi->valueydiff, 1, WHITE, BLACK);

	bi->last_value = value;
	bi->last_peakvalue = peakval;
}
