/*
 * setup.h
 *
 *  Created on: 27.09.2017
 *      Author: kurt
 */

#ifndef SETUP_H_
#define SETUP_H_

void draw_menu_setup();
void setup_changevalue(int touchbutton);
void init_setup_buttons();
int do_button_setup(int touchbutton);
void setup_drawvalues();

extern int setup_modified;

#endif /* SETUP_H_ */
