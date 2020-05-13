/*
 * touchbutton.h
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#ifndef TOUCHBUTTON_H_
#define TOUCHBUTTON_H_


#define TOUCH_DEFAULT_LEFT 		3900
#define TOUCH_DEFAULT_RIGHT 	123
#define TOUCH_DEFAULT_TOP 		3950
#define TOUCH_DEFAULT_BOTTOM 	290

#define BANDBUTTONS_ANZ			12

enum _BUTTONNAMES {
	TB_ACTIVE = 0,
	TB_STANDBY = 1,
	TB_EMERG_OFF = 2,
	TB_MENU,
	TB_BAND1,
	TB_BAND2,
	TB_BAND3,
	TB_BAND4,
	TB_BAND5,
	TB_BAND6,
	TB_BAND7,
	TB_BAND8,
	TB_BAND9,
	TB_BAND10,
	TB_BAND11,
	TB_BAND12, // Autoband
	TB_P1000,
	TB_P100,
	TB_P10,
	TB_P1,
	TB_M1000,
	TB_M100,
	TB_M10,
	TB_M1,
	TB_GETVAL,
	TB_SETUP1,
	TB_SETUP2,
	TB_SETUP3,
	TB_SETUP4,
	TB_SETUP5,
	TB_SETUP6,
	TB_SETUP7,
	TB_SYSTEM1,
	TB_SYSTEM2,
	TB_SYSTEM3,
	TB_SYSTEM4,
	TB_SYSTEM5,
	TB_SYSTEM6,
	TB_SYSTEM7,
	TB_SYSTEM8,
	TB_SYSTEM9,
	TB_SYSTEM_2_1,
	TB_SYSTEM_2_2,
	TB_SYSTEM_2_3,
	TB_SYSTEM_2_4,
	TB_SYSTEM_2_5,
	TB_SYSTEM_2_6,
	TB_SYSTEM_2_7,
	TB_SYSTEM_2_8,
	TB_SYSTEM_2_9,
	TB_HIST100,
	TB_HIST250,
	TB_HIST500,
	TB_HIST1000,
	TB_KOP1,
	TB_KOP2,
	TB_KOP3,
	TB_ANTSW1,
	TB_ANTSW2,
	TB_ANTSW3,
	MAXBUTTONS
};

typedef struct _TOUCHBUTTON {
	int x;
	int y;
	int width;
	int height;
	int release_color;
	int press_color;
	int text_color;
	char text[20];
	FONT_INFO font;
	uint8_t picture;	// 0=no 1=yes
	char picname[10];
	char presspicname[10];
	// intern berechnete Daten
	int txpos;
	int typos;
	uint8_t pressed;
} t_touchbutton;

void init_touchbuttons();
void draw_touchbuttons_mainmenu();
void draw_touchbuttons_bottom();
void draw_setup_buttons();
void draw_touchbutton(t_touchbutton *ptb);
void process_touchbuttons();
void draw_touchbuttons_history();
void draw_touchbuttons_bridge();
void draw_touchbuttons_antswitch();
void setup_touchbutton(t_touchbutton *ptb, int x, int y, int w, int h, int color, int presscolor, int textcolor, char *text, FONT_INFO font,
						int picture, char *picname, char *presspicname);
void setup_touchbutton_presscolor(t_touchbutton *ptb, uint16_t col);
void blink_touchbutton(t_touchbutton *ptb, char *blinktext);
void update_touchref(int rawx, int rawy);

extern t_touchbutton tb_buttons[MAXBUTTONS];

#endif /* TOUCHBUTTON_H_ */
