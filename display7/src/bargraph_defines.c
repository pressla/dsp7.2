/*
 * bargraph_defines.c
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#include <main.h>

void bar_initstruct(t_bargraph *bar);
void bar_setxy(t_bargraph *bar, int leftx, int starty, int width, int height);
void bar_title(t_bargraph *bar, FONT_INFO *pfi, int xdiff, int ydiff, char *text);
void bar_style(t_bargraph *bar, int style);
void bar_labelunit(t_bargraph *bar, FONT_INFO *pfi, char *text);
void bar_labelentry(t_bargraph *bar, int x, int shift, char *text);
void bar_value(t_bargraph *bar, FONT_INFO *pfi);
void bar_barinfo(t_bargraph *bar, float vyellow, float vred, float v100);

// Balken für das Übersichts Balkenfenster
t_bargraph bar_out_power;
t_bargraph bar_out_swr;
t_bargraph bar_in_power;
t_bargraph bar_voltage;
t_bargraph bar_current;
t_bargraph bar_dc_power;
t_bargraph bar_efficiency;
t_bargraph bar_temperature1;
t_bargraph bar_temperature2;

// Balken für die großen Balken
t_bargraph bar_big_power;
t_bargraph bar_big_swr;

// Balken für Bridge Menü
t_bargraph bar_bg_antfwd;
t_bargraph bar_bg_antrev;
t_bargraph bar_bg_fltfwd;
t_bargraph bar_bg_fltrev;
t_bargraph bar_bg_drvfwd;
t_bargraph bar_bg_drvrev;

void makeLabelEntries_Power(t_bargraph *bg, FONT_INFO *pfi, float maxpwr, int unitidx)
{
	for(int i=0; i<5; i++)
	{
		int pwr = h_power[unitidx][i];// fünf Zwischenschritte
		int percent = log_percent(pwr,maxpwr);		// Zwischenschritte in Prozent (Vollausschlag = 100)
		char s[10];
		//sprintf(s,"%d",pwr);
		if(pwr >= 1000)
		{
			sprintf(s,"%dk%d",pwr/1000,pwr - (pwr/1000)*1000);
			for(int i=strlen(s)-1; i>0; i--)
				if(s[i] == '0') s[i] = 0;
				else break;
		}
		else
			sprintf(s,"%d",pwr);
		int w,h;
		tft_textwidth_pixel(pfi, s, &w, &h);
		bar_labelentry(bg, percent, -w/2, s);
	}
}

void makeLabelEntries_SWR(t_bargraph *bg, FONT_INFO *pfi)
{
	int maxswr = 20;								// SWR bei Vollausschlag
	bg->labelanz = 0;
	for(int i=0; i<5; i++)
	{
		float swr = h_swr[i];// fünf Zwischenschritte
		int percent = lin_log_percent_swr(swr,maxswr);		// Zwischenschritte in Prozent (Vollausschlag = 100)
		char s[10];
		if(swr == (float)((int)swr))
			sprintf(s,"%d",(int)swr);
		else
			sprintf(s,"%.1f",swr);
		int w,h;
		tft_textwidth_pixel(pfi, s, &w, &h);
		bar_labelentry(bg, percent, -w/2, s);
	}

}

void makeLabelEntries_linear(t_bargraph *bg, FONT_INFO *pfi, int maxval)
{
	bg->labelanz = 0;
	for(int i=0; i<5; i++)
	{
		int val = (i * maxval) / 5;
		char s[10];
		if(val >= 1000)
		{
			sprintf(s,"%dk%d",val/1000,val - (val/1000)*1000);
			for(int i=strlen(s)-1; i>0; i--)
				if(s[i] == '0') s[i] = 0;
				else break;
		}
		else
			sprintf(s,"%d",val);
		int w,h;
		tft_textwidth_pixel(pfi, s, &w, &h);
		int percent = (100 * val) / maxval;
		bar_labelentry(bg, percent, -w/2, s);
	}

}

void init_bars()
{
int leftx = 10;		// linke Spalte von Bars
int rightx = 350;	// rechte Spalte von Bars
int width = 200;	// Breite der Bars
int height = 18;	// Höhe der Bars
int starty = 50;	// ersten Y Zeile des obersten Bars
int diffy = 60;		// Abstand zwischen den Bars
int textyshift = -27;// Titeltext Abstand über Balken
int textxshift = 0; // Titeltext Abstand links

int barynum = 0;
char s[20];

	// ========== Ubersichtsfenster =============
	// linke Spalte
	// Output Power
	bar_initstruct(&bar_out_power);
	bar_setxy(&bar_out_power,leftx,starty + barynum++*(diffy+height), 540, height);
	bar_title(&bar_out_power, &verdana_18ptFontInfo, textxshift, textyshift, getLang(Output_Power));
	bar_style(&bar_out_power, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_out_power, &verdana_12ptFontInfo, getLang(W_RFout));
	int maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_out_power, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_out_power, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_out_power, maxpwr/2, (maxpwr*2)/3, maxpwr);

	// Output SWR
	bar_initstruct(&bar_out_swr);
	bar_setxy(&bar_out_swr,leftx,starty + barynum++*(diffy+height),width, height);
	bar_title(&bar_out_swr, &verdana_18ptFontInfo, textxshift, textyshift,getLang(Output_SWR));
	bar_style(&bar_out_swr, BARSTYLE_FULL | BARSTYLE_LOG | BARSTYLE_SWR);
	bar_labelunit(&bar_out_swr, &verdana_12ptFontInfo, ": 1");
	makeLabelEntries_SWR(&bar_out_swr, &verdana_12ptFontInfo);
	bar_value(&bar_out_swr, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_out_swr, 2, 3, 20);

	// Input Power
	bar_initstruct(&bar_in_power);
	bar_setxy(&bar_in_power,leftx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_in_power, &verdana_18ptFontInfo, textxshift, textyshift, getLang(Drive_Power));
	bar_style(&bar_in_power, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_in_power, &verdana_12ptFontInfo, getLang(W_RFin));
	maxpwr = getMaxDrvPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_in_power, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxdrvpwrunits+4);
	bar_value(&bar_in_power, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_in_power, maxpwr/2, (maxpwr*2)/3, maxpwr);

	// Efficiency
	bar_initstruct(&bar_efficiency);
	bar_setxy(&bar_efficiency,leftx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_efficiency, &verdana_18ptFontInfo, textxshift, textyshift, getLang(Efficiency));
	bar_style(&bar_efficiency, BARSTYLE_FULL);
	bar_labelunit(&bar_efficiency, &verdana_12ptFontInfo, "%");
	makeLabelEntries_linear(&bar_efficiency, &verdana_12ptFontInfo, 100);
	bar_value(&bar_efficiency, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_efficiency, 100, 100, 100);

	// Temperatur-1
	bar_initstruct(&bar_temperature1);
	bar_setxy(&bar_temperature1,leftx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_temperature1, &verdana_18ptFontInfo, textxshift, textyshift, getLang(Temperature1));
	bar_style(&bar_temperature1, BARSTYLE_FULL);
	bar_value(&bar_temperature1, &lucidaConsole_18ptFontInfo);
	strcpy(s,getLang(Degree));
	strcat(s," ");
	if(pcfgdata->tempunits == 0)
	{
		strcat(s,"[C]");
		makeLabelEntries_linear(&bar_temperature1, &verdana_12ptFontInfo, 125);
		bar_barinfo(&bar_temperature1, 50, 70, 125);
	}
	else
	{
		strcat(s,"[F]");
		makeLabelEntries_linear(&bar_temperature1, &verdana_12ptFontInfo, 250);
		bar_barinfo(&bar_temperature1, 120, 150, 250);
	}
	bar_labelunit(&bar_temperature1, &verdana_12ptFontInfo, s);

	// ==== rechte Spalte ====
	barynum = 1;
	// Voltage
	bar_initstruct(&bar_voltage);
	bar_setxy(&bar_voltage,rightx,starty + barynum++*(diffy+height),width, height);
	bar_title(&bar_voltage, &verdana_18ptFontInfo, textxshift, textyshift, getLang(PS_Voltage));
	bar_style(&bar_voltage, BARSTYLE_FULL);
	bar_labelunit(&bar_voltage, &verdana_12ptFontInfo, "V [DC]");
	makeLabelEntries_linear(&bar_voltage, &verdana_12ptFontInfo, maxUunits[pcfgdata->maxUunits]);
	bar_value(&bar_voltage, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_voltage, (maxUunits[pcfgdata->maxUunits]*2)/3, (maxUunits[pcfgdata->maxUunits]*3)/4, maxUunits[pcfgdata->maxUunits]);

	// Current
	bar_initstruct(&bar_current);
	bar_setxy(&bar_current,rightx,starty + barynum++*(diffy+height),width, height);
	bar_title(&bar_current, &verdana_18ptFontInfo, textxshift, textyshift, getLang(PS_Current));
	bar_style(&bar_current, BARSTYLE_FULL);
	bar_labelunit(&bar_current, &verdana_12ptFontInfo, "A [DC]");
	makeLabelEntries_linear(&bar_current, &verdana_12ptFontInfo, maxIunits[pcfgdata->maxIunits]);
	bar_value(&bar_current, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_current, (maxIunits[pcfgdata->maxIunits]*2)/3, (maxIunits[pcfgdata->maxIunits]*3)/4, maxIunits[pcfgdata->maxIunits]);

	// DC Power
	bar_initstruct(&bar_dc_power);
	bar_setxy(&bar_dc_power,rightx,starty + barynum++*(diffy+height),width, height);
	bar_title(&bar_dc_power, &verdana_18ptFontInfo, textxshift, textyshift, getLang(PS_Power));
	bar_style(&bar_dc_power, BARSTYLE_FULL);
	bar_labelunit(&bar_dc_power, &verdana_12ptFontInfo, "W [DC]");
	int maxdcpwr = h_power[pcfgdata->maxpwrunits][4] * 2;
	makeLabelEntries_linear(&bar_dc_power, &verdana_12ptFontInfo, maxdcpwr);
	bar_value(&bar_dc_power, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_dc_power, (maxdcpwr*2)/3, (maxdcpwr*3)/4, maxdcpwr);


	// Temperatur-2
	bar_initstruct(&bar_temperature2);
	bar_setxy(&bar_temperature2,rightx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_temperature2, &verdana_18ptFontInfo, textxshift, textyshift, getLang(Temperature2));
	bar_style(&bar_temperature2, BARSTYLE_FULL);
	bar_value(&bar_temperature2, &lucidaConsole_18ptFontInfo);
	strcpy(s,getLang(Degree));
	strcat(s," ");
	if(pcfgdata->tempunits == 0)
	{
		strcat(s,"[C]");
		makeLabelEntries_linear(&bar_temperature2, &verdana_12ptFontInfo, 125);
		bar_barinfo(&bar_temperature2, 50, 70, 125);
	}
	else
	{
		strcat(s,"[F]");
		makeLabelEntries_linear(&bar_temperature2, &verdana_12ptFontInfo, 250);
		bar_barinfo(&bar_temperature2, 120, 150, 250);
	}
	bar_labelunit(&bar_temperature2, &verdana_12ptFontInfo, s);

	// ========== grosses Balkenfenster =============
	leftx = 50;
	starty = 50;
	diffy = 80;
	height = 50;
	textyshift = -35;
	barynum = 0;

	// Output Power
	bar_initstruct(&bar_big_power);
	bar_setxy(&bar_big_power,leftx,starty + barynum++*(diffy+height), 450, height);
	bar_title(&bar_big_power, &verdana_22ptFontInfo, textxshift, textyshift, getLang(Output_Power));
	bar_style(&bar_big_power, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_big_power, &lucidaConsole_14ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();
	makeLabelEntries_Power(&bar_big_power, &lucidaConsole_14ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_big_power, &lucidaConsole_22ptFontInfo);
	bar_barinfo(&bar_big_power, maxpwr/2, (maxpwr*2)/3, maxpwr);

	// Output SWR
	bar_initstruct(&bar_big_swr);
	bar_setxy(&bar_big_swr,leftx,starty + barynum++*(diffy+height), 450, height);
	bar_title(&bar_big_swr, &verdana_22ptFontInfo, textxshift, textyshift, getLang(Output_SWR));
	bar_style(&bar_big_swr, BARSTYLE_FULL | BARSTYLE_LOG | BARSTYLE_SWR);
	bar_labelunit(&bar_big_swr, &lucidaConsole_14ptFontInfo, ": 1");
	makeLabelEntries_SWR(&bar_big_swr, &lucidaConsole_14ptFontInfo);
	bar_value(&bar_big_swr, &lucidaConsole_22ptFontInfo);
	bar_barinfo(&bar_big_swr, 2, 3, 20);

	// ==================== Bridges Fenster ==============================
	// Output Power
	leftx = 250;
	width = 300;	// Breite der Bars
	height = 12;	// Höhe der Bars
	starty = 25;	// ersten Y Zeile des obersten Bars
	diffy = 55;		// Abstand zwischen den Bars
	textyshift = -20;// Titeltext Abstand über Balken
	textxshift = 0; // Titeltext Abstand links

	barynum = 0;

	bar_initstruct(&bar_bg_antfwd);
	bar_setxy(&bar_bg_antfwd,leftx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_bg_antfwd, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(ANTFWDPWR,pcfgdata->systemMode?"":"(Ant)"));
	bar_style(&bar_bg_antfwd, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_antfwd, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_antfwd, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_antfwd, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_antfwd, maxpwr/2, (maxpwr*2)/3, maxpwr);

	bar_initstruct(&bar_bg_antrev);
	bar_setxy(&bar_bg_antrev,leftx,starty + barynum++*(diffy+height), width, height);
	bar_title(&bar_bg_antrev, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(ANTREVPWR,pcfgdata->systemMode?"":"(Ant)"));
	bar_style(&bar_bg_antrev, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_antrev, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_antrev, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_antrev, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_antrev, maxpwr/2, (maxpwr*2)/3, maxpwr);

	bar_initstruct(&bar_bg_fltfwd);
	bar_setxy(&bar_bg_fltfwd,leftx,starty + barynum++*(diffy+height)+5, width, height);
	bar_title(&bar_bg_fltfwd, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(FLTFWDPWR,pcfgdata->systemMode?"":"(Flt)"));
	bar_style(&bar_bg_fltfwd, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_fltfwd, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_fltfwd, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_fltfwd, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_fltfwd, maxpwr/2, (maxpwr*2)/3, maxpwr);

	bar_initstruct(&bar_bg_fltrev);
	bar_setxy(&bar_bg_fltrev,leftx,starty + barynum++*(diffy+height)+5, width, height);
	bar_title(&bar_bg_fltrev, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(FLTREVPWR,pcfgdata->systemMode?"":"(Flt)"));
	bar_style(&bar_bg_fltrev, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_fltrev, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_fltrev, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_fltrev, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_fltrev, maxpwr/2, (maxpwr*2)/3, maxpwr);

	bar_initstruct(&bar_bg_drvfwd);
	bar_setxy(&bar_bg_drvfwd,leftx,starty + barynum++*(diffy+height) + 15, width, height);
	bar_title(&bar_bg_drvfwd, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(DRVFWDPWR,pcfgdata->systemMode?"":"(Drv)"));
	bar_style(&bar_bg_drvfwd, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_drvfwd, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_drvfwd, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_drvfwd, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_drvfwd, maxpwr/2, (maxpwr*2)/3, maxpwr);

	bar_initstruct(&bar_bg_drvrev);
	bar_setxy(&bar_bg_drvrev,leftx,starty + barynum++*(diffy+height) + 15, width, height);
	bar_title(&bar_bg_drvrev, &lucidaConsole_14ptFontInfo, textxshift, textyshift, getLangadd(DRVREVPWR,pcfgdata->systemMode?"":"(Drv)"));
	bar_style(&bar_bg_drvrev, BARSTYLE_FULL | BARSTYLE_LOG);
	bar_labelunit(&bar_bg_drvrev, &verdana_12ptFontInfo, getLang(W_RFout));
	maxpwr = getMaxPower();					// Power bei Vollausschlag
	makeLabelEntries_Power(&bar_bg_drvrev, &verdana_12ptFontInfo,maxpwr,pcfgdata->maxpwrunits);
	bar_value(&bar_bg_drvrev, &lucidaConsole_18ptFontInfo);
	bar_barinfo(&bar_bg_drvrev, maxpwr/2, (maxpwr*2)/3, maxpwr);
}

void show_mainbars()
{
	show_bar(&bar_out_power);
	show_bar(&bar_out_swr);
	if(pcfgdata->systemMode == 0)
	{
		show_bar(&bar_in_power);
		show_bar(&bar_efficiency);
	}
	show_bar(&bar_temperature1);
	show_bar(&bar_temperature2);
	show_bar(&bar_voltage);
	show_bar(&bar_current);
	show_bar(&bar_dc_power);
}

void show_bigbars()
{
	show_bar(&bar_big_power);
	show_bar(&bar_big_swr);
}

void show_bridge_bars()
{
	show_bar(&bar_bg_antfwd);
	show_bar(&bar_bg_antrev);
	show_bar(&bar_bg_fltfwd);
	show_bar(&bar_bg_fltrev);
	show_bar(&bar_bg_drvfwd);
	show_bar(&bar_bg_drvrev);
}

void bar_initstruct(t_bargraph *bar)
{
	memset(bar,0,sizeof(t_bargraph));
}

void bar_setxy(t_bargraph *bar, int leftx, int starty, int width, int height)
{
	bar->minx =leftx;
	bar->maxx = leftx+width;
	bar->miny = starty;
	bar->maxy = starty+height;
}

void bar_title(t_bargraph *bar, FONT_INFO *pfi, int x, int y, char *text)
{
	bar->titlefont = pfi;
	bar->titlex = bar->minx + x;
	bar->titley = bar->miny + y;
	strncpy(bar->titletext,text,29);
	bar->titletext[29] = 0;
}

void bar_style(t_bargraph *bar, int style)
{
	bar->style = style;
}

void bar_labelunit(t_bargraph *bar, FONT_INFO *pfi, char *text)
{
	bar->labelfont = pfi;
	strncpy(bar->labeltext,text,9);
	bar->labeltext[9] = 0;
}

void bar_labelentry(t_bargraph *bar, int x, int shift, char *text)
{
	bar->barlabel[bar->labelanz].posx = x;
	bar->barlabel[bar->labelanz].leftshift = shift;
	strncpy(bar->barlabel[bar->labelanz].text,text,9);
	bar->barlabel[bar->labelanz].text[9] = 0;
	bar->labelanz++;
}

void bar_value(t_bargraph *bar, FONT_INFO *pfi)
{
	bar->valuefont = pfi;
	bar->valuexdiff = 2;	// Abstand vom rechten Bar Rand

	// Höhe des Fonts ermitteln
	int tw, th;
	tft_textwidth_pixel(pfi, "A", &tw, &th);
	bar->valueydiff = ((bar->maxy - bar->miny) - th) /2 + 2;
}

void bar_barinfo(t_bargraph *bar, float vyellow, float vred, float v100)
{
	bar->val100precent = v100;
	bar->red = vred;
	bar->yellow = vyellow;
}


