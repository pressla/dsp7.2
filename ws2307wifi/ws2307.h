enum {
  VALUE1 = 0,
  VALUE2,
  PHASE,
  LASTVAL,
  ARROWDIR,
  MAX_SIZE
};

extern char s_acttime[];
extern char s_actdate[];

extern unsigned long magic;
extern unsigned long html_refreshtime;
extern int dx;

extern int ax,bx,cx;
extern char sprache;
extern char *titles_english[];
extern unsigned int querynum;

extern char callsign[];
extern char eeprom_status[];
extern char wifi_msg[],aprs_debmsg[],wx_msg[],eeprom_msg[];

#define RXSTRLEN 300
extern unsigned char ws_rxdata[RXSTRLEN];
extern unsigned char tempunits; 


// List of all values measured, calculated or just displayed in the html table
enum {
  // Status
  OPSTATE = 0,
  PTT,
  
  // Band
  BAND_SELECTED,
  
  // Antenna
  ANTENNA_SELECTED,
  
  // Temperature
  TEMPERATURE1,
  TEMPERATURE2,
  FAN,

  // Power-Supply
  DCVOLT,
  DCAMP,
  DCPWR,
  EFFICIENCY,

  // Bridge-1
  B1_PWR,
  B1_SWR,
  
  // Bridge-2
  B2_PWR,
  B2_SWR,

  // Bridge-3
  B3_PWR,
  B3_SWR,

  // separator
  SEPARATOR,
  
  // Debug data
  IPADDRESS,
  WIFISTATUS,
  RSSIVAL,
  
  MAXVALUES
};

extern char t_vals[MAXVALUES][25];

#define VALSTRLEN 25
#define LASTVALANZ  10

typedef struct {
  short ival;             // actual value in 1/10 steps
  short lastvals[LASTVALANZ];  // remember the last 4 values
  char valid;             // 1=the first valid value was received
  short prevval;          // previous value for tendency detection
  char arrowdir;          // tendency: 1=up 2=down
  char sval[VALSTRLEN];   // actual value as printable string
} WXVALS;

extern WXVALS wxval[];

// full color LED ports
#define LED_GN  12
#define LED_BL  5
#define LED_RD  4

#define LEDON 0
#define LEDOFF 1

