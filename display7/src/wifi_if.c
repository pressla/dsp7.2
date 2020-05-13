/*
 * wifi_if.c
 *
 *  Created on: 28.10.2017
 *      Author: kurt
 */

// Interface to the ESP8266 Wifi Controller

#include <main.h>

int build_wifi_data();

uint8_t wifidata[MAXWIFIDATA + 2];
uint8_t wifiRXdata[MAXWIFIRXDATA + 2];
uint8_t *pwdata;

#define MAXDATASET	2
int dataset = 0;
int send_config = 0;

/*
 * Datenaufbau Stm32->Wifi
 *
 * 4 byte ... Header
 * 2 byte ... Länge len
 * len byte ... Inhalt
 * 2 byte ... crc16
 */

void add_crc16(int len)
{
	uint16_t crc16 = crc16_messagecalc(wifidata, len);
	*pwdata++ = (crc16 >> 8);
	*pwdata = (crc16 & 0xff);
}

void wifi()
{
	// prüfe ob neue Config angekommen ist
	install_new_cfg();

	// sende Messdaten
	// mache das 10x pro Sekunde
	if(wifi_1s == 0)	// wird vom Timer heruntergezählt
	{
		wifi_1s = 100;

		if(send_config)
		{
			dataset = 2;
			send_config = 0;
		}
		int len = build_wifi_data();	// len hat Header und Längenbytes schon mit dabei
		if(++dataset >= MAXDATASET) dataset = 0;

		wifidata[4] = (len >> 8);
		wifidata[5] = (len & 0xff);

		add_crc16(len); // mit Header und Länge

		if(len < MAXWIFIDATA)
			wifi_send(len+2);		// sende via DMA aus
		// else ... Puffer zu klein definiert
	}
}

// Einfügefunktionen in den Wifi Datensatz

void wifi_insert_32bit(int32_t iv)
{
	*pwdata++ = iv >> 24;
	*pwdata++ = iv >> 16;
	*pwdata++ = iv >> 8;
	*pwdata++ = iv;
}

void wifi_insert_float(float fval)
{
	// sende einen float als 32bit int, als das 10fache, also mit einer Kommastelle
	wifi_insert_32bit((int32_t)(fval * 10));
}

void wifi_insert_8bit(uint8_t v)
{
	*pwdata++ = v;
}

void wifi_insert_config()
{
int len = sizeof(flashdata[0]);
uint8_t *p = (uint8_t *)pcfgdata;

	for(int i=0; i<len; i++)
	{
		*pwdata++ = *p++;
	}
}

// baue einen Datensatz zusammen, sende diesen dann via DMA aus

void insert_pwrswr(t_pwrswr *p)
{
	wifi_insert_float(p->fwd_watt);
	wifi_insert_float(p->fwd_peakwatt);
	wifi_insert_float(p->fwd_dBm);
	wifi_insert_float(p->fwd_dBmpeak);
	wifi_insert_float(p->rev_watt);
	wifi_insert_float(p->rev_dBm);
	wifi_insert_float(p->swr);
}

int build_wifi_data()
{
	pwdata = wifidata;

	if(dataset == 0)
	{
		// Header
		wifi_insert_32bit(0x1e7cfe56);	// feste Bitkombination zur Start-Erkennung

		pwdata+=2;	// Platz für Länge

		// diverse Status- und Configwerte
		wifi_insert_8bit(state);
		wifi_insert_8bit(pcfgdata->antsw);
		wifi_insert_32bit(_selected_band);
		wifi_insert_8bit(gpio_stat[PTTOUT]);
		wifi_insert_8bit(gpio_stat[FAN_ONOFF]);
		wifi_insert_8bit(pcfgdata->tempunits);
		wifi_insert_8bit(pcfgdata->bandmode);
		wifi_insert_32bit(uptime);

		// adc_realvals
		for(int i=0; i<NUMADCVALS; i++)
		{
			wifi_insert_float(adc_realvals[i]);
		}
	}

	if(dataset == 1)
	{
		// Header
		wifi_insert_32bit(0x6478ab61);	// feste Bitkombination zur Start-Erkennung

		pwdata+=2;	// Platz für Länge

		wifi_insert_8bit(pcfgdata->systemMode); // 0=PA-Controller, 1=PwrSwrMeter

		// Pwr/Swr Werte
		insert_pwrswr(&pwrswr_antenna);
		insert_pwrswr(&pwrswr_filter);
		insert_pwrswr(&pwrswr_input);
	}

	if(dataset == 2)
	{
		// Header
		wifi_insert_32bit(0x01020305);	// feste Bitkombination zur Start-Erkennung

		pwdata+=2;	// Platz für Länge

		wifi_insert_config();
	}

	int len = (int)((uint32_t)pwdata - (uint32_t)wifidata);

	return len;
}

