/*
 * bargraph_defines.h
 *
 *  Created on: 24.09.2017
 *      Author: kurt
 */

#ifndef BARGRAPH_DEFINES_H_
#define BARGRAPH_DEFINES_H_

void init_bars();
void show_mainbars();
void show_bigbars();

// Übersichtsfenster
extern t_bargraph bar_out_power;
extern t_bargraph bar_out_swr;
extern t_bargraph bar_in_power;
extern t_bargraph bar_voltage;
extern t_bargraph bar_current;
extern t_bargraph bar_dc_power;
extern t_bargraph bar_efficiency;
extern t_bargraph bar_temperature1;
extern t_bargraph bar_temperature2;

// grosses Balkenfenster
extern t_bargraph bar_big_power;
extern t_bargraph bar_big_swr;

// Bridges Fenster
extern t_bargraph bar_bg_antfwd;
extern t_bargraph bar_bg_antrev;
extern t_bargraph bar_bg_fltfwd;
extern t_bargraph bar_bg_fltrev;
extern t_bargraph bar_bg_drvfwd;
extern t_bargraph bar_bg_drvrev;


#endif /* BARGRAPH_DEFINES_H_ */
