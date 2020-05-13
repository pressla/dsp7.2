/*
 * messagebox.c
 *
 *  Created on: 27.09.2017
 *      Author: kurt
 */

#include <main.h>

void messagebox(char *text, int duration_ms)
{
	FONT_INFO font = verdana_36ptFontInfo;

	int w,h;
	tft_textwidth_pixel(&font, text, &w, &h);

	w += 100;

	int xs = TFT_WIDTH/2 - w/2;
	int xe = TFT_WIDTH/2 + w/2;
	int ys = TFT_HEIGHT/2 - h/2;
	int ye = TFT_HEIGHT/2 + h/2;

	for(int i=30; i>1; i-=4)
	{
		TFT_Fill_Rectangle(xs-i, ys-i, xe+i, ye+i, WHITE);
		TFT_Fill_Rectangle(xs-i+2, ys-i+2, xe+i-2, ye+i-2, BLACK);
	}

	tft_print(&font, text, xs+20, ys + ((ye-ys)/2 - h/2), 4, WHITE, BLACK);

	delay_1ms(duration_ms);
}
