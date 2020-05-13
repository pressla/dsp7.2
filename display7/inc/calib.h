/*
 * calib.h
 *
 *  Created on: 28.09.2017
 *      Author: kurt
 */

#ifndef CALIB_H_
#define CALIB_H_

void calib_drawvalues();
void calib_changevalue(int touchbutton);
void draw_menu_calib();
void calib_draw_buttons();
void calib_touch(int x, int y);
int do_button_calib(int touchbutton);
uint8_t calibvalsOK();

#endif /* CALIB_H_ */
