/*
 * touchbutton.c
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#include <main.h>

t_touchbutton tb_buttons[MAXBUTTONS];

void init_touchbuttons()
{
	// Bottom Menu Buttons
	int x = 0;
	int w = 200;
	int w_swrpwr = 133;
	int yb = 420;
	int ybottom = TFT_HEIGHT-17;
	if(pcfgdata->language == 0)
		setup_touchbutton(&tb_buttons[TB_EMERG_OFF],x,yb+2,w,ybottom-yb,0,0,0,"",lucidaConsole_22ptFontInfo, 1,"bt_on","bt_off");
	else
		setup_touchbutton(&tb_buttons[TB_EMERG_OFF],x,yb+2,w,ybottom-yb,0,0,0,"",lucidaConsole_22ptFontInfo, 1,"bt_ein","bt_aus");
	x += w;
	if(pcfgdata->systemMode == 0)
	{
		// PA-Controller Modus
		setup_touchbutton(&tb_buttons[TB_ACTIVE],x,yb,w,ybottom-yb,BLUE,GREEN,WHITE,getLang(BTACTIVE),lucidaConsole_22ptFontInfo, 0,NULL,NULL);
		x += w;
		setup_touchbutton(&tb_buttons[TB_STANDBY],x,yb,w,ybottom-yb,BLUE,GREEN,WHITE,"STANDBY",lucidaConsole_22ptFontInfo, 0,NULL,NULL);
		x += w;
	}
	else if(pcfgdata->systemMode == 1)
	{
		// Power/SWR Modus
		setup_touchbutton(&tb_buttons[TB_KOP1],x,yb,w_swrpwr,ybottom-yb,BLUE,GREEN,WHITE,getLang(COUPLER1),lucidaConsole_22ptFontInfo, 0,NULL,NULL);
		x += w_swrpwr;
		setup_touchbutton(&tb_buttons[TB_KOP2],x,yb,w_swrpwr,ybottom-yb,BLUE,GREEN,WHITE,getLang(COUPLER2),lucidaConsole_22ptFontInfo, 0,NULL,NULL);
		x += w_swrpwr;
		setup_touchbutton(&tb_buttons[TB_KOP3],x,yb,w_swrpwr,ybottom-yb,BLUE,GREEN,WHITE,getLang(COUPLER3),lucidaConsole_22ptFontInfo, 0,NULL,NULL);
		x += w_swrpwr;
	}
	setup_touchbutton(&tb_buttons[TB_MENU],x,yb,w,ybottom-yb,RGB16(0,0,255),RGB16(0,255,0),WHITE,getLang(BTMENU),lucidaConsole_22ptFontInfo, 0,NULL,NULL);

	// Right Band Buttons
	int bandbuttons_bottom = 420;	// bis hierher geht der unterste Button
	int itemysize = (bandbuttons_bottom) / BANDBUTTONS_ANZ;
	int xs = 650;
	int text_rect_width = TFT_WIDTH - xs - 2;

	for(int i=0; i<BANDBUTTONS_ANZ; i++)
	{
		int y = i * itemysize;
		int ys = y+1;
		int ye = y + (itemysize-2);

		setup_touchbutton(&tb_buttons[TB_BAND1+i],xs,ys,text_rect_width,ye-ys+2,YELLOW,GREEN,BLACK,bandbuttons_text[pcfgdata->bandmode][i],lucidaConsole_18ptFontInfo, 0,NULL,NULL);
	}

	// Plus/Minus Buttons für die Setupeinstellungen
	int xbut = 490;
	int xbutspc = 80;
	int yup = 100;
	int ydn = 250;
	setup_touchbutton(&tb_buttons[TB_P1000],xbut,yup,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowup", "arruppr");
	setup_touchbutton(&tb_buttons[TB_P100] ,xbut+1*xbutspc,yup,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowup", "arruppr");
	setup_touchbutton(&tb_buttons[TB_P10]  ,xbut+2*xbutspc,yup,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowup", "arruppr");
	setup_touchbutton(&tb_buttons[TB_P1]   ,xbut+3*xbutspc,yup,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowup", "arruppr");
	setup_touchbutton(&tb_buttons[TB_M1000],xbut,ydn,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowdn", "arrdnpr");
	setup_touchbutton(&tb_buttons[TB_M100] ,xbut+1*xbutspc,ydn,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowdn", "arrdnpr");
	setup_touchbutton(&tb_buttons[TB_M10]  ,xbut+2*xbutspc,ydn,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowdn", "arrdnpr");
	setup_touchbutton(&tb_buttons[TB_M1]   ,xbut+3*xbutspc,ydn,64,64,0,0,0,"",lucidaConsoleBold_18ptFontInfo, 1, "arrowdn", "arrdnpr");
	setup_touchbutton(&tb_buttons[TB_GETVAL],xbut+1*xbutspc,340,64+xbutspc,52,RGB16(255,255,0),RGB16(0,255,0),BLACK,getLang(GETVALMV),lucidaConsoleBold_18ptFontInfo, 0,NULL,NULL);

	// Buttons für das Setup Menü
	init_setup_buttons();
	init_system_buttons();
	init_system_buttons2();

	// Buttons für die History
	int hb_wid = 140;
	int hb_step = 160;
	int hb_left = 15;
	int hbnum = 0;
	int hby = 370;
	int hb_height = 40;
	setup_touchbutton(&tb_buttons[TB_HIST100] ,hbnum++*hb_step+hb_left,hby,hb_wid,hb_height,DARKBLUE,BLUE,WHITE,"0.1s",lucidaConsole_14ptFontInfo, 0,NULL,NULL);
	setup_touchbutton(&tb_buttons[TB_HIST250] ,hbnum++*hb_step+hb_left,hby,hb_wid,hb_height,DARKBLUE,BLUE,WHITE,"0.25s",lucidaConsole_14ptFontInfo, 0,NULL,NULL);
	setup_touchbutton(&tb_buttons[TB_HIST500] ,hbnum++*hb_step+hb_left,hby,hb_wid,hb_height,DARKBLUE,BLUE,WHITE,"0.5s",lucidaConsole_14ptFontInfo, 0,NULL,NULL);
	setup_touchbutton(&tb_buttons[TB_HIST1000] ,hbnum++*hb_step+hb_left,hby,hb_wid,hb_height,DARKBLUE,BLUE,WHITE,"1s",lucidaConsole_14ptFontInfo, 0,NULL,NULL);

	// Buttons für die Antennenschalter
	int aleftx = 20;
	int atopy = 350;
	int awidth = 170;
	int aheight = 50;
	int aspace = awidth + 30;
	setup_touchbutton(&tb_buttons[TB_ANTSW1] ,aleftx,atopy,awidth,aheight,DARKBLUE,LIGHTBLUEANT,WHITE,"ANT-1",lucidaConsole_18ptFontInfo, 0,NULL,NULL);
	setup_touchbutton(&tb_buttons[TB_ANTSW2] ,aleftx+aspace*1,atopy,awidth,aheight,DARKBLUE,LIGHTBLUEANT,WHITE,"ANT-2",lucidaConsole_18ptFontInfo, 0,NULL,NULL);
	setup_touchbutton(&tb_buttons[TB_ANTSW3] ,aleftx+aspace*2,atopy,awidth,aheight,DARKBLUE,LIGHTBLUEANT,WHITE,"ANT-3",lucidaConsole_18ptFontInfo, 0,NULL,NULL);
}

void setup_touchbutton_presscolor(t_touchbutton *ptb, uint16_t col)
{
	ptb->press_color = col;
}

void setup_touchbutton(t_touchbutton *ptb, int x, int y, int w, int h, int color, int presscolor, int textcolor, char *text, FONT_INFO font,
						int picture, char *picname, char *presspicname)
{
	// speichere die Eigenschaften des Buttons
	ptb->x = x;
	ptb->y = y;
	ptb->width = w;
	ptb->height = h;
	ptb->release_color = color;
	ptb->press_color = presscolor;
	ptb->text_color = textcolor;
	ptb->font = font;
	strncpy(ptb->text, text, 19);
	ptb->text[19] = 0;
	ptb->picture = picture;
	strncpy(ptb->picname,picname,9);
	ptb->picname[9]=0;
	strncpy(ptb->presspicname,presspicname,9);
	ptb->presspicname[9]=0;

	// Berechne Position des Textes
	// Hole hor.Größe des Textes in Pixel
	int tw, th;
	tft_textwidth_pixel(&ptb->font, ptb->text, &tw, &th);
	ptb->txpos = (ptb->width - tw) / 2;
	if(ptb->txpos < 0) ptb->txpos = 0;
	ptb->typos = (ptb->height - th) / 2;
	if(ptb->typos < 0) ptb->typos = 0;

	ptb->txpos += ptb->x;
	ptb->typos += ptb->y;
}

void blink_touchbutton(t_touchbutton *ptb, char *blinktext)
{
char orig_text[20];
int orig_release_color;
int orig_press_color;
int orig_text_color;
FONT_INFO fnt = ptb->font;

	// merke originale Werte
	orig_release_color = ptb->release_color;
	orig_press_color = ptb->press_color;
	orig_text_color = ptb->text_color;
	strcpy(orig_text, ptb->text);

	ptb->release_color = YELLOW;
	ptb->press_color = YELLOW;
	ptb->text_color = BLACK;
	ptb->font = lucidaConsole_14ptFontInfo;
	strcpy(ptb->text,blinktext);
	draw_touchbutton(ptb);
	delay_1ms(500);

	/*for(int i=0; i<3; i++)
	{
		ptb->release_color = RED;
		ptb->press_color = RED;
		ptb->text_color = BLACK;
		strcpy(ptb->text,blinktext);
		draw_touchbutton(ptb);
		delay_1ms(250);

		ptb->release_color = YELLOW;
		ptb->press_color = YELLOW;
		ptb->text_color = BLACK;
		strcpy(ptb->text,"!!!");
		draw_touchbutton(ptb);
		delay_1ms(250);
	}*/

	// restauriere originale Werte
	ptb->release_color = orig_release_color;
	ptb->press_color = orig_press_color;
	ptb->text_color = orig_text_color;
	ptb->font = fnt;
	strcpy(ptb->text,orig_text);
	draw_touchbutton(ptb);
}

