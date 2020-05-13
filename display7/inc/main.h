/*
 * main.h
 *
 *  Created on: 19.09.2017
 *      Author: kurt
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "stm32f4xx_fmc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <tft.h>
#include <font.h>
#include <flashdata.h>
#include <ff.h>
#include <adc.h>
#include <bargraph.h>
#include <bargraph_defines.h>
#include <touchbutton.h>
#include <menu.h>
#include <pwrswr.h>
#include <gpio.h>
#include <setup.h>
#include <calib.h>
#include <system.h>
#include <language.h>
#include <instrument.h>
#include <controller.h>
#include <wifi.h>
#include <bandsel.h>
#include <smallflash.h>
#include <fan.h>

int main(void);
void delay_1ms(int ms);
int check_Downcounter();
void set_Downcounter(unsigned int ms);
void TIM3_Initialization();
void init_LEDs();
void switch_LED(int led, int onoff);
void touch_init();
int copy_flash_to_ram();
void copy_ram_to_flash();
void mainloop();
void show_raw_values();
void init_freq_meas();
int XPT2046_GetAdXY(int *x,int *y, int *rawx, int *rawy);
int set_touchref(int newleft, int newright, int newtop, int newbottom);
float calc_temp(unsigned long uin);
void messagebox(char *text, int duration_ms);
void hist_draw_frame();
void hist_insertdata();
int do_button_history(int touchbutton);
void show_histdata();
int log_percent(float value, float maxval);
int lin_log(float value, float maxval, int maxres);
void select_band(int button);
void bandselection();
void autoband_off();
void control();
int getMaxPower();
int getMaxDrvPower();
int lin_log_percent_swr(float value, float maxval);
int CelsiusToFahrenheit(int c);
int do_button_bridges(int touchbutton);
int do_button_antsw(int touchbutton);
void antennaswitch();
void draw_antswmenu();
void activate_bandtouchbutton();
void set_LED();
void init_CIV_uart();
void civ_send(uint8_t *pdata, int len);
void civ_request_frequency();
uint32_t getCIVfreq();

extern t_fdata *pcfgdata;
extern int h_power[8][5];
extern float h_swr[5];
extern float maxUunits[4];
extern float maxIunits[4];
extern volatile unsigned int hist_1s;
extern volatile unsigned int hist_10ms;
extern volatile unsigned int barupdate_100ms;
extern volatile uint32_t uptime;
extern volatile unsigned int wifi_1s;
extern volatile unsigned int txtime;
extern volatile unsigned int civ_period;
extern volatile unsigned int swr_filter_fail;
extern volatile unsigned int swr_ant_fail;
extern volatile unsigned int fwd_drv_fail;
extern int maxpwr[4];
extern int maxdrvpwr[4];
extern int _selected_band;
extern int systemMode;
extern uint8_t civ_avail;
extern int storeStartMenuFlag;
extern uint8_t civ_adr;
extern uint32_t civ_freq;

#define FWVERSION "DSP-7 V1.7"
#define BOARDVERSION 72			// 71 oder 72

enum LEDs {
	LEDOFF = 0,
	LEDON,
	LEDRD,
	LEDGN
};


#endif /* MAIN_H_ */
