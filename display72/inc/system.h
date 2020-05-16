/*
 * system.h
 *
 *  Created on: 30.09.2017
 *      Author: kurt
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

void draw_values_system();
void draw_values_system2();
void draw_menu_system();
void draw_menu_system2();
void draw_system_buttons();
void draw_system_buttons2();
void init_system_buttons();
void init_system_buttons2();
int do_button_system(int touchbutton);
int do_button_system2(int touchbutton);
void setDisplayReverse();

extern float shuntValue[4];

#endif /* SYSTEM_H_ */
