/*
 * wifi.h
 *
 *  Created on: 29.10.2017
 *      Author: kurt
 */

#ifndef WIFI_H_
#define WIFI_H_

void wifi();
void init_wifi_uart();
void wifi_send(int len);
void handle_wifi(uint8_t data);
void check_wifi_commands();
void install_new_cfg();

#define MAXWIFIDATA 300
#define MAXWIFIRXDATA 300

extern uint8_t wifidata[MAXWIFIDATA+2];
extern uint8_t wifiRXdata[MAXWIFIRXDATA+2];
extern int wifi_emergoff;
extern char ipadr[21];
extern int wifi_active;
extern int wifi_standby;
extern int send_config;

#endif /* WIFI_H_ */