// ===== Empfang vom WiFi ============

int wifi_emergoff = 0;
int wifi_on = 0;
int wifi_active = 0;
int wifi_standby = 0;
char ipadr[21] = {"WLan ... "};
int wifi_connected = 0;
int rxcfglen = 0;
unsigned char rxcfg[300];
int rxcfg_ready = 0;

// wird aus dem UART RX IRQ aufgerufen
void handle_wifi(uint8_t data)
{
static int state = 0;
static int idx = 0;

	switch (state)
	{
	// Start Kennung: 01 02 03 04
	case 0: if(data == 0x01) state++; break;
	case 1: if(data == 0x02) state++; else state = 0; break;
	case 2: if(data == 0x03) state++; else state = 0; break;

	case 3: if(data == 0x01) { wifi_emergoff = 1; 	state = 0; break; }
			if(data == 0x02) { wifi_on = 1; 		state = 0; break; }
			if(data == 0x03) { wifi_standby = 1; 	state = 0; break; }
			if(data == 0x04) { wifi_active = 1; 	state = 0; break; }

			if(data == 0x05)
			{
				if(rxcfg_ready == 1)
				{
					// die alte Config wurde noch nicht verarbeitet
					state = 0;
					break;
				}

				idx = 0;
				rxcfg[idx++] = 1;
				rxcfg[idx++] = 2;
				rxcfg[idx++] = 3;
				rxcfg[idx++] = 5;
				state = 100;
				break;
			}

			if(data == 6)
			{
				// myIP
				idx = 0;
				state = 200;
				break;
			}

			if(data == 7)
			{
				// Wifi wurde neu gestartet, schicke die Config runter
				send_config = 1;
				state = 0;
				break;
			}

			state = 0; break;

			// Einlesen der Konfig
	case 100:
			rxcfg[idx++] = data;
			rxcfglen = data;
			rxcfglen <<= 8;
			state++;
			break;
	case 101:
			rxcfg[idx++] = data;
			rxcfglen |= data;
			rxcfglen -= 6;	// abzgl Header und Länge
			rxcfglen += 2;	// zuzgl. CRC16
			state++;
			break;
	case 102:
			rxcfg[idx++] = data;
			rxcfglen--;
			if(rxcfglen == 0)
			{
				rxcfglen = idx-2;
				rxcfg_ready = 1;
				state = 0;
			}
			break;

	case 200:
			if(idx < 20) ipadr[idx++] = data;
			if(idx == 20)
			{
				ipadr[idx] = 0;
				if(wifi_connected == 0)
				{
					wifi_connected = 1;
					send_config = 1;	// send config to wifi, nachdem die IP erstmals bekannt wurde, danach bei jeder Änderung der Config
				}
				state = 0;
			}
			break;
	}
}

void install_new_cfg()
{
	if(rxcfg_ready == 0) return;

	// der cfg vom WiFi wurde im usart irq eingelesen
	// prüfe die CRC16
	uint16_t crc16 = crc16_messagecalc(rxcfg, rxcfglen);
	uint16_t rxcrc16 = rxcfg[214];
	rxcrc16 <<= 8;
	rxcrc16 |= rxcfg[215];

	if(crc16 == rxcrc16)
	{
		// die neue Config ist OK
		// sie beginnt bei rxcfg[6] und hat die Länge rxcfglen-6 (abzgl. header, len)
		int len = sizeof(flashdata[0]);
		uint8_t *pcfgstruct = (uint8_t *)pcfgdata;
		uint8_t *pnewcfg = (uint8_t *)rxcfg + 6;

		if(len == (rxcfglen-6))
		{
			// die Länge stimmt auch
			// kopiere sie in die Config
			for(int i=0; i<len; i++)
			{
				*pcfgstruct++ = *pnewcfg++;
			}
			copy_ram_to_flash();
			messagebox(getLang(NEWCFG),1000);
			messagebox("Rebooting",1000);
			NVIC_SystemReset();
		}
	}
	rxcfg_ready = 0;	// fertig
}

void check_wifi_commands()
{
	if(wifi_emergoff == 1)
	{
		wifi_emergoff = 0;
		switchto_emergencymode();
	}

	if(wifi_on == 1)
	{
		wifi_on = 0;
		switchto_ON();
	}

	if(wifi_active == 1)
	{
		wifi_active = 0;
		switchto_active();
	}

	if(wifi_standby == 1)
	{
		wifi_standby = 0;
		switchto_standby();
	}
}
