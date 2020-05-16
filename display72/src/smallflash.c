/*
 * smallflash.c
 *
 *  Created on: 01.01.2018
 *      Author: kurt
 */

/* Sektor 1: 16kB zur Speicherung von wenigen Runtime Dingen, wie aktuelle GUI Einstellungen usw.
 *
 * von t_smalldata passen in diesen Sektor 16384 / sizeof(smallflash) Stück
 * wenn das ROM gelöscht wird, ist alles FF
 * das erste Bit im ersten Byte in t_smalldata ist 0 und wird als Kennung benutzt um das letzte aktuelle zu finden
 *
 * wenn das ROM vollgeschrieben ist, wird es gelöscht und es beginnt wieder von vorne
 */

#include <main.h>

void save_smalldata();
int seekNextFreePos();
int seekActualPos();

#define SMALLFLASHDATA_START 	0x08004000		// Sector 1
#define SMALLDATASECTOR 		FLASH_Sector_1
#define SMALLFLASHLENGTH     	0x4000			// 16k

int sf_autoband = 1;
int sf_startmenu = MENU_BAR;
t_smalldata smallflash;

void store_autoband(uint8_t onoff)
{
	if(sf_autoband != onoff)
	{
		sf_autoband = onoff;
		if(onoff == 0)
		{
			sf_autoband = 0; // für Breakpoint only
		}
		save_smalldata();
	}
}

void store_startmenu()
{
	if(menunum != sf_startmenu)
	{
		if(menunum >= MENU_OVERVIEW && menunum <= MENU_HISTORY)
		{
			sf_startmenu = menunum;
			save_smalldata();
		}
	}
}

void restore_smallflash_data()
{
	int actpos = seekActualPos();

	if(actpos == -1)
	{
		// kein Block da, setze Defaultwerte
		sf_autoband = 0;
		sf_startmenu = MENU_BAR;
		return;
	}

	volatile unsigned char *pflash = (unsigned char *)(SMALLFLASHDATA_START + actpos * sizeof(smallflash));

	if(pflash[0] & 2)
		sf_autoband = 1;
	else
		sf_autoband = 0;

	sf_startmenu = (pflash[0] >> 2) & 7;
	if(sf_startmenu < MENU_OVERVIEW || sf_startmenu > MENU_HISTORY) sf_startmenu = MENU_BAR;
}

void save_smalldata()
{
	// Baue den Datenblock auf
	smallflash.var0 = 0xfe;	// Startkennung (Bit 0 des ersten Bytes ist 0)

	if(sf_autoband == 0)
		smallflash.var0 &= ~2;	// Bit 1 ist das autoband onoff Bit

	// Bit 2,3,4 ist die Menünummer des Startmenüs (1 bis 5)
	if(sf_startmenu < MENU_OVERVIEW || sf_startmenu > MENU_HISTORY) sf_startmenu = MENU_BAR;
	smallflash.var0 &= ~(7<<2);
	smallflash.var0 |= ((sf_startmenu & 7) << 2);

	// Suche nächste Position, wenn voll dann lösche den Sektor
	int pos = seekNextFreePos();

	uint32_t pflash = SMALLFLASHDATA_START + pos * sizeof(smallflash);
	unsigned char *psfdata = (unsigned char *)(&smallflash);

	// schreibe Daten ins Flash
	FLASH_Unlock();

	for(int i=0; i<sizeof(smallflash); i++)
	{
		FLASH_ProgramByte(pflash++, psfdata[i]);
	}

	FLASH_Lock();
}

int seekNextFreePos()
{
int blockanz = SMALLFLASHLENGTH / sizeof(smallflash);	// Anzahl der Blöcke die ins 16k ROM passen
volatile unsigned char *pflash = (unsigned char *)SMALLFLASHDATA_START;

	for(int i=0; i<blockanz; i++)
	{
		// ist die Kennung gesetzt ?
		if((*pflash & 0x01) == 1)
		{
			return i;	// dieser Block ist noch frei
		}
		// gehe zum nächten Block
		pflash += sizeof(smallflash);
	}

	// es wurde kein freier Block gefunden, lösche das ROM
    FLASH_Unlock();
	FLASH_EraseSector(SMALLDATASECTOR, VoltageRange_3);
	FLASH_Lock();

	return 0;	// und gebe den ersten zurück
}

int seekActualPos()
{
	int blockanz = SMALLFLASHLENGTH / sizeof(smallflash);	// Anzahl der Blöcke die ins 16k ROM passen
	volatile unsigned char *pflash = (unsigned char *)SMALLFLASHDATA_START;

	for(int i=0; i<blockanz; i++)
	{
		// ist die Kennung gesetzt ?
		if((*pflash & 0x01) == 1)
		{
			// dieser Block ist noch frei,
			// der vorherige ist also der aktuelle
			return i-1;	// wenn -1, dann gibt es noch keinen Block
		}
		// gehe zum nächten Block
		pflash += sizeof(smallflash);
	}

	return -1;
}
