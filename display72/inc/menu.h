/*
 * menu.h
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#ifndef MENU_H_
#define MENU_H_

int process_button(int touchbutton);
void selection_menu(int x, int y);
void draw_values();
void switch_outputs(int x, int y);
void touchcal(int x, int y, int rawx, int rawy);
void draw_meter_bar();
void draw_menu_startup();
void draw_menu_emergency(float soll, float ist);
void statusline();
void draw_menu_frame();
void switchto_emergencymode();
void switchto_ON();
int switchto_active();
void switchto_standby();
unsigned char changeValue(int touchbutton, unsigned char v, unsigned char max);


enum _MENUNUM {
	MENU_SELECTION = 0,
	MENU_OVERVIEW,
	MENU_BAR,
	MENU_METER,
	MENU_BRIDGES,
	MENU_HISTORY,
	MENU_ADC,
	MENU_VALUES,
	MENU_STATUS,
	MENU_SWITCH,
	MENU_ANTSW,
	MENU_SETUP,
	MENU_CALIB,
	MENU_SYSTEM1,
	MENU_SYSTEM2,
	MENU_TOUCHCAL,
	MENU_STARTUP
};

extern int menunum;
extern uint8_t showstatus;

#endif /* MENU_H_ */
