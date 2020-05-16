/*
Speichern von Daten im STM32 Flash

Adressen siehe tft_highlevel.c

Datenaufteilung:
================
im Ram gibt es drei identische Structs.
Bei einem Schreibvorgang werden alle drei auf einen Rutsch ins Flash geschrieben.
Beim Lesen wird eine korrekte Struktur gesucht
*/

#include <main.h>

unsigned int crc16_messagecalc(unsigned char *data, int len);

#define FLASHDATA_START 0x08080000	// Sector 8
#define DATASECTOR FLASH_Sector_8

t_fdata flashdata[3];
t_fdata *pcfgdata = &flashdata[0];   // hiermit kann im Programm gearbeitet werden

#define MAGIC 0x124ea7a9	// Ändern zum Löschen der Konfig

void setDefaults()
{
	pcfgdata->magic = MAGIC;

	pcfgdata->touch_left = TOUCH_DEFAULT_LEFT;
	pcfgdata->touch_right = TOUCH_DEFAULT_RIGHT;
	pcfgdata->touch_top = TOUCH_DEFAULT_TOP;
	pcfgdata->touch_bottom = TOUCH_DEFAULT_BOTTOM;

	pcfgdata->language = 0;
	pcfgdata->display_reverse = 0;
	pcfgdata->tempunits = 0;
	pcfgdata->maxpwrunits = 2;
	pcfgdata->maxdrvpwrunits = 2;
	pcfgdata->maxUunits = 1;
	pcfgdata->maxIunits = 1;
	pcfgdata->bandmode = 0;
	pcfgdata->shuntR = 0;

	pcfgdata->I_limit = 30;
	pcfgdata->U_limit = 51;
	pcfgdata->Temp_limit = 55;
	pcfgdata->Drive_limit = 10;
	pcfgdata->Time_limit_minutes = 5;
	pcfgdata->fan_temp = 40;
	pcfgdata->pttdelay = 0;
	pcfgdata->systemMode = 0;
	pcfgdata->auxInputMode = PTT4x; // 0=rotSwitch, 1=4xPTTin
	pcfgdata->civ_adr1 = 0;
	pcfgdata->civ_adr2 = 0;
	pcfgdata->civ_adr3 = 0;
	pcfgdata->civ_adr4 = 0;

	pcfgdata->ant_W_low = 3;
	pcfgdata->ant_W_high = 100;
	pcfgdata->ant_mV_low = 1666;
	pcfgdata->ant_mV_high = 2058;
	pcfgdata->flt_W_low = 3;
	pcfgdata->flt_W_high = 100;
	pcfgdata->flt_mV_low = 1561;
	pcfgdata->flt_mV_high = 1909;
	pcfgdata->drv_W_low = 2;
	pcfgdata->drv_W_high = 10;
	pcfgdata->drv_mV_low = 1412;
	pcfgdata->drv_mV_high = 1572;

	pcfgdata->antsw = 0;
	for(int i=0; i<11; i++)
	{
		pcfgdata->antsw_bandsel[0][i] = 0;
		pcfgdata->antsw_bandsel[1][i] = 0;
		pcfgdata->antsw_bandsel[2][i] = 0;
	}

	copy_ram_to_flash();
}

/*
 * es wird im Programm nur die erste Struktur benutzt.
 * Erst vor dem schreiben ins Flash wird sie dupliziert
*/

void copy_ram_to_flash()
{
int i;
uint32_t pflash = FLASHDATA_START;
unsigned char *pdata0 = (unsigned char *)(&flashdata[0]);
unsigned char *pdata1 = (unsigned char *)(&flashdata[1]);
unsigned char *pdata2 = (unsigned char *)(&flashdata[2]);

    // calc CRC
    flashdata[0].crc16 = crc16_messagecalc((unsigned char *)(&flashdata[0]), sizeof(t_fdata) - sizeof(int));

    // die abgelegten Werte stehen in pdata0
    // kopiere sie in die anderen beiden Strukturen
    for(i=0; i<sizeof(t_fdata); i++)
        pdata1[i] = pdata2[i] = pdata0[i];
    
    // unlock flash
    FLASH_Unlock();

    // lösche den ganzen Sector 8
    FLASH_EraseSector(DATASECTOR, VoltageRange_3);
    
    // kopiere die Daten
    for(i=0; i<sizeof(t_fdata); i++)
    	FLASH_ProgramByte(pflash++, pdata0[i]);

    for(i=0; i<sizeof(t_fdata); i++)
    	FLASH_ProgramByte(pflash++, pdata1[i]);

    for(i=0; i<sizeof(t_fdata); i++)
    	FLASH_ProgramByte(pflash++, pdata2[i]);

    // lock flash
    FLASH_Lock();

    setup_modified = 0;
    send_config = 1;	// send config to wifi
}

// 1=ok, 0=Fehler (reset to defaults)
int copy_flash_to_ram()
{
volatile unsigned char *pflash = (unsigned char *)FLASHDATA_START;
unsigned char *pdata0 = (unsigned char *)(&flashdata[0]);
unsigned char *pdata1 = (unsigned char *)(&flashdata[1]);
unsigned char *pdata2 = (unsigned char *)(&flashdata[2]);
int i;
unsigned int crc16;
int datasize = sizeof(t_fdata) - sizeof(int);

	// kopiere die Daten aus dem Flash ins Ram
    for(i=0; i<sizeof(t_fdata); i++)
        pdata0[i] = *pflash++;
    flashdata[0].magic = MAGIC;	// überschreibe, um eine geänderte Magic zu erkennen

    for(i=0; i<sizeof(t_fdata); i++)
        pdata1[i] = *pflash++;
    flashdata[1].magic = MAGIC;

    for(i=0; i<sizeof(t_fdata); i++)
        pdata2[i] = *pflash++;
    flashdata[2].magic = MAGIC;

    // prüfe crc16
    crc16 = crc16_messagecalc(pdata0, datasize);
    if(crc16 == flashdata[0].crc16) return 1; // alles ok
        
    // erste Struktur defekt, versuche zweite
    crc16 = crc16_messagecalc(pdata1, datasize);
    if(crc16 == flashdata[1].crc16) 
    {
        // ist ok
        for(i=0; i<sizeof(t_fdata); i++) pdata0[i] = pdata1[i];
        return 1;
    }
    
    // zweite Struktur defekt, versuche dritte
    crc16 = crc16_messagecalc(pdata2, datasize);
    if(crc16 == flashdata[2].crc16) 
    {
        // ist ok
        for(i=0; i<sizeof(t_fdata); i++) pdata0[i] = pdata2[i];
        return 1;
    }

    return 0;
}

// ====== CRC Funktionen ======

 unsigned int reg16 = 0xffff;         // Schieberegister
 
 unsigned int crc16_bytecalc(unsigned char byte)
 {
	 int i;
	 unsigned int polynom = 0x8408;        // Generatorpolynom

	 for (i=0; i<8; ++i)
	 {
		 if ((reg16&1) != (byte&1))
            reg16 = (reg16>>1)^polynom;
		 else
            reg16 >>= 1;
		 byte >>= 1;
	 }
	 return reg16;             // inverses Ergebnis, MSB zuerst
 }

 unsigned int crc16_messagecalc(unsigned char *data, int len)
 {
	 int i;

	 reg16 = 0xffff;					// Initialisiere Shift-Register mit Startwert
	 for(i=0; i<len; i++) {
		 reg16 = crc16_bytecalc(data[i]);		// Berechne fuer jeweils 8 Bit der Nachricht
	 }
	 return reg16;
 }
 
