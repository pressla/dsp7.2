/*
 * EEPROM Map:
 * 
 * 0, len=50 ..... SSID
 * 50, len=50..... WiFi Password
 * 100, len=15 ... Callsign
 * 115, len=1 .... Sprache
 * 116, len=1 .... Refreshtime
 * 117,  ...
 * 
 * 504, len=4 .... magic
 * 508, len=4 .... checksum
 * 
 */
#include <EEPROM.h>

int dx=0;
int reserved_eeprom_size = 3350;

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);

    delay(50);  // this resets also the WDT watchdog, important for long EEPROM usage
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);

    delay(50);  // this resets also the WDT watchdog, important for long EEPROM usage
    return i;
}

void ee_begin()
{
  EEPROM.begin(reserved_eeprom_size); // max.4096, angeblich nutzt es auch RAM daher nicht viel mehr als notwendig nehmen
}

void ee_endread()
{
  EEPROM.end();
}

void ee_endwrite()
{
  EEPROM.commit();
  EEPROM.end();
}

void writeEEPROM()
{
int adr=10; // die ersten 10 lasse frei für Magic, Chksum

  led_access_eeprom(1);
  ee_begin();

  EEPROM_writeAnything(adr,ssid);      adr += sizeof(ssid);
  EEPROM_writeAnything(adr,password);  adr += sizeof(password);
  EEPROM_writeAnything(adr,accesscode);  adr += sizeof(accesscode);
  EEPROM_writeAnything(adr,callsign);  adr += sizeof(callsign);
  EEPROM_writeAnything(adr,sprache);  adr += sizeof(sprache);
  EEPROM_writeAnything(adr,html_refreshtime);  adr += sizeof(html_refreshtime);

  dx = adr;

  ee_endwrite();

  ee_writeChecksum();
  led_access_eeprom(0);

  eeprom_printf("EEPROM geschrieben. Reserve %d Bytes",reserved_eeprom_size-adr);
}

char readEEPROM()
{
int adr = 10;
char ok=1;

  led_access_eeprom(1);
  ee_begin();

  if(ee_readChecksum())
  {
    EEPROM_readAnything(adr,ssid);          adr += sizeof(ssid);
    EEPROM_readAnything(adr,password);      adr += sizeof(password);
    EEPROM_readAnything(adr,accesscode);    adr += sizeof(accesscode);
    EEPROM_readAnything(adr,callsign);      adr += sizeof(callsign);
    EEPROM_readAnything(adr,sprache);       adr += sizeof(sprache);
    EEPROM_readAnything(adr,html_refreshtime);  adr += sizeof(html_refreshtime);

    //eeprom_printf("EEPROM erfolgreich gelesen.");

    dx = adr;
  }
  else
  {
    // init some arrays
    eeprom_printf("EEPROM Neustart mit Defaultwerten");
    ok=0;
  }
  ee_endread();
  led_access_eeprom(0);
  return ok;
}


#define MAGIC 0x5c2b4e19

void ee_writeChecksum()
{
int i;
unsigned long wchksum=0;
unsigned long _MAGIC = MAGIC;

  ee_begin();
  for(i=10; i<reserved_eeprom_size; i++)
  {
    wchksum += EEPROM.read(i);
  }
  ee_endread();

  delay(100);

  ee_begin();
  EEPROM_writeAnything(4,wchksum);
  EEPROM_writeAnything(0,_MAGIC);
  ee_endwrite();
}

unsigned long magic=0x12345678;
char ee_readChecksum()
{
int i;
unsigned long rchksum=0, uist;

  for(i=10; i<reserved_eeprom_size; i++)
  {
    rchksum += EEPROM.read(i);
  }
  
  EEPROM_readAnything(4,uist);
  EEPROM_readAnything(0,magic);

  if(magic == MAGIC && uist == rchksum) return 1;
  
  return 0;
}