void draw_touchbutton(t_touchbutton *ptb)
{
	if(ptb == NULL) return;

	int color = ptb->release_color;
	char *fn = ptb->picname;
	if(ptb->pressed == 1)
	{
		color = ptb->press_color;
		fn = ptb->presspicname;
	}

	// Zeichne Button
	if(ptb->picture == 0)
	{
		// gezeichneter und beschrifteter Button
		TFT_Fill_Rectangle(ptb->x, ptb->y, ptb->x + ptb->width, ptb->y + ptb->height, color);
		TFT_Rectangle(ptb->x, ptb->y, ptb->x + ptb->width, ptb->y + ptb->height, BLACK, 2);
		tft_print(&ptb->font, ptb->text, ptb->txpos, ptb->typos, 1, ptb->text_color , color);
	}
	else
	{
		// Picture Button
		TFT_DrawFlash(fn,ptb->x, ptb->y);
	}
}

void draw_touchbuttons_history()
{
	for(int i=TB_HIST100; i<=TB_HIST1000; i++)
	{
		draw_touchbutton(&tb_buttons[i]);
	}
}

void draw_touchbuttons_bottom()
{
	for(int i=0; i<=TB_MENU; i++)
	{
		draw_touchbutton(&tb_buttons[i]);
	}
}

void draw_touchbuttons_mainmenu()
{
	draw_touchbutton(&tb_buttons[TB_EMERG_OFF]);
	draw_touchbutton(&tb_buttons[TB_MENU]);

	if(pcfgdata->systemMode == 0)
	{
		draw_touchbutton(&tb_buttons[TB_ACTIVE]);
		draw_touchbutton(&tb_buttons[TB_STANDBY]);
	}
	else if(pcfgdata->systemMode == 1)
	{
		draw_touchbutton(&tb_buttons[TB_KOP1]);
		draw_touchbutton(&tb_buttons[TB_KOP2]);
		draw_touchbutton(&tb_buttons[TB_KOP3]);
	}

	for(int i=TB_BAND1; i<=TB_BAND12; i++)
	{
		draw_touchbutton(&tb_buttons[i]);
	}

	activate_bandtouchbutton();	// aktiviere den zum gewählten Band zugehörigen Touchbutton
}

int secret_count = 0;

void process_touchbuttons()
{
	int x,y,rawx,rawy;
	static int ispressed = 0;

	int ret = XPT2046_GetAdXY(&x, &y,&rawx,&rawy);

	/*char s[50] = {"                      "};
	if(ret)
		sprintf(s,"x:%d y:%d           ",x,y);
	tft_print(&lucidaConsole_12ptFontInfo, s, 400, 400, 4, WHITE , BLACK);*/

	if(ret)
	{
		// Touch wurde gedrückt

		if(menunum == MENU_TOUCHCAL)
		{
			// Touch Kalibration, behandelt Dauerdrücken selbst
			touchcal(x,y,rawx,rawy);
			return;
		}

		if(ispressed == 1) return;	// es ist bereits gedrückt
		ispressed = 1;

		// Prüfe ob es bei der Secret Position ist (Selection Menü, rechts unten)
		/*if(menunum == MENU_SELECTION)
		{
			if(x>700 && y>400) secret_count++;
			else secret_count = 0;

			if(secret_count >= 10)
			{
				// Umschaltung PA/SWR Modus
				if(pcfgdata->systemMode == 1) pcfgdata->systemMode = 0;
				else pcfgdata->systemMode = 1;

				char s[30];
				sprintf(s,"SysMode now %s",pcfgdata->systemMode?"SWR":"PA");
				messagebox(s,2000);
				messagebox("save SysMode",1);
				copy_ram_to_flash();
				messagebox("Please power off-on",1);
				init_bars();
				init_touchbuttons();
				return;
			}
		}*/

		// zunächst die Aktionen welche nichts mit Buttons zu tun haben sondern
		// die Touchkoordinaten selbst bearbeiten
		if(menunum == MENU_SELECTION)
		{
			// Haupt-Auswahlmenu
			selection_menu(x,y);
			return;
		}
		if(menunum == MENU_SWITCH)
		{
			// Manuelle Ausgangsumschaltung
			switch_outputs(x,y);
		}

		if(menunum == MENU_CALIB)
		{
			calib_touch(x,y);
		}

		// jetzt suchen welcher Button gedrückt wurde
		for(int i=0; i<MAXBUTTONS; i++)
		{
			if(x > tb_buttons[i].x && x < (tb_buttons[i].x+tb_buttons[i].width) &&
			   y > tb_buttons[i].y && y < (tb_buttons[i].y+tb_buttons[i].height))
			{
				// Button gefunden
				// bearbeite das Kommando
				if(process_button(i))
					break;
			}
		}
	}
	else
	{
		// es ist kein Button gedrückt
		ispressed = 0;

		switch_outputs(-1,-1);		// melde das dem Switch Menü
		touchcal(-1,-1,-1,-1); 		// melde es der Touch Kalibration

		// release Number Buttons
		for(int i=TB_P1000; i<=TB_GETVAL; i++)
		{
			if(tb_buttons[i].pressed)
			{
				tb_buttons[i].pressed = 0;
				draw_touchbutton(&tb_buttons[i]);
			}
		}
	}
}
